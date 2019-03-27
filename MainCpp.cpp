#include<graphics.h>
#include<cstdlib>

using namespace std;

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_ZOOMIN		16
#define	CMD_ZOOMOUT		32
#define	CMD_QUIT		64

const int g_y0 = 400;//地面高度

					 //函数声明
void Init();						// 初始化
void Quit();						// 退出
int  GetCommand();					// 获取控制命令
void DispatchCommand(int _cmd);		// 分发控制命令
void DispatchCommand(int _cmd, char jump);// 分发控制命令(Jumping)
inline void OnLeft();
inline void OnUp();
inline void OnRight();
inline void OnLeft();
inline void OnDown();
inline void Bomp(float value, int direction);//弹射出去 类似于跳跃 但是可以设置力度和方向 内部采用浮点 1左 2右 3上 4向下
////////////////////////////////////////////////////
class Ball {
private:
	int ballx, bally, ballr;
	char ballcolor = 1;//1-BLUE
public:
	Ball(int x, int y, int r) :ballx(x), bally(y), ballr(r) {};//构造函数
	inline void printCircle();//画圆
	inline void setBallColor(int color);//设置圆的颜色（花纹）
	inline void moveball(int value, char type);//移动 1左 2右 3上 4下(向某个方向移动的距离)
	inline void setballxy(int x, int y);//直接设置球的位置 （根据坐标系设置）
	inline int getBally();//获取球当前所在y
	inline int getBallx();//获取球当前所在x
	inline int getBallr();//获取球当前r
};

inline void Ball::printCircle() {
	solidcircle(ballx, bally, ballr);
}

inline void Ball::setBallColor(int color) {
	switch (color) {
	case 0:
		setfillcolor(BLACK);
		break;
	case 1:
		setfillcolor(BLUE);
		break;
	}
}

inline void Ball::setballxy(int x, int y) {
	ballx = x;
	bally = y;
}

inline void Ball::moveball(int value, char type) {
	switch (type) {
	case 1:
		ballx = ballx - value;
		if (ballx <= 0 + ballr) {
			ballx = ballr;
		}
		break;
	case 2:
		ballx = ballx + value;
		if (ballx >= 640 - ballr) {
			ballx = 640 - ballr;
		}
		break;
	case 3:
		bally = bally - value;
		if (bally <= 0 + ballr) {
			//bally = ballr;// 1.0.1 向上跳出边界检测 掉落停滞 暂时性修复
		}
		break;
	case 4:
		bally = bally + value;
		if (bally >= 640 - ballr) {
			bally = 640 - ballr;
		}
		break;
	}
}

inline int Ball::getBally() {
	return bally;
}

inline int Ball::getBallx() {
	return ballx;
}

inline int Ball::getBallr() {
	return ballr;
}

///////

class Block {//地图中的固定方块
private:
	int blockx, blocky, blocktype, attack, height, width;//attack实际上是一个数值，具体含义对应不同type有不同
	char type;//击退等等 无=0，击退=1；
public:
	Block();
	Block(int x, int y, int l, int h, int att, int type) :blockx(x), blocky(y), blocktype(type), height(h), width(l), attack(att) {};//构造函数
	void printBlock();//打印地图方块
	inline int x_inblock(int x, int r);//球x是否在方块x范围内？1=在
	inline int getBlockBottom();
	inline int getBlockTop();
	inline int getBlockLeft();
	inline int getBlockRight();
	inline int getBlockType();
	inline int getBlockAtt();
};

Block::Block() {}

void Block::printBlock() {
	switch (type) {
	case 0:
		setfillcolor(BLACK);
	}
	solidrectangle(blockx, blocky, blockx + width, blocky + height);
}

inline int Block::x_inblock(int x, int r) {
	if ((x + r) > blockx && (x - r) < (blockx + width)) {
		return 1;
	}
	else {
		return 0;
	}
}

inline int Block::getBlockBottom() {
	return (blocky + height);
}

inline int Block::getBlockTop() {
	return blocky;
}

inline int Block::getBlockLeft() {
	return blockx;
}

inline int Block::getBlockRight() {
	return blockx + width;
}

inline int Block::getBlockType() {
	return type;
}

inline int Block::getBlockAtt() {
	return attack;
}

///////
////////////////////////////////////////

Ball Mainball(200, 380, 20);//主人公球球
Block Normalblock[20];//block的数组
int blocknum = 2;//block有多少个
char isjumping = 0;//是否在向上跳  1=是
char isdroping = 0;//是否在下落   1=是

int main() {
	Init();//初始化函数
	int c;
	do {
		c = GetCommand();
		DispatchCommand(c);
		if (isjumping != 1) {
			OnDown();//下落（向上跳时不下落 其余时候都监测是否在盒子上或者地面上 如果不是就下落 如果是就不下落）
		}
		Sleep(20);//10
	} while (!(c & CMD_QUIT));
	Quit();
}

