///////////////////////////////////////////////////////////////////////////////////////////
//                                  AyaRenju 五子棋 说明                                  //
///////////////////////////////////////////////////////////////////////////////////////////
// 版本:		v0.9 测试版
// 作者:		ls9512
// QQ:		598914653
// E-mail:	598914653@qq.com
// 说明:		这是本人的第二个EasyX库练习程序
//			五子棋AI算法参考自网络
//			源码供学习交流使用,转载请保留本信息.
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是  系统函数库                                    //
///////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是   扩展支持库                                   //
///////////////////////////////////////////////////////////////////////////////////////////
#include <graphics.h>						//EasyX图形库
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是  全局宏定义                                    //
///////////////////////////////////////////////////////////////////////////////////////////
//以下是游戏状态标志定义
#define GAME_START	0x000001				//游戏开始画面
#define GAME_RUN	0x000002				//游戏进行中
#define GAME_OVER	0x000003				//游戏结束
#define GAME_HELP	0x000004				//游戏帮助
#define GAME_PAUSE	0x000005				//游戏暂停
#define GAME_ABOUT	0x000006				//游戏关于
//以下是游戏模式定义
#define GM_PVP		0x000001				//人人对战
#define GM_PVC		0x000002				//人机对战
#define GM_CVC		0x000003				//(调试)机机对战
//以下是游戏窗体及界面布局参数
#define WIN_WIDTH	640						//窗体宽度
#define WIN_HEIGHT	435						//窗体高度
#define WIN_BLNK	20						//留边宽度
#define WIN_TOP		0						//顶栏宽度
#define WIN_TIPW	180						//消息区宽度
#define CB_W		15						//棋盘宽度个数
#define CB_WIDTH	25						//棋盘格子宽度
#define CB_BLANK	20						//棋盘边框宽度
#define CB_LAST		CB_W*CB_W				//记录步数
#define CB_LASTN	5						//可悔棋步数
//以下是界面元素的颜色定义
#define COLOR_BACK	RGB(247,240,240)		//背景颜色
#define COLOR_BBL	RGB(51,136,204)			//按钮边框线颜色
#define COLOR_BB	RGB(240,240,240)		//按钮背景色
#define COLOR_BBS	RGB(176,208,232)		//按钮选中颜色
#define COLOR_BF	RGB(186,65,176)			//按钮字体颜色
#define COLOR_WC	RGB(245,245,245)		//白色棋子颜色
#define COLOR_WCB	RGB(167,167,167)		//白色棋子边框颜色
#define COLOR_BC	RGB(55,55,55)			//黑色棋子颜色
#define COLOR_BCB	RGB(0,0,0)				//黑色棋子边框颜色
#define COLOR_CB	RGB(250,211,195)		//棋盘颜色
#define COLOR_CBL	RGB(105,83,53)			//棋盘线颜色
//以下是棋子定义
#define C_S			0x000000				//空白
#define C_W			0x000001				//白色棋子
#define C_B			0x000002				//黑色棋子
#define	C_E			0x000003				//和局
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是 全局变量定义                                   //
///////////////////////////////////////////////////////////////////////////////////////////
int isShowFPS=1;							//(调试)是否显示帧数
int isShowStep=0;							//是否显示步数
int isMarkLast=1;							//是否标注最后一个
int isShowMouse=1;							//显示鼠标位置
int gameMode;								//游戏模式
int gamePhase;								//游戏阶段状态
int mouseX,mouseY;							//鼠标所在位置
int isMouseDown;							//鼠标按下标记
int chess[CB_W][CB_W];						//棋盘数组
int chess_step[CB_W][CB_W];					//计步数组
int chess_p1;								//玩家1下子类型
int chess_p2;								//玩家2下子类型
int chess_now;								//当前下子玩家
bool isComputerPlay;						//是否电脑在下子
int isWin;									//是否胜利 -1未胜 否则为胜利方棋子类型
int lastchess_x[CB_LAST];					//悔棋数组x
int lastchess_y[CB_LAST];					//悔棋数组y
int lastchessnum;							//悔棋步数
int pointnum;								//提示数
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是   函数声明                                     //
///////////////////////////////////////////////////////////////////////////////////////////
float GetFPS();								//计算帧数
void Delay(DWORD ms);						//绝对延时,稳定帧数控制
bool IsInRect(int x,int y,RECT r);			//检测坐标是否位于矩形内
POINT GetMousePoint(int x,int y);			//获取鼠标所在棋子坐标(不在棋盘内返回(-1))
void InitChessBoard();						//初始化棋盘
void SetChess(int x,int y);					//下子
int CheckWin(int p,int q);					//判断胜利,失败返回-1,胜利返回胜利棋子类型
void moveChess(int n,int *i,int *j);		//棋盘位置移动,用于搜索和检查
int ChessStatus(int n,int p,int q);			//检查棋型
void AI(int *p,int *q);						//电脑下子
int CountChess();							//统计棋子个数
int ChessValue(int p,int q);				//检查空处的价值
void AddUndoChess(int x,int y);				//添加走棋记录(用于保存棋局和悔棋)
void UndoChess();							//执行悔棋
POINT GetLastChessPoint();					//获取最后一次下子的坐标
void Manager();								//逻辑控制
void DrawFPS();								//绘制帧数
void DrawStart();							//绘制游戏开始画面
void DrawBack();							//绘制背景
void DrawChessBoard();						//绘制棋盘
void DrawChess(int x,int y);				//绘制棋子
void DrawChessBoard();						//绘制提示板
void DrawRun();								//绘制游戏画面
void DrawHelp();							//绘制帮助画面
void DrawPause();							//绘制暂停界面
void DrawAbout();							//绘制关于界面
void Graphics();							//绘图控制
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是 辅助功能函数                                   //
///////////////////////////////////////////////////////////////////////////////////////////
//计算画面FPS(每秒帧数)
float GetFPS()
{
#define FPS_COUNT 8
	static int i = 0;
	static DWORD oldTime = GetTickCount();
	static float fps;
	if (i > FPS_COUNT)
	{
		i = 0;
		int newTime = GetTickCount();
		int elapsedTime = newTime - oldTime;
		fps = FPS_COUNT / (elapsedTime / 1000.0f);
		oldTime = newTime;
	}
	i++;
	return fps;
}
//绝对延时
void Delay(DWORD ms)
{
	static DWORD oldTime = GetTickCount();
	while(GetTickCount() - oldTime < ms)
		Sleep(1);
	oldTime = GetTickCount();
}
//是否在矩形内
bool IsInRect(int x,int y,RECT r)
{
	if((x>=r.left && x<=r.right) && (y>=r.top && y<=r.bottom)) return true;
	else return false;
}
//获取鼠标所在棋子坐标(不在棋盘内返回(-1))
POINT GetMousePoint(int x,int y)
{
	int xx,yy;
	xx=WIN_BLNK+CB_BLANK - CB_WIDTH/2;
	yy=WIN_BLNK+WIN_TOP+CB_BLANK - CB_WIDTH/2;
	POINT p;
	p.x=(x-xx)/CB_WIDTH;
	p.y=(y-yy)/CB_WIDTH;
	//溢出处理
	p.x=p.x>=CB_W?-1:p.x;
	p.x=p.x<0?-1:p.x;
	p.y=p.y>=CB_W?-1:p.y;
	p.y=p.y<0?-1:p.y;
	return p;
}
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是  具体逻辑实现                                  //
///////////////////////////////////////////////////////////////////////////////////////////
//初始化棋盘
void InitChessBoard()
{
	//全置空
	for(int i=0;i<CB_W;i++){
		for(int j=0;j<CB_W;j++){
			chess[i][j]=C_S;
			chess_step[i][j]=C_S;
		}
	}
	//置初始数据
	int i=rand()%11;
	if(i<5){
		chess_p1 = C_W;
		chess_p2 = C_B;
		chess_now = chess_p1;
	}else{
		chess_p1 = C_B;
		chess_p2 = C_W;
		chess_now = chess_p1;
	}
	isWin=-1;
	//置悔棋数组
	for(int i=0;i<CB_LAST;i++){
		lastchess_x[i]=-1;
		lastchess_y[i]=-1;
	}
	lastchessnum=0;
	pointnum=3;
}
//棋盘位置移动,用于搜索和检查
void moveChess(int n,int *i,int *j)        /* 在n方向上对坐标 i j 移位 n为1-8方向 从右顺时针开始数 */
{
	switch(n){
	case 1: *i+=1; break;
	case 2: *i+=1; *j+=1; break;
	case 3: *j+=1; break;
	case 4: *i-=1; *j+=1; break;
	case 5: *i-=1; break;
	case 6: *i-=1; *j-=1; break;
	case 7: *j-=1; break;
	case 8: *i+=1; *j-=1; break;
	}
}
//判断胜利
int CheckWin(int p,int q)
{
	//和局
	if(CountChess()==CB_W*CB_W){
		return C_E;
	}
	int k,n=1,m,P,Q;      /* k储存判断点p q的状态COM或MAN。P Q储存判断点坐标。n为判断方向。m为个数。 */
	P=p; Q=q;
	k=chess[q][p];
	while(n!=5){
		m=0; 
		while(k==chess[q][p]){
			m++;
			if(m==5) return k; 
			moveChess(n,&p,&q); if(p<0||p>CB_W-1||q<0||q>CB_W-1) break;
		}
		n+=4; m-=1; p=P; q=Q;   /* 转向判断 */
		while(k==chess[q][p]){
			m++;
			if(m==5) return k; 
			moveChess(n,&p,&q); if(p<0||p>CB_W-1||q<0||q>CB_W-1) break;
		}
		n-=3; p=P; q=Q;         /* 不成功则判断下一组方向 */
	}
	return -1;
}
//检查棋型
int ChessStatus(int n,int p,int q)            /* 返回空点p q在n方向上的棋型号 n为1-8方向 从右顺时针开始数 */ 
{
	int k,m=0;  /* 棋型号注解:  己活000-003 己冲010-013 对活100-103 对冲110-113 己空活020-023 己空冲030-033 对空活120-123 对空冲130-133 空-1 边界冲-2 边界空冲-3*/
	moveChess(n,&p,&q);
	if(p<0||p>14||q<0||q>14) k=-2;                                      /* 边界冲棋型 */
	switch(chess[q][p]){
		case C_B:{
					m++; moveChess(n,&p,&q);
					if(p<0||p>14||q<0||q>14) { k=m+9; return k; }
					while(chess[q][p]==C_B) { m++; moveChess(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+9; return k; } }
					if(chess[q][p]==C_S) k=m-1;                         /* 己方活棋型 */
 					else k=m+9;                                         /* 己方冲棋型 */
				 }break;
		case C_W:{
					m++; moveChess(n,&p,&q);
					if(p<0||p>14||q<0||q>14) { k=m+109; return k; }
					while(chess[q][p]==C_W) { m++; moveChess(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+109; return k; } }
					if(chess[q][p]==C_S) k=m+99;                         /* 对方活棋型 */
					else k=m+109;                                        /* 对方冲棋型 */
				 }break;
		case C_S:{
					moveChess(n,&p,&q);
					if(p<0||p>14||q<0||q>14) { k=-3; return k; }         /* 边界空冲棋型 */
					switch(chess[q][p]){
					case C_B:{
								m++; moveChess(n,&p,&q);
								if(p<0||p>14||q<0||q>14) { k=m+29; return k; }
								while(chess[q][p]==C_B) { m++; moveChess(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+29; return k; } }
								if(chess[q][p]==C_S) k=m+19;                      /* 己方空活棋型 */
								else k=m+29;                                      /* 己方空冲棋型 */
							 }break;
					case C_W:{
								m++; moveChess(n,&p,&q);
								if(p<0||p>14||q<0||q>14) { k=m+129; return k; }
								while(chess[q][p]==C_W) { m++; moveChess(n,&p,&q); if(p<0||p>14||q<0||q>14) { k=m+129; return k; } }		   
								if(chess[q][p]==C_S) k=m+119;                     /* 对方空活棋型 */
								else k=m+129;                                     /* 对方空冲棋型 */ 
							 }break;
					case C_S: k=-1; break;                                         /* 空棋型 */
					}
				}break;
	}
	return k;
}
//统计棋子个数
int CountChess()
{
	int n=0;
	for(int j=0;j<CB_W;j++){
		for(int i=0;i<CB_W;i++){
			if(chess[i][j]!=C_S) n++;
		}
	}
	return n;
}
//电脑下子,返回P Q下子坐标
void AI(int *p,int *q)
{
	//初始随机走
	if(CountChess()==0){
		int d=CB_W/2;
		*p=rand()%d+CB_W/2-d/2;
		*q=rand()%d+CB_W/2-d/2;
		return;
	}
	//棋盘满.和局
	if(CountChess()==CB_W*CB_W){
		return;
	}
	int i,j,k,max=0,I,J;        /* I J为下点坐标 */
	for(j=0;j<CB_W;j++)
		for(i=0;i<CB_W;i++)
			if(chess[j][i]==C_S){      /* 历遍棋盘，遇到空点则计算价值，取最大价值点下子。 */
				k=ChessValue(i,j);
				if(k>=max) { I=i; J=j; max=k; }
			}
	*p=I; *q=J;
}
//计算空点p q的价值 以k返回 
int ChessValue(int p,int q) 
{
	int n=1,k=0,k1,k2,K1,K2,X1,Y1,Z1,X2,Y2,Z2,temp;  
	int a[2][4][4]={40,400,3000,10000,6,10,600,10000,20,120,200,0,6,10,500,0,30,300,2500,5000,2,8,300,8000,26,160,0,0,4,20,300,0};
	       /* 数组a中储存己方和对方共32种棋型的值  己方0对方1    活0冲1空活2空冲3    子数0-3（0表示1个子，3表示4个子） */
	while(n!=5){
		k1=ChessStatus(n,p,q); n+=4;			/* k1,k2为2个反方向的棋型编号 */
		k2=ChessStatus(n,p,q); n-=3;
		if(k1>k2) { temp=k1; k1=k2; k2=temp; }  /* 使编号小的为k1,大的为k2 */
		K1=k1; K2=k2;       /* K1 K2储存k1 k2的编号 */
		Z1=k1%10; Z2=k2%10; k1/=10; k2/=10; Y1=k1%10; Y2=k2%10; k1/=10; k2/=10; X1=k1%10; X2=k2%10;
		    /* X Y Z分别表示 己方0对方1    活0冲1空活2空冲3    子数0-3（0表示1个子，3表示4个子） */
		if(K1==-1) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]+5; continue;  };    /* 空棋型and其他 */
		if(K1==-2) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/2; continue; };       /* 边界冲棋型and其他 */ 
		if(K1==-3) { if(K2<0) { k+=0; continue; } else k+=a[X2][Y2][Z2]/3; continue; };    /* 边界空冲棋型and其他 */ 
		if(((K1>-1&&K1<4)&&((K2>-1&&K2<4)||(K2>9&&K2<14)))||((K1>99&&K1<104)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){
			/* 己活己活 己活己冲 对活对活 对活对冲 的棋型赋值*/
			if(Z1+Z2>=2) { k+=a[X2][Y2][3]; continue; }
			else { k+=a[X2][Y2][Z1+Z2+1]; continue; }
		}
		if(((K1>9&&K1<14)&&(K2>9&&K2<14))||((K1>109&&K1<114)&&(K2>109&&K2<114))){
			/* 己冲己冲 对冲对冲 的棋型赋值*/
			if(Z1+Z2>=2) { k+=10000; continue; }
			else { k+=0; continue; }
		}
		if(((K1>-1&&K1<4)&&((K2>99&&K2<104)||(K2>109&&K2<114)))||((K1>9&&K1<14)&&((K2>99&&K2<104)||(K2>109&&K2<114)))){
			/* 己活对活 己活对冲 己冲对活 己冲对冲 的棋型赋值*/
			if(Z1==3||Z2==3) { k+=10000; continue; }
			else { k+=a[X2][Y2][Z2]+a[X1][Y1][Z1]/4; continue; } 
		}
		else  { k+=a[X1][Y1][Z1]+a[X2][Y2][Z2]; continue;  }    /* 其他棋型的赋值 */
	}
	return k;
}
//下子
void SetChess(int x,int y)
{
	//下子
	chess[y][x]=chess_now;
	chess_step[y][x]=CountChess();
	//添加悔棋记录
	AddUndoChess(x,y);
	//切换玩家
	if(chess_now==chess_p1)chess_now=chess_p2;
	else chess_now=chess_p1;
	//判断胜利
	isWin=CheckWin(x,y);
}
//添加走棋记录
void AddUndoChess(int x,int y)
{
	int i=0;
	while(lastchess_x[i]!=-1 && lastchess_y[i]!=-1 && i<CB_LAST) i++;
	//数组满,向前移动记录
	if(i==CB_LAST){
		for(int j=0;j<CB_LAST;j++){
			lastchess_x[j] = lastchess_x[j+1];
			lastchess_y[j] = lastchess_y[j+1];
		}
		i=CB_LAST-1;
	}
	lastchess_x[i]=x;
	lastchess_y[i]=y;
}
//悔棋
void UndoChess()
{
	int i=CB_LAST-1;
	while(lastchess_x[i]==-1 && lastchess_y[i]==-1 && i>-1) i--;
	if(i<0) return;
	if(gameMode==GM_PVC){
		//置空
		chess[lastchess_y[i]][lastchess_x[i]]=C_S;
		lastchess_x[i]=-1;
		lastchess_y[i]=-1;
		chess[lastchess_y[i-1]][lastchess_x[i-1]]=C_S;
		lastchess_x[i-1]=-1;
		lastchess_y[i-1]=-1;
		//置悔棋步数
		lastchessnum--;
		lastchessnum=lastchessnum<0?0:lastchessnum;
	}
	else{
		//置空
		chess[lastchess_y[i]][lastchess_x[i]]=C_S;
		lastchess_x[i]=-1;
		lastchess_y[i]=-1;
		//置悔棋步数
		lastchessnum--;
		lastchessnum=lastchessnum<0?0:lastchessnum;
	}
}
//获取最后一次下子
POINT GetLastChessPoint()
{
	int i=CB_LAST-1;
	while(lastchess_x[i]==-1 && lastchess_y[i]==-1 && i>-1) i--;
	if(i<0) i=0;
	POINT p;
	p.x=lastchess_x[i];
	p.y=lastchess_y[i];
	return p;
}
//逻辑
void Manager()
{
	if(gamePhase==GAME_RUN){
		//判断下子
		POINT p=GetMousePoint(mouseX,mouseY);
		if(p.x!=-1 && p.y!=-1 && chess[p.y][p.x]==C_S && isMouseDown && isWin==-1){
			SetChess(p.x,p.y);
			//置悔棋步数
			lastchessnum++;
			lastchessnum=lastchessnum>CB_LASTN?CB_LASTN:lastchessnum;
			isMouseDown=false;
		}
		//当前是PVC模式且为电脑下子时
		if(gameMode==GM_PVC && chess_now==chess_p2 && isWin==-1 && isComputerPlay==false){
			int x,y;
			isComputerPlay=true;
			AI(&x,&y);
			SetChess(x,y);
			isComputerPlay=false;
		}
		//(调试)CVC
		if(gameMode==GM_CVC && isWin==-1 && isComputerPlay==false){
			int x,y;
			isComputerPlay=true;
			AI(&x,&y);
			SetChess(x,y);
			//置悔棋步数
			lastchessnum++;
			lastchessnum=lastchessnum>CB_LASTN?CB_LASTN:lastchessnum;
			isComputerPlay=false;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是  具体绘图实现                                  //
///////////////////////////////////////////////////////////////////////////////////////////
//绘制帧数
void DrawFPS()
{
	setcolor(RED);
	settextstyle(14, 0, _T("Verdana"));
	char s[5];
	sprintf_s(s,"%.1f", GetFPS());
	outtextxy(0, 0, s);
}
//绘制背景
void DrawBack()
{
	//绘制背景图
	setcolor(COLOR_BACK);
	setfillcolor(RGB(255,204,255));
	fillrectangle(0,0,WIN_WIDTH,WIN_HEIGHT);
	setcolor(COLOR_BACK);
	setfillcolor(COLOR_BACK);
	fillcircle(280,-300,700);
}
//绘制开始画面
void DrawStart()
{
	DrawBack();
	//零散装饰图形
	setcolor(RGB(153,187,238));
	setfillcolor(RGB(153,187,238));
	fillcircle(WIN_WIDTH/2-230,75,55);
	setcolor(COLOR_BACK);
	setfillcolor(COLOR_BACK);
	fillcircle(WIN_WIDTH/2-220,75,48);
	settextstyle(52, 30, _T("Impact"));
	//标题
	setcolor(RGB(255,17,102));
	outtextxy(WIN_WIDTH/2-253, 53, "AyaRenju 五子棋");
	setcolor(RGB(255,173,241));
	outtextxy(WIN_WIDTH/2-250, 50, "AyaRenju 五子棋");
	//版本
	settextstyle(22, 10, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(WIN_WIDTH/2+130, 133,"v0.9 By:ls9512");
	//人机对战按钮
	RECT r;
	r.left=(WIN_WIDTH-125)/2 - 10;
	r.top=190;
	r.right=r.left+125;
	r.bottom=r.top+30;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//开始游戏按键响应
		if(isMouseDown)
		{
			gamePhase = GAME_RUN;
			gameMode=GM_PVC;
			InitChessBoard();
			isMouseDown=false;
		}
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 10, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+3," Play  PvC");
	//人人对战按钮
	r.left=(WIN_WIDTH-125)/2 + 10;
	r.top=240;
	r.right=r.left+125;
	r.bottom=r.top+30;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//开始游戏按键响应
		if(isMouseDown)
		{
			gamePhase = GAME_RUN;
			gameMode=GM_PVP;
			InitChessBoard();
			isMouseDown=false;
		}
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 10, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+3," Play  PvP");
	//帮助游戏按钮
	r.left=(WIN_WIDTH-125)/2 - 10;
	r.top=290;
	r.right=r.left+125;
	r.bottom=r.top+30;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r))
	{
		setfillcolor(COLOR_BBS);
		//帮助按键响应
		if(isMouseDown) gamePhase=GAME_HELP;
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 10, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+3," Help  Info");
	//退出游戏按钮
	r.left=(WIN_WIDTH-125)/2 + 10;
	r.top=340;
	r.right=r.left+125;
	r.bottom=r.top+30;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r))
	{
		setfillcolor(COLOR_BBS);
		//退出按键响应
		if(isMouseDown) exit(0);
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 10, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+3," Exit Game");
	//关于游戏按钮
	r.left=WIN_WIDTH-135;
	r.top=WIN_HEIGHT-30;
	r.right=r.left+120;
	r.bottom=r.top+20;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r))
	{
		setfillcolor(RGB(153,221,204));
		//关于按键响应
		if(isMouseDown) gamePhase = GAME_ABOUT;
	}
	else setfillcolor(RGB(255,255,153));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(16, 9, _T("Verdana"));
	setcolor(RGB(153,68,153));
	outtextxy(r.left+5, r.top+2,"About Game");
	//说明
	settextstyle(16, 5, _T("Verdana"));
	setcolor(RGB(238,0,119));
	outtextxy(WIN_WIDTH/2-130, WIN_HEIGHT-20,"Programing By VC++ And EasyX     Date:2013.10.22");
}
//绘制棋盘
void DrawChessBoard()
{
	int x,y,xw,yw,i;
	//绘制棋盘
	setcolor(COLOR_CBL);
	setfillcolor(COLOR_CB);
	x=WIN_BLNK;
	y=WIN_BLNK+WIN_TOP;
	xw=x+CB_BLANK*2+(CB_W-1)*CB_WIDTH;
	yw=y+CB_BLANK*2+(CB_W-1)*CB_WIDTH;
	fillrectangle(x,y,xw,yw);
	fillrectangle(x+3,y+3,xw-3,yw-3);
	//胜利
	if(isWin!=-1){
		switch(isWin){
		case C_W:setcolor(COLOR_WC);break;
		case C_B:setcolor(COLOR_BC);break;
		case C_E:setcolor(RGB(128,128,128));break;
		}
		rectangle(x+1,y+1,xw-1,yw-1);
		rectangle(x+2,y+2,xw-2,yw-2);
	}
	//绘制线格
	setcolor(COLOR_CBL);
	xw=(CB_W-1)*CB_WIDTH;
	yw=(CB_W-1)*CB_WIDTH;
	for(i=0;i<CB_W;i++){
		x=WIN_BLNK+CB_BLANK+CB_WIDTH*i;
		y=WIN_BLNK+CB_BLANK+WIN_TOP;
		line(x,y,x,y+xw);
		x=WIN_BLNK+CB_BLANK;
		y=WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*i;
		line(x,y,x+yw,y);
	}
	//绘制点
	setfillcolor(COLOR_CBL);
	fillcircle(WIN_BLNK+CB_BLANK+CB_WIDTH*3,WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*3,3);
	fillcircle(WIN_BLNK+CB_BLANK+CB_WIDTH*11,WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*3,3);
	fillcircle(WIN_BLNK+CB_BLANK+CB_WIDTH*3,WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*11,3);
	fillcircle(WIN_BLNK+CB_BLANK+CB_WIDTH*11,WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*11,3);
	fillcircle(WIN_BLNK+CB_BLANK+CB_WIDTH*7,WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*7,3);
}
//绘制提示板
void DrawTipBoard()
{
	//底板
	int x,y,xw,yw,tx,ty;
	x=WIN_BLNK*2+CB_BLANK*2+(CB_W-1)*CB_WIDTH+5;
	y=WIN_BLNK+WIN_TOP;
	tx=x;
	ty=y;
	xw=x+WIN_TIPW;
	yw=y+CB_BLANK*2+(CB_W-1)*CB_WIDTH;
	setcolor(RGB(136,17,17));
	setfillcolor(RGB(255,255,204));
	fillrectangle(x,y,xw,yw);
	//悔棋按钮
	RECT r;
	r.left=x+10;
	r.top=y+295;
	r.right=r.left+75;
	r.bottom=r.top+20;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r) && lastchessnum>0 && isWin==-1)
	{
		setfillcolor(RGB(153,221,204));
		//悔棋响应
		if(isMouseDown){
			UndoChess();
			isMouseDown=false;
		}
	}
	else setfillcolor(RGB(255,255,153));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(16, 9, _T("Verdana"));
	setcolor(RGB(153,68,153));
	outtextxy(r.left+5, r.top+2,"Undo( )");
	outtextxy(r.left+56, r.top+2,toascii(lastchessnum+48));
	//暂停菜单按钮
	r.left=x+10;
	r.top=y+10;
	r.right=r.left+WIN_TIPW-20;
	r.bottom=r.top+20;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r) && isWin==-1)
	{
		setfillcolor(RGB(153,221,204));
		//暂停响应
		if(isMouseDown) gamePhase=GAME_PAUSE;
	}
	else setfillcolor(RGB(255,255,153));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(16, 11, _T("Verdana"));
	setcolor(RGB(153,68,153));
	outtextxy(r.left+4, r.top+2,"Pause & Menu");
	//提示按钮
	r.left=x+95;
	r.top=y+295;
	r.right=r.left+75;
	r.bottom=r.top+20;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r) && pointnum>0 && isWin==-1 && gameMode!=GM_PVP)
	{
		setfillcolor(RGB(153,221,204));
		//提示响应
		if(isMouseDown){
			//UndoChess();
			int x,y;
			AI(&x,&y);
			SetChess(x,y);
			pointnum--;
			isMouseDown=false;
		}
	}
	else setfillcolor(RGB(255,255,153));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(16, 9, _T("Verdana"));
	setcolor(RGB(153,68,153));
	outtextxy(r.left+5, r.top+2,"Point( )");
	outtextxy(r.left+55, r.top+2,toascii(pointnum+48));
	//提示信息
	setcolor(RGB(255,180,189));
	rectangle(x+5,y+90,xw-5,y+230);
	setcolor(RGB(255,17,119));
	setfillcolor(RGB(255,187,255));
	//fillrectangle(x+10,y+40,xw-10,y+68);
	char *s;
	COLORREF c;
	if(isWin != -1){
		switch(isWin){
		case C_W:
			c=RED;
			s="White Win!";
			break;
		case C_B:
			c=RED;
			s="Black Win!";
			break;
		case C_E:
			c=RGB(128,128,128);
			s="Deuce!";
			break;
		}
	}
	else{
		switch(chess_now){
		case C_W:
			c=COLOR_BC;
			s="Now White...";
			break;
		case C_B:
			c=COLOR_BC;
			s="Now Black...";
			break;
		}
	}
	settextstyle(16,9,_T("Verdana"));
	setcolor(RGB(102,17,68));
	if(false){
		outtextxy(451,66,s);
		outtextxy(451,64,s);
		outtextxy(449,64,s);
		outtextxy(449,66,s);
	}
	setcolor(c);
	outtextxy(tx+10,ty+40,s);
	//步数
	outtextxy(tx+10,ty+60,"Step:");
	char sn[5];
	sprintf_s(sn,"%d",CountChess());
	outtextxy(tx+60,ty+60,sn);
	//玩家1信息
	switch(chess_p1){
	case C_W:setcolor(COLOR_WCB);setfillcolor(COLOR_WC);break;
	case C_B:setcolor(COLOR_BCB);setfillcolor(COLOR_BC);break;
	}
	fillcircle(tx+50,ty+120,15);
	setcolor(COLOR_BC);
	outtextxy(tx+75,ty+112,"Player1");
	//玩家2信息
	switch(chess_p2){
	case C_W:setcolor(COLOR_WCB);setfillcolor(COLOR_WC);break;
	case C_B:setcolor(COLOR_BCB);setfillcolor(COLOR_BC);break;
	}
	fillcircle(tx+50,ty+200,15);
	setcolor(COLOR_BC);
	if(gameMode==GM_PVP) outtextxy(tx+75,ty+192,"Player2");
	else outtextxy(tx+75,ty+192,"Computer");
	settextstyle(24,14,_T("Verdana"));
	outtextxy(tx+75,ty+152,"VS");
	//提示箭头
	if(isWin==-1){
		setcolor(COLOR_CBL);setfillcolor(COLOR_CBL);
		if(chess_now==chess_p1){
			fillrectangle(tx+20,ty+118,tx+24,ty+122);
		}else{
			fillrectangle(tx+20,ty+198,tx+24,ty+202);
		}	
	}
	//胜利提示框
	else{
		setcolor(RED);
		if(isWin==chess_p1){
			rectangle(tx+10,ty+100,tx+WIN_TIPW-10,ty+140);
		}else{
			rectangle(tx+10,ty+180,tx+WIN_TIPW-10,ty+220);
		}	
	}
	//绘制是否显示步数选择框
	settextstyle(16,9,_T("Verdana"));
	setcolor(COLOR_BC);
	outtextxy(tx+35,yw-45,"Mark Last");
	r.left=tx+13;
	r.top=yw-43;
	r.right=r.left+12;
	r.bottom=r.top+12;
	setcolor(RGB(85,0,85));
	if(IsInRect(mouseX,mouseY,r)){
		setfillcolor(RGB(221,170,238));
		if(isMouseDown){
			isMarkLast=!isMarkLast;
			isMouseDown=false;
		}
	}
	else setfillcolor(RGB(221,255,221));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	if(isMarkLast){
		line(r.left+2,r.top+4,r.left+5,r.top+7);
		line(r.left+2,r.top+5,r.left+5,r.top+8);
		line(r.left+5,r.top+7,r.left+13,r.top-3);
		line(r.left+5,r.top+8,r.left+13,r.top-2);
	}
	r.left=tx+13;
	r.top=yw-23;
	r.right=r.left+12;
	r.bottom=r.top+12;
	setcolor(RGB(85,0,85));
	if(IsInRect(mouseX,mouseY,r)){
		setfillcolor(RGB(221,170,238));
		if(isMouseDown){
			isShowStep=!isShowStep;
			isMouseDown=false;
		}
	}
	else setfillcolor(RGB(221,255,221));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	if(isShowStep){
		line(r.left+2,r.top+4,r.left+5,r.top+7);
		line(r.left+2,r.top+5,r.left+5,r.top+8);
		line(r.left+5,r.top+7,r.left+13,r.top-3);
		line(r.left+5,r.top+8,r.left+13,r.top-2);
	}
	setcolor(COLOR_BC);
	outtextxy(tx+35,yw-25,"Show Steps");
	r.left=tx+13;
	r.top=yw-63;
	r.right=r.left+12;
	r.bottom=r.top+12;
	setcolor(RGB(85,0,85));
	if(IsInRect(mouseX,mouseY,r)){
		setfillcolor(RGB(221,170,238));
		if(isMouseDown){
			isShowMouse=!isShowMouse;
			isMouseDown=false;
		}
	}
	else setfillcolor(RGB(221,255,221));
	fillrectangle(r.left,r.top,r.right,r.bottom);
	if(isShowMouse){
		line(r.left+2,r.top+4,r.left+5,r.top+7);
		line(r.left+2,r.top+5,r.left+5,r.top+8);
		line(r.left+5,r.top+7,r.left+13,r.top-3);
		line(r.left+5,r.top+8,r.left+13,r.top-2);
	}
	setcolor(COLOR_BC);
	outtextxy(tx+35,yw-65,"Show Mouse");
	//重新开始按钮
	if(isWin!=-1){
		r.left=tx+20;
		r.top=yw-153;
		r.right=r.left+WIN_TIPW-40;
		r.bottom=r.top+50;
		setcolor(RGB(85,0,85));
		if(IsInRect(mouseX,mouseY,r)){
			setfillcolor(RGB(221,190,238));
			if(isMouseDown){
				InitChessBoard();
				isMouseDown=false;
			}
		}
		else setfillcolor(RGB(231,255,231));
		fillrectangle(r.left,r.top,r.right,r.bottom);
		setcolor(RED);
		settextstyle(38,13,_T("Verdana"));
		outtextxy(r.left+5,r.top+2,"Try Again!");
	}
}
//绘制棋子
void DrawChess(int x,int y)
{
	//非空
	if(chess[y][x] != C_S){
		switch(chess[y][x]){
		case C_W:setcolor(COLOR_WCB);setfillcolor(COLOR_WC);break;
		case C_B:setcolor(COLOR_BCB);setfillcolor(COLOR_BC);break;
		}
		int xx=WIN_BLNK+CB_BLANK+CB_WIDTH*x;
		int yy=WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*y;
		fillcircle(xx,yy,CB_WIDTH/2);
		if(isShowStep){
			settextstyle(11,5,_T("宋体"));
			switch(chess[y][x]){
			case C_W:setcolor(COLOR_BC);break;
			case C_B:setcolor(COLOR_WC);break;
			}
			char s[5];
			sprintf_s(s,"%d",chess_step[y][x]);
			if(chess_step[y][x]>=100)outtextxy(xx-7,yy-5,s);
			else if(chess_step[y][x]>=10)outtextxy(xx-5,yy-5,s);
			else outtextxy(xx-2,yy-5,s);
		}
	}
}
//绘制游戏画面
void DrawRun()
{
	DrawBack();
	int i;
	//绘制棋盘
	DrawChessBoard();
	//绘制提示板
	DrawTipBoard();
	i=0;
	while(lastchess_x[i]!=-1 && lastchess_y[i]!=-1 && i<CB_W*CB_W){
		DrawChess(lastchess_x[i],lastchess_y[i]);
		i++;
	}
	POINT p=GetMousePoint(mouseX,mouseY);
	//画出鼠标位置
	if(isShowMouse){
		if(p.x!=-1 && p.y!=-1 && chess[p.y][p.x]==C_S && isWin==-1){
			int xx=WIN_BLNK+CB_BLANK+CB_WIDTH*p.x;
			int yy=WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*p.y;
			setcolor(RGB(221,68,119));
			rectangle(xx-6,yy-6,xx+6,yy+6);
			rectangle(xx-5,yy-5,xx+5,yy+5);
		}
	}
	p=GetLastChessPoint();
	//上一次下子提示
	if(isMarkLast){
		if(p.x!=-1 && p.y!=-1){
			int xx=WIN_BLNK+CB_BLANK+CB_WIDTH*p.x;
			int yy=WIN_BLNK+CB_BLANK+WIN_TOP+CB_WIDTH*p.y;
			setcolor(RGB(0,68,204));
			rectangle(xx-CB_WIDTH/2,yy-CB_WIDTH/2,xx+CB_WIDTH/2,yy+CB_WIDTH/2);
			rectangle(xx-CB_WIDTH/2-1,yy-CB_WIDTH/2-1,xx+CB_WIDTH/2+1,yy+CB_WIDTH/2+1);
		}
	}
}
//绘制帮助画面
void DrawHelp()
{
	settextstyle(52, 30, _T("Impact"));
	//标题
	setcolor(RGB(255,17,152));
	outtextxy(253, 33, "Help");
	setcolor(RGB(215,193,238));
	outtextxy(250, 30, "Help");
	//底板
	setcolor(RGB(255,17,204));
	setfillcolor(RGB(255,221,255));
	fillrectangle(60,100,570,350);
	//版本
	settextstyle(16, 8, _T("宋体"));
	setcolor(RGB(205,86,52));
	outtextxy(70, 110,"五子棋是一款经典的黑白棋类游戏。");
	outtextxy(70, 145,"【简单玩法说明】");
	outtextxy(80, 170,"1.PvC/PvP,玩家和电脑或者其他玩家轮流下子.");
	outtextxy(80, 195,"2.有5个或更多同色子位于同一直线(横纵斜)即为胜利.");
	outtextxy(80, 220,"3.游戏过程中可以悔棋5次,PvP模式下两位玩家共享5次悔棋机会.");
	outtextxy(80, 245,"4.开局随机给玩家分配黑子或白子.");
	outtextxy(80, 270,"5.PvC模式每一局有3次提示机会,PvP模式下不可用.");
	outtextxy(80, 295,"6.计步,最后一步,鼠标位置等是否需要显示可在右下角勾选.");
	outtextxy(80, 320,"7.本游戏为入门级小游戏,不考虑五子棋的禁手等其他规则.");
	//返回
	RECT r;
	r.left=(WIN_WIDTH-120)/2;
	r.top=370;
	r.right=r.left+120;
	r.bottom=r.top+35;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//返回按键响应
		if(isMouseDown) gamePhase=GAME_START;
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 11, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+5,"Back Title");
}
//绘制暂停界面
void DrawPause()
{
	DrawBack();
	//零散装饰图形
	setcolor(RGB(153,187,238));
	setfillcolor(RGB(153,187,238));
	fillcircle(WIN_WIDTH/2-230,75,55);
	setcolor(COLOR_BACK);
	setfillcolor(COLOR_BACK);
	fillcircle(WIN_WIDTH/2-220,75,48);
	settextstyle(52, 30, _T("Impact"));
	//标题
	setcolor(RGB(255,17,102));
	outtextxy(WIN_WIDTH/2-253, 53, "AyaRenju 五子棋");
	setcolor(RGB(255,173,241));
	outtextxy(WIN_WIDTH/2-250, 50, "AyaRenju 五子棋");
	//暂停提示
	setcolor(RGB(168,51,153));
	outtextxy(WIN_WIDTH/2-103, 153, "Pause...");
	setcolor(RGB(187,238,153));
	outtextxy(WIN_WIDTH/2-100, 150, "Pause...");
	//返回游戏
	RECT r;
	r.left=(WIN_WIDTH-120)/2;
	r.top=240;
	r.right=r.left+120;
	r.bottom=r.top+35;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//返回按键响应
		if(isMouseDown){
			isMouseDown=false;
			FlushMouseMsgBuffer();
			gamePhase=GAME_RUN;
		}
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 14, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+5,"Resume");
	//重新开始
	r.left=(WIN_WIDTH-120)/2;
	r.top=290;
	r.right=r.left+120;
	r.bottom=r.top+35;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//重新开始按键响应
		if(isMouseDown){
			isMouseDown=false;
			FlushMouseMsgBuffer();
			InitChessBoard();
			gamePhase=GAME_RUN;
		}
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 16, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+5,"Restart");
	//返回开始
	r.left=(WIN_WIDTH-120)/2;
	r.top=340;
	r.right=r.left+120;
	r.bottom=r.top+35;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//返回开始按键响应
		if(isMouseDown){
			isMouseDown=false;
			gamePhase=GAME_START;
		}
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 11, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+5,"Back  Title");
}
//绘制关于界面
void DrawAbout()
{
	settextstyle(52, 30, _T("Impact"));
	//标题
	setcolor(RGB(255,17,152));
	outtextxy(243, 33, "About");
	setcolor(RGB(215,193,238));
	outtextxy(240, 30, "About");
	//底板
	setcolor(RGB(255,17,204));
	setfillcolor(RGB(255,221,255));
	fillrectangle(60,100,570,350);
	//版本
	settextstyle(16, 8, _T("楷体"));
	setcolor(RGB(205,86,52));
	outtextxy(80, 120,"名  称：AyaRenju 五子棋");
	outtextxy(80, 145,"版  本: v0.9 测试版");
	outtextxy(80, 170,"日  期: 2013.10.22");
	outtextxy(80, 195,"作  者: ls9512");
	outtextxy(80, 220,"联系QQ: 598914653                   邮  箱: l-9512@126.com");
	outtextxy(80, 245,"开  发: vc++ & EasyX                编  译: vs2010");
	outtextxy(80, 270,"说  明: 仅供休闲娱乐,学习交流使用.");
	outtextxy(80, 295,"补  充: 1.简单AI算法参考网络资料,针对本程序所需有修改.");
	outtextxy(80, 320,"        2.图标取自互联网,未使用任何其他素材.");
	//返回
	RECT r;
	r.left=(WIN_WIDTH-120)/2;
	r.top=375;
	r.right=r.left+120;
	r.bottom=r.top+35;
	setcolor(COLOR_BBL);
	if(IsInRect(mouseX,mouseY,r)) 
	{
		setfillcolor(COLOR_BBS);
		//返回按键响应
		if(isMouseDown) gamePhase=GAME_START;
	}
	else setfillcolor(COLOR_BB);
	fillrectangle(r.left,r.top,r.right,r.bottom);
	settextstyle(25, 11, _T("Verdana"));
	setcolor(COLOR_BF);
	outtextxy(r.left+5, r.top+5,"Back Title");
}
//绘图控制
void Graphics()
{
	//清屏
	setcolor(COLOR_BACK);
	setfillcolor(COLOR_BACK);
	fillrectangle(0,0,WIN_WIDTH,WIN_HEIGHT);
	switch(gamePhase){
	case GAME_START:	DrawStart();	break;
	case GAME_RUN:		DrawRun();		break;
	case GAME_HELP:		DrawHelp();		break;
	case GAME_PAUSE:	DrawPause();	break;
	case GAME_ABOUT:	DrawAbout();	break;
	}
	//输出帧数
	if(isShowFPS) DrawFPS();
}
///////////////////////////////////////////////////////////////////////////////////////////
//                                  以下是  主入口函数                                    //
///////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	//置随机数种子
	srand((unsigned)time(NULL));
	//初始化设备
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//设置窗口标题
	SetWindowText(GetHWnd(),"AyaRenju v0.9 By:ls9512");
	//设置黑色背景
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	//开启双缓冲,防止闪屏
	BeginBatchDraw();
	//鼠标消息变量
	MOUSEMSG mmsg;
	//置初始状态
	gamePhase=GAME_START;
	while(true){
		//处理所有鼠标消息
		//如此处使用if处理一条消息,会导致鼠标消息累积(主要是移动消息),致使响应迟缓
		while(MouseHit()){
			mmsg = GetMouseMsg();
			switch(mmsg.uMsg){
			case WM_MOUSEMOVE:	mouseX = mmsg.x; mouseY = mmsg.y; break;
			case WM_LBUTTONDOWN: isMouseDown=true ;break;
			case WM_LBUTTONUP: isMouseDown=false; break;
			}
		}
		//逻辑
		Manager();
		//绘图
		Graphics();
		//显示缓存的绘制内容
		FlushBatchDraw();
		//延迟,帧数控制,60帧
		Delay(1);
	}
	// 关闭
	EndBatchDraw();
	closegraph();
	return 0;
}