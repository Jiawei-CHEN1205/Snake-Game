// 程序名称：Gluttonous Snake
// 编译环境：Visual C++ 2017, EasyX_20200520(beta)
// 作    者：陈家苇
// 最后修改：2020-7-24/16：00

#include <graphics.h>
#include <ctime>
#include <conio.h>
#include <vector>
#include <string.h>
#include "mmsystem.h"//导入声音头文件 
#pragma comment(lib,"Winmm.lib")
using namespace std;

// 坐标属性
struct point
{
	int x;
	int y;
};

// 蛇的属性
struct Snake
{
	vector <point> xy;			// 每节坐标
	point next;					// 为下一节预留的位置
	vector <COLORREF> color;	// 每节颜色
	int num;					// 长度
	int position;				// 方向
}snake;

// 食物的属性
struct Food
{
	point fdxy[10];			// 坐标
	int grade;				// 分数
	int num = 1;			// 食物总数
	COLORREF color;		// 食物颜色
}food;

// 障碍物的属性
struct Barrier
{
	point brxy;			// 坐标
	COLORREF color;		// 障碍物颜色
}barrier1, barrier2, barrier3, barrier4;


enum position { up, down, left, right };		// 枚举蛇的方向


//游戏开始界面设定

// 初始化蛇
void initSnake()
{
	point xy;
	xy.x = 20;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(112, 128, 144));	// 石板灰（112，128，144）头部[0]
	xy.x = 10;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(112, 128, 144));	// 石板灰（112，128，144）头部[1]//两块[0][1]一起代表头部
	xy.x = 0;
	xy.y = 0;
	snake.xy.push_back(xy);
	snake.color.push_back(RGB(255, 215, 0));	// 蛇身体的颜色金色（255，215，0）
	snake.num = 3;
	snake.position = right;
}