// 初始化
void Init() {
	// 设置绘图屏幕和绘图模式
	initgraph(640, 480);
	setwritemode(R2_XORPEN);
	//方块初始化（测试中）
	Normalblock[0] = Block(100, 150, 180, 100, 0, 0);//x,y 宽度 厚度 数值 类型
	Normalblock[1] = Block(300, 300, 50, 50, 2, 1);
	// 显示操作说明
	outtextxy(20, 270, _T("Baaaall Beta 1.0.1"));
	outtextxy(20, 290, _T(" ↑   Jump"));
	outtextxy(20, 310, _T(" ←   Left"));
	outtextxy(20, 330, _T(" →   Right"));
	outtextxy(20, 350, _T("ESC  Exit"));
	//画地图
	////地面
	line(0, 400, 640, 400);
	////方块
	for (int i = 0; i < blocknum; i++) {
		Normalblock[i].printBlock();
	}
	//画圆
	Mainball.setBallColor(1);
	Mainball.printCircle();
}

// 退出
void Quit() {
	closegraph();
}

// 获取控制命令
int GetCommand() {
	int c = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)	c |= CMD_ZOOMIN;
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)	c |= CMD_ZOOMOUT;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)	c |= CMD_QUIT;
	return c;
}

// 分发控制命令(1参)
inline void DispatchCommand(int _cmd) {
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}

// 分发控制命令(2参)//跳高时/下落时
inline void DispatchCommand(int _cmd, char jump) {
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}

inline void OnUp() {
	int flag = 0;
	if (isdroping == 1) {
		return;
	}
	isjumping = 1;

	for (int i = 13; i > 1; i--) {
		Mainball.printCircle();
		Mainball.moveball(i*i - (i - 1)*(i - 1), 3);

		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向上）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() - Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockBottom() - Mainball.getBally() + Mainball.getBallr(), 4);
					Mainball.printCircle();
					isjumping = 0;
					flag = 1;
					if (Normalblock[i].getBlockType() == 1) {//判断是否是bomp（弹射）类型的方块
						Bomp(Normalblock[i].getBlockAtt(), 4);
					}
				}
			}
		}
		if (flag == 1) {
			break;
		}
		Mainball.printCircle();
		Sleep(25);
		DispatchCommand(GetCommand(), 1);//跳跃时移动位置（1=跳跃中）
	}
	isjumping = 0;
}

inline void OnDown() {
	isdroping = 1;
	char flag = 0;
	int y0 = 400 - Mainball.getBallr();
	for (int i = 1; i <= 30; i++) {
		Mainball.printCircle();
		Mainball.moveball(i*i - (i - 1)*(i - 1), 4);
		if (Mainball.getBally() > y0) {//地面监测
			Mainball.moveball(Mainball.getBally() - y0, 3);
			Mainball.printCircle();
			isdroping = 0;
			break;
		}
		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向下）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockTop() - Mainball.getBally() - Mainball.getBallr(), 4);
					Mainball.printCircle();
					isdroping = 0;
					flag = 1;
					if (Normalblock[i].getBlockType() == 1) {//判断是否是bomp（弹射）类型的方块
						Bomp(Normalblock[i].getBlockAtt(), 3);
					}
				}
			}
		}
		if (flag == 1) {
			break;
		}
		Mainball.printCircle();
		Sleep(25);
		DispatchCommand(GetCommand(), 2);//调用移动位置函数（2=下落中）
	}
}

inline void OnRight() {
	Mainball.printCircle();
	Mainball.moveball(6, 2);
	for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向右）
		if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
			if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
				Mainball.setballxy(Normalblock[i].getBlockLeft() - Mainball.getBallr(), Mainball.getBally());
				if (Normalblock[i].getBlockType() == 1) {//判断是否是bomp（弹射）类型的方块
					Bomp(Normalblock[i].getBlockAtt(), 1);
				}
				break;
			}
		}
	}
	Mainball.printCircle();
}

inline void OnLeft() {
	Mainball.printCircle();
	Mainball.moveball(6, 1);
	for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向左）
		if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
			if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
				Mainball.setballxy(Normalblock[i].getBlockRight() + Mainball.getBallr(), Mainball.getBally());
				if (Normalblock[i].getBlockType() == 1) {//判断是否是bomp（弹射）类型的方块
					Bomp(Normalblock[i].getBlockAtt(), 2);
				}
				break;
			}
		}
	}
	Mainball.printCircle();
}

inline void Bomp(float value, int direction) {
	int y0 = 400 - Mainball.getBallr();
	Mainball.printCircle();
	int flag = 0;
	for (float i = 0; i < 10; i=i+0.5) {
		Mainball.printCircle();
		Mainball.moveball(i*value, direction);

		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向上）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() - Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockBottom() - Mainball.getBally() + Mainball.getBallr(), 4);
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向右）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.setballxy(Normalblock[i].getBlockLeft() - Mainball.getBallr(), Mainball.getBally());
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向左）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.setballxy(Normalblock[i].getBlockRight() + Mainball.getBallr(), Mainball.getBally());
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		if (Mainball.getBally() > y0) {//地面监测
			Mainball.moveball(Mainball.getBally() - y0, 3);
			Mainball.printCircle();
			break;
		}
		for (int i = 0; i < blocknum; i++) {//检测是否碰撞Block（向下）
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockTop() - Mainball.getBally() - Mainball.getBallr(), 4);
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		if (flag == 1) {
			break;
		}
		Mainball.printCircle();
		Sleep(10);
		//被弹射时不能操作小球
	}	
}