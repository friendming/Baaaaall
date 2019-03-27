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

const int g_y0 = 400;//����߶�

					 //��������
void Init();						// ��ʼ��
void Quit();						// �˳�
int  GetCommand();					// ��ȡ��������
void DispatchCommand(int _cmd);		// �ַ���������
void DispatchCommand(int _cmd, char jump);// �ַ���������(Jumping)
inline void OnLeft();
inline void OnUp();
inline void OnRight();
inline void OnLeft();
inline void OnDown();
inline void Bomp(float value, int direction);//�����ȥ ��������Ծ ���ǿ����������Ⱥͷ��� �ڲ����ø��� 1�� 2�� 3�� 4����
////////////////////////////////////////////////////
class Ball {
private:
	int ballx, bally, ballr;
	char ballcolor = 1;//1-BLUE
public:
	Ball(int x, int y, int r) :ballx(x), bally(y), ballr(r) {};//���캯��
	inline void printCircle();//��Բ
	inline void setBallColor(int color);//����Բ����ɫ�����ƣ�
	inline void moveball(int value, char type);//�ƶ� 1�� 2�� 3�� 4��(��ĳ�������ƶ��ľ���)
	inline void setballxy(int x, int y);//ֱ���������λ�� ����������ϵ���ã�
	inline int getBally();//��ȡ��ǰ����y
	inline int getBallx();//��ȡ��ǰ����x
	inline int getBallr();//��ȡ��ǰr
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
			//bally = ballr;// 1.0.1 ���������߽��� ����ͣ�� ��ʱ���޸�
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

class Block {//��ͼ�еĹ̶�����
private:
	int blockx, blocky, blocktype, attack, height, width;//attackʵ������һ����ֵ�����庬���Ӧ��ͬtype�в�ͬ
	char type;//���˵ȵ� ��=0������=1��
public:
	Block();
	Block(int x, int y, int l, int h, int att, int type) :blockx(x), blocky(y), blocktype(type), height(h), width(l), attack(att) {};//���캯��
	void printBlock();//��ӡ��ͼ����
	inline int x_inblock(int x, int r);//��x�Ƿ��ڷ���x��Χ�ڣ�1=��
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

Ball Mainball(200, 380, 20);//���˹�����
Block Normalblock[20];//block������
int blocknum = 2;//block�ж��ٸ�
char isjumping = 0;//�Ƿ���������  1=��
char isdroping = 0;//�Ƿ�������   1=��

int main() {
	Init();//��ʼ������
	int c;
	do {
		c = GetCommand();
		DispatchCommand(c);
		if (isjumping != 1) {
			OnDown();//���䣨������ʱ������ ����ʱ�򶼼���Ƿ��ں����ϻ��ߵ����� ������Ǿ����� ����ǾͲ����䣩
		}
		Sleep(20);//10
	} while (!(c & CMD_QUIT));
	Quit();
}

// ��ʼ��
void Init() {
	// ���û�ͼ��Ļ�ͻ�ͼģʽ
	initgraph(640, 480);
	setwritemode(R2_XORPEN);
	//�����ʼ���������У�
	Normalblock[0] = Block(100, 150, 180, 100, 0, 0);//x,y ��� ��� ��ֵ ����
	Normalblock[1] = Block(300, 300, 50, 50, 2, 1);
	// ��ʾ����˵��
	outtextxy(20, 270, _T("Baaaall Beta 1.0.1"));
	outtextxy(20, 290, _T(" ��   Jump"));
	outtextxy(20, 310, _T(" ��   Left"));
	outtextxy(20, 330, _T(" ��   Right"));
	outtextxy(20, 350, _T("ESC  Exit"));
	//����ͼ
	////����
	line(0, 400, 640, 400);
	////����
	for (int i = 0; i < blocknum; i++) {
		Normalblock[i].printBlock();
	}
	//��Բ
	Mainball.setBallColor(1);
	Mainball.printCircle();
}

// �˳�
void Quit() {
	closegraph();
}

// ��ȡ��������
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

// �ַ���������(1��)
inline void DispatchCommand(int _cmd) {
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
}

// �ַ���������(2��)//����ʱ/����ʱ
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

		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����ϣ�
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() - Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockBottom() - Mainball.getBally() + Mainball.getBallr(), 4);
					Mainball.printCircle();
					isjumping = 0;
					flag = 1;
					if (Normalblock[i].getBlockType() == 1) {//�ж��Ƿ���bomp�����䣩���͵ķ���
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
		DispatchCommand(GetCommand(), 1);//��Ծʱ�ƶ�λ�ã�1=��Ծ�У�
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
		if (Mainball.getBally() > y0) {//������
			Mainball.moveball(Mainball.getBally() - y0, 3);
			Mainball.printCircle();
			isdroping = 0;
			break;
		}
		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����£�
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockTop() - Mainball.getBally() - Mainball.getBallr(), 4);
					Mainball.printCircle();
					isdroping = 0;
					flag = 1;
					if (Normalblock[i].getBlockType() == 1) {//�ж��Ƿ���bomp�����䣩���͵ķ���
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
		DispatchCommand(GetCommand(), 2);//�����ƶ�λ�ú�����2=�����У�
	}
}

inline void OnRight() {
	Mainball.printCircle();
	Mainball.moveball(6, 2);
	for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����ң�
		if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
			if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
				Mainball.setballxy(Normalblock[i].getBlockLeft() - Mainball.getBallr(), Mainball.getBally());
				if (Normalblock[i].getBlockType() == 1) {//�ж��Ƿ���bomp�����䣩���͵ķ���
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
	for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock������
		if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
			if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
				Mainball.setballxy(Normalblock[i].getBlockRight() + Mainball.getBallr(), Mainball.getBally());
				if (Normalblock[i].getBlockType() == 1) {//�ж��Ƿ���bomp�����䣩���͵ķ���
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

		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����ϣ�
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() - Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.moveball(Normalblock[i].getBlockBottom() - Mainball.getBally() + Mainball.getBallr(), 4);
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����ң�
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.setballxy(Normalblock[i].getBlockLeft() - Mainball.getBallr(), Mainball.getBally());
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock������
			if (Normalblock[i].x_inblock(Mainball.getBallx(), Mainball.getBallr()) == 1) {
				if (Mainball.getBally() - Mainball.getBallr() < Normalblock[i].getBlockBottom() && Mainball.getBally() + Mainball.getBallr() > Normalblock[i].getBlockTop()) {
					Mainball.setballxy(Normalblock[i].getBlockRight() + Mainball.getBallr(), Mainball.getBally());
					Mainball.printCircle();
					flag = 1;
				}
			}
		}
		if (Mainball.getBally() > y0) {//������
			Mainball.moveball(Mainball.getBally() - y0, 3);
			Mainball.printCircle();
			break;
		}
		for (int i = 0; i < blocknum; i++) {//����Ƿ���ײBlock�����£�
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
		//������ʱ���ܲ���С��
	}	
}