// 画蛇
void drawSnake()
{
	for (int i = 0; i < snake.num; i++)
	{
		setfillcolor(snake.color[i]);
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}

// 移动蛇
void moveSnake()
{
	// 将预留节设置为未移动前的尾节
	snake.next = snake.xy[snake.num - 1];
	// 将除蛇头以外的节移动到它的前面一节
	for (int i = snake.num - 1; i >= 1; i--)
		snake.xy[i] = snake.xy[i - 1];
	// 根据当前移动方向移动蛇头
	switch (snake.position)
	{
	case right:
		snake.xy[0].x += 10;
		break;
	case left:
		snake.xy[0].x -= 10;
		break;
	case up:
		snake.xy[0].y -= 10;
		break;
	case down:
		snake.xy[0].y += 10;
	}
}

// 按键交互
void keyDown()
{
	char userKey = _getch();
	if (userKey == -32)			// 表明这是方向键
		userKey = -_getch();	// 获取具体方向，并避免与其他字母的 ASCII 冲突
	switch (userKey)
	{
	case 'w':
	case 'W':
	case -72:
		if (snake.position != down)
			snake.position = up;
		break;
	case 's':
	case 'S':
	case -80:
		if (snake.position != up)
			snake.position = down;
		break;
	case 'a':
	case 'A':
	case -75:
		if (snake.position != right)
			snake.position = left;
		break;
	case 'd':
	case 'D':
	case -77:
		if (snake.position != left)
			snake.position = right;
		break;
	}
}

// 初始化食物
void initFood(int num /* 食物编号 */)
{
	food.fdxy[num].x = rand() % 80 * 10;
	food.fdxy[num].y = rand() % 60 * 10;
	for (int i = 0; i < snake.num; i++)
		if (food.fdxy[num].x == snake.xy[i].x && food.fdxy[num].y == snake.xy[i].y)		// 避免食物生成在蛇身上
		{
			food.fdxy[num].x = rand() % 80 * 10;
			food.fdxy[num].y = rand() % 60 * 10;
		}
}
//初始化障碍物（4个）
void initbarrier1()
{
	barrier1.brxy.x = rand() % 70 * 10;
	barrier1.brxy.y = rand() % 50 * 10;
	for (int i = 0; i < snake.num; i++)
		if (barrier1.brxy.x == snake.xy[i].x && barrier1.brxy.y == snake.xy[i].y || barrier1.brxy.x == food.fdxy[i].x && barrier1.brxy.y == food.fdxy[i].y)
			// 避免障碍生成在蛇上或者与食物重合或者障碍物彼此重合
		{
			barrier1.brxy.x = rand() % 70 * 10;
			barrier1.brxy.y = rand() % 50 * 10;
		}
}
void initbarrier2()
{
	barrier2.brxy.x = rand() % 70 * 10;
	barrier2.brxy.y = rand() % 50 * 10;
	for (int i = 0; i < snake.num; i++)
		if (barrier2.brxy.x == snake.xy[i].x && barrier2.brxy.y == snake.xy[i].y || barrier2.brxy.x == food.fdxy[i].x && barrier2.brxy.y == food.fdxy[i].y ||
			barrier2.brxy.x == barrier1.brxy.x && barrier2.brxy.y == barrier1.brxy.y)
			// 避免障碍生成在蛇上或者与食物重合或者障碍物彼此重合
		{
			barrier2.brxy.x = rand() % 70 * 10;
			barrier2.brxy.y = rand() % 50 * 10;
		}
}
void initbarrier3()
{
	barrier3.brxy.x = rand() % 70 * 10;
	barrier3.brxy.y = rand() % 50 * 10;
	for (int i = 0; i < snake.num; i++)
		if (barrier3.brxy.x == snake.xy[i].x && barrier3.brxy.y == snake.xy[i].y || barrier3.brxy.x == food.fdxy[i].x && barrier3.brxy.y == food.fdxy[i].y ||
			barrier3.brxy.x == barrier1.brxy.x && barrier3.brxy.y == barrier1.brxy.y || barrier3.brxy.x == barrier2.brxy.x && barrier3.brxy.y == barrier2.brxy.y)
			// 避免障碍生成在蛇上或者与食物重合或者障碍物彼此重合
		{
			barrier3.brxy.x = rand() % 70 * 10;
			barrier3.brxy.y = rand() % 50 * 10;
		}
}
void initbarrier4()
{
	barrier4.brxy.x = rand() % 70 * 10;
	barrier4.brxy.y = rand() % 50 * 10;
	for (int i = 0; i < snake.num; i++)
		if (barrier4.brxy.x == snake.xy[i].x && barrier4.brxy.y == snake.xy[i].y || barrier4.brxy.x == food.fdxy[i].x && barrier4.brxy.y == food.fdxy[i].y ||
			barrier4.brxy.x == barrier1.brxy.x && barrier4.brxy.y == barrier1.brxy.y || barrier4.brxy.x == barrier2.brxy.x && barrier4.brxy.y == barrier2.brxy.y ||
			barrier4.brxy.x == barrier3.brxy.x && barrier4.brxy.y == barrier3.brxy.y)
			// 避免障碍生成在蛇上或者与食物重合或者障碍物彼此重合
		{
			barrier4.brxy.x = rand() % 70 * 10;
			barrier4.brxy.y = rand() % 50 * 10;
		}
}

// 画食物
void drawFood()
{
	for (int i = 0; i <= food.num - 1; i++)
	{
		setfillcolor(food.color = RGB(255, 215, 0));	// 食物：金色
		fillrectangle(food.fdxy[i].x, food.fdxy[i].y, food.fdxy[i].x + 10, food.fdxy[i].y + 10);
	}
}

//画障碍物
void drawBarrier()
{
	setfillcolor(barrier1.color = RGB(240, 65, 85));	// 障碍物：朱红色
	fillrectangle(barrier1.brxy.x, barrier1.brxy.y, barrier1.brxy.x + 10, barrier1.brxy.y + 10);
	setfillcolor(barrier2.color = RGB(240, 65, 85));
	fillrectangle(barrier2.brxy.x, barrier2.brxy.y, barrier2.brxy.x + 10, barrier2.brxy.y + 10);
	setfillcolor(barrier3.color = RGB(240, 65, 85));
	fillrectangle(barrier3.brxy.x, barrier3.brxy.y, barrier3.brxy.x + 10, barrier3.brxy.y + 10);
	setfillcolor(barrier4.color = RGB(240, 65, 85));
	fillrectangle(barrier4.brxy.x, barrier4.brxy.y, barrier4.brxy.x + 10, barrier4.brxy.y + 10);
}


// 吃食物
void eatFood()
{
	for (int i = 0; i <= food.num - 1; i++)
		if (snake.xy[0].x == food.fdxy[i].x && snake.xy[0].y == food.fdxy[i].y)
		{
			snake.num++;
			snake.xy.push_back(snake.next);					// 新增一个节到预留位置
			snake.color.push_back(food.color);				// 将新增节的颜色设置为当前吃掉食物的颜色//即金色
			food.grade += 10;								//吃食物之后，分数加10分
			initFood(i);
			if (food.num < 10 && food.grade % 100 == 0 && food.grade != 0)
			{
				food.num++;									// 每得 100 分，增加一个食物，但食物总数不超过 10 个
				initFood(food.num - 1);						// 初始化新增加的食物
			}
			break;
		}
}

// 分数显示
void showgrade()
{
	wchar_t grade[20] = L"";
	swprintf_s(grade, L"分数：%d", food.grade);
	outtextxy(650, 50, grade);					//显示分数的位置
	settextcolor(RGB(0, 0, 0));					//显示字符字体设为金色（255，215，0）
	settextstyle(20, 0, L"分数：%d");			//设置分数显示的字符为高度20像素
	setbkmode(TRANSPARENT);						// 设置文字输出模式为透明

}

// 游戏结束
bool gameOver()
{
	// 撞墙，将墙向外扩展一圈（否则蛇无法到达地图边缘）
	if (snake.xy[0].y <= -10 && snake.position == up)			return true;
	if (snake.xy[0].y + 10 >= 610 && snake.position == down)	return true;
	if (snake.xy[0].x <= -10 && snake.position == left)			return true;
	if (snake.xy[0].x + 10 >= 810 && snake.position == right)	return true;
	// 蛇头撞自己
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[0].x <= snake.xy[i].x + 10 && snake.xy[0].x >= snake.xy[i].x && snake.xy[0].y == snake.xy[i].y&&snake.position == left)
			return true;
		if (snake.xy[0].x + 10 >= snake.xy[i].x && snake.xy[0].x + 10 <= snake.xy[i].x + 10 && snake.xy[0].y == snake.xy[i].y && snake.position == right)
			return true;
		if (snake.xy[0].y <= snake.xy[i].y + 10 && snake.xy[0].y >= snake.xy[i].y && snake.xy[0].x == snake.xy[i].x && snake.position == up)
			return true;
		if (snake.xy[0].y + 10 >= snake.xy[i].y && snake.xy[0].y + 10 <= snake.xy[i].y + 10 && snake.xy[0].x == snake.xy[i].x && snake.position == down)
			return true;
	}
	//撞到障碍物1
	if (snake.xy[0].x <= barrier1.brxy.x + 10 && snake.xy[0].x >= barrier1.brxy.x && snake.xy[0].y == barrier1.brxy.y&&snake.position == left)
		return true;
	if (snake.xy[0].x + 10 >= barrier1.brxy.x && snake.xy[0].x + 10 <= barrier1.brxy.x + 10 && snake.xy[0].y == barrier1.brxy.y && snake.position == right)
		return true;
	if (snake.xy[0].y <= barrier1.brxy.y + 10 && snake.xy[0].y >= barrier1.brxy.y && snake.xy[0].x == barrier1.brxy.x && snake.position == up)
		return true;
	if (snake.xy[0].y + 10 >= barrier1.brxy.y && snake.xy[0].y + 10 <= barrier1.brxy.y + 10 && snake.xy[0].x == barrier1.brxy.x && snake.position == down)
		return true;
	//撞到障碍物2
	if (snake.xy[0].x <= barrier2.brxy.x + 10 && snake.xy[0].x >= barrier2.brxy.x && snake.xy[0].y == barrier2.brxy.y&&snake.position == left)
		return true;
	if (snake.xy[0].x + 10 >= barrier2.brxy.x && snake.xy[0].x + 10 <= barrier2.brxy.x + 10 && snake.xy[0].y == barrier2.brxy.y && snake.position == right)
		return true;
	if (snake.xy[0].y <= barrier2.brxy.y + 10 && snake.xy[0].y >= barrier2.brxy.y && snake.xy[0].x == barrier2.brxy.x && snake.position == up)
		return true;
	if (snake.xy[0].y + 10 >= barrier2.brxy.y && snake.xy[0].y + 10 <= barrier2.brxy.y + 10 && snake.xy[0].x == barrier2.brxy.x && snake.position == down)
		return true;
	//撞到障碍物3
	if (snake.xy[0].x <= barrier3.brxy.x + 10 && snake.xy[0].x >= barrier3.brxy.x && snake.xy[0].y == barrier3.brxy.y&&snake.position == left)
		return true;
	if (snake.xy[0].x + 10 >= barrier3.brxy.x && snake.xy[0].x + 10 <= barrier3.brxy.x + 10 && snake.xy[0].y == barrier3.brxy.y && snake.position == right)
		return true;
	if (snake.xy[0].y <= barrier3.brxy.y + 10 && snake.xy[0].y >= barrier3.brxy.y && snake.xy[0].x == barrier3.brxy.x && snake.position == up)
		return true;
	if (snake.xy[0].y + 10 >= barrier3.brxy.y && snake.xy[0].y + 10 <= barrier3.brxy.y + 10 && snake.xy[0].x == barrier3.brxy.x && snake.position == down)
		return true;
	//撞到障碍物4
	if (snake.xy[0].x <= barrier4.brxy.x + 10 && snake.xy[0].x >= barrier4.brxy.x && snake.xy[0].y == barrier4.brxy.y&&snake.position == left)
		return true;
	if (snake.xy[0].x + 10 >= barrier4.brxy.x && snake.xy[0].x + 10 <= barrier4.brxy.x + 10 && snake.xy[0].y == barrier4.brxy.y && snake.position == right)
		return true;
	if (snake.xy[0].y <= barrier4.brxy.y + 10 && snake.xy[0].y >= barrier4.brxy.y && snake.xy[0].x == barrier4.brxy.x && snake.position == up)
		return true;
	if (snake.xy[0].y + 10 >= barrier4.brxy.y && snake.xy[0].y + 10 <= barrier4.brxy.y + 10 && snake.xy[0].x == barrier4.brxy.x && snake.position == down)
		return true;

	return false;
}

