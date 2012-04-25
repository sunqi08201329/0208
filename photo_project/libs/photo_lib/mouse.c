/*
 *  2010.2.25
 *  mouse.c
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

#include "common.h"
#include "mouse.h"
#include "filename_list.h"


#define C_WIDTH  10
#define C_HEIGHT 17
#define T___     0XFFFFFFFF
#define BORD     0x0
#define X___     0xFFFF
static unsigned long cursor_pixel[C_WIDTH * C_HEIGHT] = {
	BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
	BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
	BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
	BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
	BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
	BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
	BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
	BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
	BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
	BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
	BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
	BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
	BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
	BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
	T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
	T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
	T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};
static unsigned long save_cursor[C_WIDTH * C_HEIGHT];

int test_mouse(fb_info fb_inf)
{
	int mfd;
	mouse_open(NULL, &mfd);

	int m_x = fb_inf.w / 2;
	int m_y = fb_inf.h / 2;
	fb_drawcursor(fb_inf, m_x, m_y);

	u8_t buf[8];
	mouse_event_t mevent;

	int j = 0;
	int i;

	//* *********************test photo**************************** */
	char **file_names = NULL;
	file_names = struct_a_array(file_names);

	//file_list *read_file_name(char * path, file_list *list);
	file_list *photo_list = NULL;
	photo_list = read_file_name(PHOTO_PATH, photo_list);
	int photo_num ;
	print_list(photo_list);

	//char **jose_sort(char **file_names, file_list *list);
	photo_num = jose_sort(file_names, photo_list);


	while(1){	
		int n = read(mfd, buf, 8);
		if (n != -1){

			mouse_parse(buf,&mevent);
			//printf("dx:%d\tdy:%d\n", mevent.x, mevent.y);
			//printf("mx:%d\tmy:%d\n", m_x, m_y);

			fb_restorecursor(fb_inf, m_x, m_y);

			m_x += mevent.x;
			m_y += mevent.y;
			fb_drawcursor(fb_inf, m_x, m_y);

			switch (mevent.button){
				case 1:
					for (i = 1; i < 5; i++) 
					{
						if(j == photo_num)
							break;
						//int display_jpeg_view (char *filename, fb_info fb_inf, int k)
						display_jpeg_view(file_names[j], fb_inf, i);
						j++;

					}
					//printf("left\n");
					mevent.button = 0;
					break;
				case 2:
					//printf("right\n");
					//if(j >= 3)
					//{
						//if(i < 4)
						//{
							//j = j - i - 1;
						//}
						//else
							//j -= 4;
					//}
					//else
						//j = 0;
					if(j == photo_num)
					{
						j -= photo_num % 4;
						if(j == photo_num)
							j -= 4;
						//j --;
					}
					else if(j >= 3)
						j -= 4;
					else 
						j = 0;
					mevent.button = 0;
					
					break;
				case 3:
					//printf("middle\n");
					mevent.button = 0;
					break;
				default:
					mevent.button = 0;
					break;
			}
		}

		usleep(100);
	}
	return 0;
}

int mouse_open(char *device_name, int *fd)
{
	if (NULL == device_name)
		device_name = "/dev/input/mice";

	*fd = open(device_name, O_RDONLY | O_NONBLOCK);
	if(*fd < 0){
		fprintf(stderr, "Open %s:%s\n", device_name, strerror(errno));
		exit(1);
	}

	return 0;
}

int mouse_parse(const u8_t *buf, mouse_event_t* mevent)
{
	switch(buf[0] & 0x7){
		case 1:						/* left */
			mevent->button = 1;
			break;
		case 2:
			mevent->button = 2;		/* right */
			break;
		case 4:
			mevent->button = 3;		/* middle */
			break;
		default:
			break;
	}
#if 0
	mevent->x = buf[1];
	mevent->y = buf[2];
#else
	mevent->x =  buf[1] - ((buf[0] & 0x10) ? 256 : 0);
	mevent->y = -buf[2] + ((buf[0] & 0x20) ? 256 : 0); 
#endif
	return 0;
}

