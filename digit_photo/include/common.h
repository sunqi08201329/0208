#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>
#include <dirent.h>

#define COLOR 0x8A2BE2

typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;

typedef struct{
	int w;
	int h;
	int bpp;
	u8_t *fbmem;
}fb_info;

int pic_menu(fb_info fb_inf,int flag[4],int row,int col,int page,char filename[4][20]);

int welcome_menu(fb_info fb_inf,pid_t *pid,int flag[4],int row,int col,char filename[4][20]);

int view_menu(fb_info fb_inf,int flag[4],int row,int col, char filename[4][20],int *page,int count);

void mouse_come(fb_info fb_inf,int row,int col,int flag[4]);

void mouse_leave(fb_info fb_inf,int row,int col,int flag[4]);

int show(char *filename, fb_info fb_inf,int k);

void view_pic(fb_info fb_inf, char name[4][20]);

int get_pic_name(char filename[4][20],int page);

int create_list(const char *pathname,char *listname,char *type);

int mouse(fb_info fb_inf);

void play_pic(fb_info fb_inf);

void welcome(fb_info fb_inf);

void play_welcome(fb_info fb_inf);

int play_list(fb_info fb_inf,int page);

void play_fun(fb_info fb_inf);

void goodbye(fb_info fb_inf);

void mp3(int num,fb_info fb_inf);

void set_vol(int vol);

int init_fb(fb_info *fb_inf);

int init_mouse(void);

int init_ft (const char *file, int size);

int display_string (const char * buf, int x, int y, fb_info fb_inf,u32_t color);

void set_dsp(void);

int play_mp3(char *mp3_name);

int draw_pixel(fb_info fb_inf,int x,int y,u32_t color);

int display_jpeg(char *filename,fb_info fb_inf);

int display_block(char *filename, fb_info fb_inf);

int display_one(char *filename,fb_info fb_inf);

int display_jpeg6(char *filename,fb_info fb_inf);

u_char *decode_jpeg (const char *filename, fb_info *jpeg_inf);

u32_t * rgb24to32(u8_t *buf24, fb_info jpeg_inf);

u8_t * scale24(u8_t *buf24, fb_info new_inf, fb_info jpeg_inf);

#endif