int main()								//主函数开始
{
	initgraph(800, 600);				//画布大小800*600
	//setbkcolor(RGB(100, 149, 237));	//整体游戏背景颜色为矢车菊蓝（100，149，237）
	srand((unsigned)time(NULL));
	//打开并播放背景音乐
	MCI_OPEN_PARMS mciOpen;
	mciOpen.lpstrDeviceType = _T("mpegvideo");
	//mciOpen.lpstrElementName = _T("D://background.mp3");//背景音乐的位置
	mciOpen.lpstrElementName = _T("../background.mp3");//背景音乐的位置
	MCIERROR mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	if (mciError)
	{
		TCHAR buf[128] = { 0 };
		mciGetErrorString(mciError, buf, 128);
		printf("%s/n", buf);
	}
	UINT DeviceID = mciOpen.wDeviceID;
	MCI_PLAY_PARMS mciPlay;
	mciError = mciSendCommand(DeviceID, MCI_PLAY, 0, (DWORD)&mciPlay);
	if (mciError)
	{
		printf("send MCI_PLAY command failed/n");
	}

	setfillcolor(RGB(138, 43, 226));	  //欢迎界面的背景颜色：浅紫色
	solidrectangle(0, 0, 800, 600);
	LOGFONT z;
	gettextstyle(&z);                     // 获取当前字体设置
	z.lfHeight = 30;                      // 设置字体高度

	settextstyle(&z);
	setbkmode(TRANSPARENT);// 设置字体为透明
	outtextxy(255, 200, _T("Welcome to the Game"));
	outtextxy(285, 300, _T("Gluttonous Snake"));
	Sleep(1800);
	cleardevice();

	setfillcolor(RGB(128, 138, 135));		 //玩家使用手册界面的背景颜色：浅灰
	solidrectangle(0, 0, 800, 600);
	LOGFONT p;
	gettextstyle(&p);                   // 获取当前字体设置
	p.lfHeight = 20;					// 设置字体高度
	settextstyle(&p);
	setbkmode(TRANSPARENT);				// 设置字体为透明
	outtextxy(320, 90, _T("《玩家须知》"));
	outtextxy(265, 150, _T("(1)这是一个贪吃蛇的小游戏；"));
	outtextxy(240, 200, _T("(2)按上下左右或WSAD可控制蛇的移动；"));
	outtextxy(255, 250, _T("(3)黄色块为食物，红色块为障碍物；"));
	outtextxy(215, 300, _T("(4)蛇撞到四周墙壁、障碍物、蛇自身则游戏结束；"));
	outtextxy(205, 350, _T("(5)每次吃到食物得10分，每得100分将多出现一个食物；"));
	outtextxy(275, 400, _T("(6)食物最多同时出现10个。"));
	outtextxy(235, 470, _T("现在让我们开始游戏吧！请按任意键继续......"));
	outtextxy(20, 580, _T("Copyright @ 2020 ChenJiawei. All right reserved."));
	system("pause");
	cleardevice();


	//游戏操作界面
	setbkcolor(RGB(100, 149, 237));	//整体游戏背景颜色为矢车菊蓝（100，149，237）
	initSnake();
	initFood(0);
	initbarrier1();
	initbarrier2();
	initbarrier3();
	initbarrier4();
	drawSnake();
	while (!gameOver())
	{
		Sleep(100);					//对于游戏蛇的运动速度的设置，数字越大速度越慢
		BeginBatchDraw();			// 开始批量绘图，作用是避免闪烁
		cleardevice();
		if (_kbhit()) keyDown();
		moveSnake();
		eatFood();
		drawFood();
		drawSnake();
		drawBarrier();
		showgrade();
		EndBatchDraw();				// 结束批量绘图
	}
	// 如果因为撞墙死亡则退回到撞墙前的状态以便将蛇完整显示出来
	if (snake.xy[0].y <= -10 && snake.position == up ||
		snake.xy[0].y + 10 >= 610 && snake.position == down ||
		snake.xy[0].x <= -10 && snake.position == left ||
		snake.xy[0].x + 10 >= 810 && snake.position == right)
	{
		for (int i = 0; i <= snake.num - 2; i++)
			snake.xy[i] = snake.xy[i + 1];
		snake.xy[snake.num - 1] = snake.next;
		drawSnake();
	}

	setfillcolor(RGB(0, 153, 0));					//游戏结束界面的背景颜色：绿色
	solidrectangle(0, 0, 800, 600);
	wchar_t overout[50] = L"";
	swprintf_s(overout, L"Game Over!您的最终得分为：%d", food.grade);	//输出"Game over"提示语句和总得分
	outtextxy(260, 250, overout);					//显示结束语和得分的位置
	FlushBatchDraw();								//结束批量绘图
	Sleep(1000);

	_getch();										// 按任意键退出
	closegraph();									//关闭图形窗口
	return 0;
}