/* cursor */

static int fb_pixel_m(fb_info fb_inf, int x, int y, u32_t color)
{
	/* 16 bits */
	u8_t *pos = (u8_t*)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp/8;


	switch (fb_inf.bpp){
		case 32:				  /* 32 bits */
			*(pos + 3) = color >> 24;
		case 24:				  /* 24 bits */
			*(pos + 2) = color >> 16;
		case 16:	  		       /* 16 bits */
			*(pos + 1) = color >> 8;
		case 8:			      /* 8 bits */
			*pos = color;
			return 0;
		default:
			return -1;
	}
	return -1;
}

int fb_restorecursor(fb_info fb_inf, int x, int y)
{
	int i, j;
	u32_t color;
	for (j = 0;j<C_HEIGHT;j++){
		for (i = 0;i<C_WIDTH;i++){
			color = save_cursor[i + j * C_WIDTH];
			fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}

	return 0;
}
static int  fb_savecursor (fb_info fb_inf,int x,int y)
{
	int i,j;
	u32_t color;

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = *(u32_t*)(fb_inf.fbmem + ((y + j) * fb_inf.w + x + i) * fb_inf.bpp/8);
			save_cursor[i + j * C_WIDTH] = color;
		}
	}
	return 0;
}

int  fb_drawcursor(fb_info fb_inf, int x, int y)
{
	int i,j;
	unsigned int color;

	fb_savecursor (fb_inf,x,y);

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = cursor_pixel[i + j * C_WIDTH];
			if (color != T___)
				fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------
//suo fang----not failed
//----------------------------------------------------------------------------------------
int display_jpeg_view (char *filename, fb_info fb_inf, int k)
{
	fb_info jpeg_inf;

	fb_inf.h = fb_inf.h / 2;	// suo xiao
	fb_inf.w = fb_inf.w / 2;

	u8_t *buf24 = decode_jpeg (filename, &jpeg_inf);
	u8_t *scale_buf = scale24 (buf24, fb_inf, jpeg_inf);	/* 缩放 */
	u32_t *buf32 = rgb24to32 (scale_buf, fb_inf);

	fb_inf.h = fb_inf.h * 2;	// hui fu chi cun 
	fb_inf.w = fb_inf.w * 2;



	int i, j;


	if (k <= 2)
		for (i = 0 + 10 * fb_inf.w / 1024; i < fb_inf.h / 2 - 10 * fb_inf.w / 1024 - 30 * fb_inf.w / 1024; i++)
		{
			for (j = fb_inf.w * k / 2 + 10 * fb_inf.w / 1024; j < fb_inf.w / 2 + fb_inf.w * k / 2 - 10 * fb_inf.w / 1024; j++)
			{
				fb_pixel (fb_inf, j, i, buf32[j - (fb_inf.w * k / 2 + 10 * fb_inf.w / 1024) + (i - 10 * fb_inf.w / 1024) * (fb_inf.w / 2)]);
			}

		}
	else
		for (i = fb_inf.h / 2 + 10 * fb_inf.w / 1024 - 30 * fb_inf.w / 1024; i < fb_inf.h - 10 * fb_inf.w / 1024 - 60 * fb_inf.w / 1024; i++)
		{
			for (j = fb_inf.w * (k - 2) / 2 + 10 * fb_inf.w / 1024; j < fb_inf.w / 2 + fb_inf.w * (k - 2) / 2 - 10 * fb_inf.w / 1024;
					j++)
			{
				fb_pixel (fb_inf, j, i, buf32[j - (fb_inf.w * (k - 2) / 2 + 10 * fb_inf.w / 1024) + (i - (fb_inf.h / 2 + 10 * fb_inf.w / 1024 - 30 * fb_inf.w / 1024)) * (fb_inf.w / 2)]);
			}

		}

	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}
