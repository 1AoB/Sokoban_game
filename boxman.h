#pragma once

//判断是否越界
#define isValid(pos)  pos.x >= 0 && pos.x < LINE && pos.y >= 0 &&pos.y < COLUMN  


#define BG_WIDTH      860   //背景图尺寸
#define BG_LENGTH     668

#define RATIO        61  //道具像素

#define LINE    9   //数组行
#define COLUMN  12  //数组列

#define START_X 65	//起始点偏移量
#define START_Y 65

//控制键  上,下,左,右=>控制方向 ‘q’控制退出
#define KEY_UP	    'w'		//按键
#define KEY_DOWN		's'
#define KEY_LEFT		'a'
#define KEY_RIGHT		'd'
#define KEY_QUIT		'q'


struct _POS {  //小人在地图中的位置
	int x;
	int y;
};
