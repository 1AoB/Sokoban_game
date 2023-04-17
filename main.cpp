#include<iostream>
#include<graphics.h>
#include<stdlib.h>//可以暂停
#include<string>
#include<conio.h>//这个头文件用于获取热键

#include "boxman.h"//自己定义的头文件要用双引号

using namespace std;


struct _POS man;

enum _DIRECTION {	//人物将要移动的方向
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum _PROPS {
	WALL,			//墙
	FLOOR,			//地板
	BOX_DES,		//箱子目的地
	MAN,			//小人
	BOX,			//箱子
	HIT,			//箱子命中目标
	ALL
};

IMAGE images[ALL];	//道具加载数组

int map[LINE][COLUMN] = {	//地图
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 4, 1, 0, 2, 1, 0, 2, 1, 0, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0 },
		{ 0, 1, 0, 2, 0, 1, 1, 4, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 0, 3, 1, 1, 1, 4, 1, 0 },
		{ 0, 1, 2, 1, 1, 4, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

//改变游戏地图视图中一格对应道具并重新显示
//prop –道具的类型
void changeMap(struct _POS* p, enum _PROPS prop) {
	map[p->x][p->y] = prop;
	putimage(START_X + p->y * RATIO, START_Y + p->x * RATIO, &images[prop]);
}
/**********************************************
*实现游戏四个方向（上、下、左、右）的控制
*输入:
* direct - 人前进方向
*输出:无
**********************************************/
void gameControl(enum  _DIRECTION direct) {//小人前进的方向
	struct _POS next_pos = man;
	struct _POS next_next_pos = man;
	switch (direct) {//switch是if代码的优化
	case UP:
		next_pos.x--;//这样就可以使得向上推箱子的动作不断进行
		next_next_pos.x -= 2;
		break;
	case DOWN:
		next_pos.x++;//小人的位置发生变动
		next_next_pos.x += 2;
		break;
	case LEFT:
		next_pos.y--;
		next_next_pos.y -= 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_next_pos.y += 2;
		break;
	}
	//宏展开：next_pos.x >= 0 && next_pos.x < LINE && next_pos.y >= 0 && next_pos.y < COLUMN
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR) {
		changeMap(&next_pos, MAN);//小人前进一格
		changeMap(&man, FLOOR);
		man = next_pos;
	}
	else if (isValid(next_next_pos) && map[next_pos.x][next_pos.y] == BOX) {
		if (map[next_next_pos.x][next_next_pos.y] == FLOOR) {
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN); //小人前进一格
			changeMap(&man, FLOOR);
			man = next_pos;
		}
		else if (map[next_next_pos.x][next_next_pos.y] == BOX_DES) {
			changeMap(&next_next_pos, HIT);
			changeMap(&next_pos, MAN); //小人前进一格
			changeMap(&man, FLOOR);
			man = next_pos;
		}
	}
};

/**********************************************
*判断游戏是否结束，如果不存在任何一个箱子目的地，就代表游戏结束
* true - 游戏结束 false - 游戏继续
**********************************************/
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == BOX_DES)
				return false;
		}
	}
	return true;
}

/************************
*游戏结束场景，在玩家通关后显示
* bg - 背景图片变量的指针
*************************/
void gameOver(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, BG_WIDTH, BG_LENGTH };
	settextstyle(20, 0, _T("宋体"));
	drawtext(_T("恭喜您~您终于成为一个合格的搬箱子老司机了!\n "), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

int main(void) {
	IMAGE bg_image;
	initgraph(BG_WIDTH, BG_LENGTH, NULL);
	loadimage(&bg_image, _T("blackground.bmp"), BG_WIDTH, BG_LENGTH, true);
	putimage(0, 0, &bg_image);

	loadimage(&images[0], _T("wall.bmp"), RATIO, RATIO, true);
	loadimage(&images[1], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[2], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[3], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[4], _T("box.bmp"), RATIO, RATIO, true);
	loadimage(&images[5], _T("box.bmp"), RATIO, RATIO, true);

	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
			if (map[i][j] == MAN) {
				man.x = i;
				man.y = j;
			}
		}
	}

	bool quit = false;

	do {
		if (_kbhit()) {//如果有击键的操作,_kbhit函数就会返回ture
			char ch = _getch();

			if (ch == KEY_UP) {//用switch语句和if语句都是可以的
				gameControl(UP);
			}
			else if (ch == KEY_DOWN) {
				gameControl(DOWN);
			}
			else if (ch == KEY_LEFT) {
				gameControl(LEFT);
			}
			else if (ch == KEY_RIGHT) {
				gameControl(RIGHT);
			}
			else if (ch == KEY_QUIT) {
				quit = true;
			}
			if (isGameOver()) {
				gameOver(&bg_image);
			}
		}
		Sleep(100);
	} while (quit == false);//!quit

	closegraph();
	system("pause");
	return 0;
}
