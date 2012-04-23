#include "common.h"

#define SCREEN_W fb_inf.w/1024
#define SCREEN_H fb_inf.h/768

/***************************************************
* mouse leave
****************************************************/
void mouse_leave(fb_info fb_inf,int row,int col,int flag[4])
{
	int size = 32 * fb_inf.w/1024;
	int x_start = 60 * fb_inf.w/1024;
	int y_start = 700 * fb_inf.h/768; 

	if((!((row > 60 * SCREEN_W)&&(row < 124 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H)))&&(flag[0] == 1))
		display_string("预览",x_start,y_start,fb_inf,COLOR);
	if((!((row > 188 * SCREEN_W)&&(row < 256 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H)))&&(flag[0] == 1))
		display_string("幻灯",x_start + size * 4,y_start,fb_inf,COLOR);
	if((!((row > 316 * SCREEN_W)&&(row < 380 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H)))&&(flag[0] == 1))
		display_string("音乐",x_start + size*8,y_start,fb_inf,COLOR);
	if((!((row > 444 * SCREEN_W)&&(row < 502 * SCREEN_W)&&(col > 700 * SCREEN_H - 32*SCREEN_W)&&(col < 700 * SCREEN_H)))&&(flag[0] == 1))
		display_string("退出",x_start + size * 12,y_start,fb_inf,COLOR);

	int x = fb_inf.w/4 - size * 3/2;
	int y = fb_inf.h - 20 * fb_inf.h/768;

	if((flag[1] == 1)&&(!((row >= 208 * SCREEN_W)&&(col >= 720 * SCREEN_H)&&(row < 304 * SCREEN_W) &&( col <748 * SCREEN_H))))
		display_string("上一页", x, y, fb_inf, COLOR);

	if((flag[1] == 1)&&(!((row >= 720 * SCREEN_W)&&(col >= 720 * SCREEN_H)&&(row < 816 * SCREEN_W) && (col <748 * SCREEN_H))))
		display_string("下一页", x+fb_inf.w/2, y, fb_inf, COLOR);
}

/*********************************************************
* mouse come on word
**********************************************************/
void mouse_come(fb_info fb_inf,int row,int col,int flag[4])
{
	int size = 32 * fb_inf.w/1024;
	int x_start = 60 * fb_inf.w/1024;
	int y_start = 700 * fb_inf.h/768; 

	if((flag[0] == 1)&&(row > 60 * SCREEN_W)&&(row < 124*SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H))
		display_string("预览",x_start,y_start,fb_inf,0xff0000);
	if((flag[0] == 1)&&(row > 188 * SCREEN_W)&&(row < 256 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H))
		display_string("幻灯",x_start + size * 4,y_start,fb_inf,0xff0000);
	if((flag[0] == 1)&&(row > 316 * SCREEN_W) && (row < 380 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W) && (col < 700 * SCREEN_H))
		display_string("音乐",x_start + size * 8,y_start,fb_inf,0xff0000);
	if((flag[0] == 1)&&(row > 444 * SCREEN_W) && (row < 502 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W) && (col < 700 * SCREEN_H))
		display_string("退出",x_start + size*12,y_start,fb_inf,0xff0000);

	int x = fb_inf.w/4 - size * 3/2;
	int y = fb_inf.h - 20 * fb_inf.h/768;

	if((flag[1] == 1)&&(row >= 208 * SCREEN_W)&&(col >= 720 * SCREEN_H)&&(row < 304 * SCREEN_W) &&( col <748 * SCREEN_H))
		display_string("上一页", x, y, fb_inf, 0xff0000);

	if((flag[1] == 1)&&(row >= 720 * SCREEN_W)&&(col >= 720 * SCREEN_H)&&(row < 816 * SCREEN_W) && (col <748 * SCREEN_H))
		display_string("下一页", x+fb_inf.w/2, y, fb_inf, 0xff0000);
}
