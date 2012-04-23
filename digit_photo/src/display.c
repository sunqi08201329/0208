#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "common.h"

/********************************************************
* delay
*********************************************************/
int my_delay(int num)
{
	int i;
	
	for(i = 0; i < num; i++)
		;
	
	return 0;
}

/*******************************************************
 * draw pixel
 *******************************************************/
int draw_pixel(fb_info fb_inf,int x,int y,u32_t color)
{
	if((x >= 0) && (x < fb_inf.w) && (y >= 0) && (y < fb_inf.h))
	{
		u8_t *pos = (u8_t *)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp / 8;
	
		switch(fb_inf.bpp)
		{
			case 32:
				*(pos + 3) = color >> 24;
			
			case 24:
				*(pos + 2) = color >> 16;
			
			case 16:
				*(pos + 1) = color >> 8;
			
			case 8:
				*pos = color;

				return 0;
			
			default:
				
				return -1;
		}
		
	}

	return -1;
}

/*****************************************************************
 * swap x,y
 ********************************************************************/
int swap(int * x,int * y)
{
	int z = *x;
	*x = *y;
	*y = z;

	return 0;
}

/*******************************************************************
 * picture draw a line
 *********************************************************************/
int draw_line(fb_info fb_inf,int x1,int y1,int x2,int y2, u32_t * buf16)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int p,inc = ((dx*dy) >= 0) ? 1 : -1;
	
	if(abs(dx) > abs(dy))
	{
		if(dx < 0)
		{
			swap(&x1,&x2);
			swap(&y1,&y2);
			dx = -dx;
			dy = -dy;
		}
		
		dy = (dy > 0) ? dy : -dy;
		p= 2 * dy - dx;
		
		while(x1++ <= x2)
		{
			if((x1-1 >= 0) && (x1 - 1 < fb_inf.w) && (y1 >= 0) && (y1 < fb_inf.h))
				
				draw_pixel(fb_inf,x1-1,y1, buf16[fb_inf.w * y1 + x1 - 1]);
			
			if(p < 0)
			{
				p += 2 * dy;
			}
			else
			{
				y1 += inc;
				p += 2 * (dy - dx);
			}		
			
		}
		
	}
	else
	{
		if(dy < 0)
		{
			swap(&x1,&x2);
			swap(&y1,&y2);
			dx = -dx;
			dy = -dy;
		}
		
		dx = (dx > 0) ? dx : -dx;
		p = 2 * dx - dy;
		
		while(y1++ < y2)
		{
			if((x1 >= 0) && (x1 < fb_inf.w) && (y1-1 >= 0) && (y1-1 < fb_inf.h))
				
				draw_pixel(fb_inf,x1,y1-1,buf16[fb_inf.w * (y1 - 1) + x1]);

			if(p < 0)
			{
				p += 2 * dx;

			}
			else
			{
				x1 += inc;
				p += 2 * (dx - dy);
			}
					
		}
		
	}

	return 0;
}

/*********************************************************************
 * picture windmill special efficacy
 *******************************************************************/
int display_jpeg22(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf16 = rgb24to32(scale_buf, fb_inf);

	int i;

	for(i = 0; i <= fb_inf.w/2 + fb_inf.h/2; i++)
	{
		if(i < fb_inf.w/2)
		{
			draw_line(fb_inf,fb_inf.w/2+i,0,fb_inf.w/2-1-i,fb_inf.h-1,buf16);
		}
		else
		{
			draw_line(fb_inf,fb_inf.w-1,i-fb_inf.w/2,0,fb_inf.h + fb_inf.w/2- i,buf16);
		}

		if(i < fb_inf.h/2)				
		{
			draw_line(fb_inf,fb_inf.w-1,i+fb_inf.h/2-1,0,fb_inf.h/2-1-i,buf16);
		}
		else
		{
			draw_line(fb_inf,i-fb_inf.h/2,0,fb_inf.w + fb_inf.h/2 -i,fb_inf.h-1,buf16);
		}

	}
		
	free(buf24);
	free(scale_buf);
	free(buf16);

	return 0;
}

/***************************************************************
 * picture five star special efficacy
 ****************************************************************/
int display_jpeg23(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf16 = rgb24to32(scale_buf, fb_inf);

	int r,i,c = 0;
	int coordinate[5][2]; 

	for (r = 0;r < fb_inf.w;r++)
	{
		for(i = 0; i < 360; i += 72)
		{
			int m = sin((i + c) * 3.14/180) * r;
			int n = cos((i + c) * 3.14/180) * r;
			coordinate[i/72][0] = fb_inf.w/2 + m;
			coordinate[i/72][1] = fb_inf.h/2 - n;
		}

		draw_line(fb_inf,coordinate[0][0],coordinate[0][1],coordinate[2][0],coordinate[2][1],buf16);
		draw_line(fb_inf,coordinate[0][0],coordinate[0][1],coordinate[3][0],coordinate[3][1],buf16);
		draw_line(fb_inf,coordinate[4][0],coordinate[4][1],coordinate[1][0],coordinate[1][1],buf16);
		draw_line(fb_inf,coordinate[4][0],coordinate[4][1],coordinate[2][0],coordinate[2][1],buf16);
		draw_line(fb_inf,coordinate[1][0],coordinate[1][1],coordinate[3][0],coordinate[3][1],buf16);
		
		c += 10;
		c %= 360;
	}

	free(buf24);
	free(scale_buf);
	free(buf16);

	return 0;
}

/***************************************************************
 * picture empty five star special efficacy
 ****************************************************************/
int display_jpeg24(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf16 = rgb24to32(scale_buf, fb_inf);

	int r,i,c=0; 
	int coordinate[5][2],coordinate1[5][2],coordinate2[5][2]; 

	for (r = 0;r < fb_inf.w/2;r++)
	{
		for(i = 0; i < 360; i += 72)
		{
			int m = sin((i + c) * 3.14/180) * r;
			int n = cos((i + c) * 3.14/180) * r;
			coordinate[i/72][0] = fb_inf.w/2 + m;
			coordinate[i/72][1] = fb_inf.h/4 * 3 - n;
			coordinate1[i/72][0] = fb_inf.w/5 + m;
			coordinate1[i/72][1] = fb_inf.h/4 -n;
			coordinate2[i/72][0] = fb_inf.w/5 * 4 + m;
			coordinate2[i/72][1] = fb_inf.h/4 - n;
		}

		draw_line(fb_inf,coordinate[0][0],coordinate[0][1],coordinate[2][0],coordinate[2][1],buf16);
		draw_line(fb_inf,coordinate[0][0],coordinate[0][1],coordinate[3][0],coordinate[3][1],buf16);
		draw_line(fb_inf,coordinate[4][0],coordinate[4][1],coordinate[1][0],coordinate[1][1],buf16);
		draw_line(fb_inf,coordinate[4][0],coordinate[4][1],coordinate[2][0],coordinate[2][1],buf16);
		draw_line(fb_inf,coordinate[1][0],coordinate[1][1],coordinate[3][0],coordinate[3][1],buf16);
		draw_line(fb_inf,coordinate1[0][0],coordinate1[0][1],coordinate1[2][0],coordinate1[2][1],buf16);
		draw_line(fb_inf,coordinate1[0][0],coordinate1[0][1],coordinate1[3][0],coordinate1[3][1],buf16);
		draw_line(fb_inf,coordinate1[4][0],coordinate1[4][1],coordinate1[1][0],coordinate1[1][1],buf16);
		draw_line(fb_inf,coordinate1[4][0],coordinate1[4][1],coordinate1[2][0],coordinate1[2][1],buf16);
		draw_line(fb_inf,coordinate1[1][0],coordinate1[1][1],coordinate1[3][0],coordinate1[3][1],buf16);
		draw_line(fb_inf,coordinate2[0][0],coordinate2[0][1],coordinate2[2][0],coordinate2[2][1],buf16);
		draw_line(fb_inf,coordinate2[0][0],coordinate2[0][1],coordinate2[3][0],coordinate2[3][1],buf16);
		draw_line(fb_inf,coordinate2[4][0],coordinate2[4][1],coordinate2[1][0],coordinate2[1][1],buf16);
		draw_line(fb_inf,coordinate2[4][0],coordinate2[4][1],coordinate2[2][0],coordinate2[2][1],buf16);
		draw_line(fb_inf,coordinate2[1][0],coordinate2[1][1],coordinate2[3][0],coordinate2[3][1],buf16);
			
		c += 10;
		c %= 360;
	}

	free(buf24);
	free(scale_buf);
	free(buf16);

	return 0;
}

/****************************************************************
* picture full_square
*****************************************************************/
int display_jpeg19(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
        
  u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
  u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
  u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
        
  int i, j, k, m;

  for(i = 0; i <= fb_inf.h/20; i++)
  {
		for(k = 0; k < fb_inf.h-fb_inf.h/10; k += (fb_inf.h/10))
		{
			for(m = 0; m < fb_inf.w-fb_inf.w/10; m += (fb_inf.w/10))
			{
      	for (j = fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i; j < fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i; j++)
      	{
       		draw_pixel(fb_inf, m + j, k + fb_inf.h/20 - i, buf32[m + j + (k + fb_inf.h/20 - i) * fb_inf.w]);
	        draw_pixel(fb_inf, m + j, k + fb_inf.h/20 - 1 + i, buf32[m + j + (k + fb_inf.h/20-1 + i) * fb_inf.w]);
        }	
        	
      }
      
		}
		
		usleep(100);
	}
		
	usleep(1000);
	
	for(i = 0; i <= fb_inf.h/20; i++)
	{
		for(k = 0; k < fb_inf.h-fb_inf.h/10; k += (fb_inf.h/10))
		{
			for(m = 0; m < fb_inf.w-fb_inf.w/10; m += (fb_inf.w/10))
			{
				for(j = fb_inf.h/20 - i; j < fb_inf.h/20 + i; j++)
				{
					draw_pixel(fb_inf,m + fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i,k + j,buf32[m + fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i + (k + j) * fb_inf.w]);
         		draw_pixel(fb_inf,m + fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i,k + fb_inf.h/10 - 1 - j,buf32[m + fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i + (k + fb_inf.h/10 - 1 - j) * fb_inf.w]);
 				}
 				
      }
      
		}
		
		usleep(100);
	}

	for(i = 1; i <= fb_inf.h%10; i++)
		for(j = 0; j < fb_inf.w; j++)
			draw_pixel(fb_inf,j,fb_inf.h - i,buf32[j + (fb_inf.h - i) * fb_inf.w]);

	for(i = 1; i <= fb_inf.w%10; i++)
		for(j = 0; j < fb_inf.h; j++)
			draw_pixel(fb_inf,fb_inf.w - i,j,buf32[fb_inf.w - i + j * fb_inf.w]);

  free(buf24);
  free(scale_buf);
  free(buf32);
        
  return 0;
}

/*******************************************************
* picture sandglass
********************************************************/
int display_jpeg2(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
        
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
        
	int i, j, k, m;

	for(i = 0; i <= fb_inf.h/20; i++)
	{
		for(k = 0; k < fb_inf.h-fb_inf.h/10; k += (fb_inf.h/10))
		{
			for(m = 0; m < fb_inf.w-fb_inf.w/10; m += (fb_inf.w/10))
			{
      	for (j = fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i; j < fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i; j++)
      	{
        	draw_pixel(fb_inf, m + j, k + fb_inf.h/20 - i, buf32[m + j + (k + fb_inf.h/20 - i) * fb_inf.w]);
	        draw_pixel(fb_inf, m + j, k + fb_inf.h/20 - 1 + i, buf32[m + j + (k + fb_inf.h/20 - 1 + i) * fb_inf.w]);
        }
		
				for(j = fb_inf.h/20 - i; j < fb_inf.h/20 + i; j++)
				{
					draw_pixel(fb_inf, m + fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i, k + j, buf32[m + fb_inf.w/20 + (fb_inf.w/10 - fb_inf.h/10)/2 + i + (k + j) * fb_inf.w]);
          draw_pixel(fb_inf, m + fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i, k + fb_inf.h/10 - 1 - j, buf32[m + fb_inf.w/20 - (fb_inf.w/10 - fb_inf.h/10)/2 - i + (k + fb_inf.h/10 - 1 - j) * fb_inf.w]);
 				}
 				
      }
      
		}
		
	}

	for(i = 1; i <= fb_inf.h%10; i++)
		for(j = 0; j < fb_inf.w; j++)
			draw_pixel(fb_inf,j,fb_inf.h - i,buf32[j + (fb_inf.h - i) * fb_inf.w]);

	for(i = 1; i <= fb_inf.w%10; i++)
		for(j = 0; j < fb_inf.h; j++)
			draw_pixel(fb_inf,fb_inf.w - i,j,buf32[fb_inf.w - i + j * fb_inf.w]);

	free(buf24);
	free(scale_buf);
	free(buf32);
        
  return 0;
}

/***********************************************************
* welcome
************************************************************/
void welcome(fb_info fb_inf)
{
	char *filename = "background/welcome.jpg";

	display_jpeg23(filename,fb_inf);
	display_jpeg6(filename,fb_inf);

	int size = 32 * fb_inf.w/1024;
	int x_start = 60 * fb_inf.w/1024;
	int y_start = 700 * fb_inf.h/768; 

	display_string("预览",x_start,y_start,fb_inf,COLOR);
	display_string("幻灯",x_start + size*4,y_start,fb_inf,COLOR);
	display_string("音乐",x_start + size*8,y_start,fb_inf,COLOR);
	display_string("退出",x_start + size*12,y_start,fb_inf,COLOR);
}

/**************************************************************
* goodbye
***************************************************************/
void goodbye(fb_info fb_inf)
{
	char *filename = "background/goodbye.jpg";
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	srand(time(NULL));
	
	int i, j;
	int times;
	char a[1600]={0};
	char b[1600]={0};
	
	for(times = 0; times < 10000; times++)
	{
		i = rand()%fb_inf.h;
		
		if(a[i] == 0)
		{
			a[i] = 1;
			
			for (j = 0; j < fb_inf.w; j++)
			{
				draw_pixel(fb_inf, j,i,buf32[j + i * fb_inf.w]);
			}

			usleep(1000);
		}
		
	}
	
	sleep(2);

	for(times = 0; times < 10000; times++)
	{
		i = rand()%fb_inf.h;
		
		if(b[i] == 0)
		{
			b[i] = 1;
			
			for (j = 0; j < fb_inf.w; j++)
			{
				draw_pixel(fb_inf,j,i,0);
			}

			usleep(1000);
		}
		
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
}

/**********************************************************
* play_welcome
***********************************************************/
void play_welcome(fb_info fb_inf)
{
	char *filename = "background/welcome.jpg";
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;

	for(i = 0; i < fb_inf.h; ++i)
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
			
	}

	free(buf24);
	free(scale_buf);
	free(buf32);

	int size = 32 * fb_inf.w/1024;
	int x_start = 60 * fb_inf.w/1024;
	int y_start = 700 * fb_inf.h/768; 

	display_string("预览",x_start,y_start,fb_inf,COLOR);
	display_string("幻灯",x_start + size * 4,y_start,fb_inf,COLOR);
	display_string("音乐",x_start + size * 8,y_start,fb_inf,COLOR);
	display_string("退出",x_start + size * 12,y_start,fb_inf,COLOR);
}

/*****************************************************************
* play_list
*****************************************************************/
int play_list(fb_info fb_inf,int page)
{
	int size = 32 * fb_inf.w/1024;
	int x_start = 200 * fb_inf.w/1024;
	int y_start = 120 * fb_inf.h/768; 
	
	FILE *pFile;
	
	if((pFile = fopen("list/mp3list.txt","r+")) != NULL)
	{
		int i = 0;
		char mp3_name[128];
		
		for(i = 0; i < 8*page; i++)
			fgets(mp3_name,64,pFile);			
		
		i = 0;

		while(i < 8)
		{
			fgets(mp3_name,64,pFile);
			
			if(strstr(mp3_name,".mp3") != NULL)
			{
				mp3_name[strlen(mp3_name) - 5] = '\0';
				display_string(mp3_name,x_start,y_start,fb_inf,COLOR);
			}
				y_start += size + 20 * fb_inf.h/768;	
				i++;
		}
		
		fclose(pFile);
	}
	
	return 0;
}

/***************************************************************
* play_function
****************************************************************/
void play_fun(fb_info fb_inf)
{
	display_string("<",355 * fb_inf.w/1024,620 * fb_inf.h/768,fb_inf,COLOR);
	display_string(">",410 * fb_inf.w/1024,610 * fb_inf.h/768,fb_inf,COLOR);
	display_string("-",55 * fb_inf.w/1024,630 * fb_inf.h/768,fb_inf,COLOR);
	display_string("+",105 * fb_inf.w/1024,605 * fb_inf.h/768,fb_inf,COLOR);
	display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,COLOR);
	display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,COLOR);
	display_string("上一首",285 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,COLOR);
	display_string("下一首",422 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,COLOR);
	display_string("循环",610 * fb_inf.w/1024,722 * fb_inf.h/768,fb_inf,COLOR);
	display_string("随机",750 * fb_inf.w/1024,720 * fb_inf.h/768,fb_inf,COLOR);	
}

/****************************************************************
* display_jpeg
*****************************************************************/
int display_jpeg(char *filename, fb_info fb_inf)
{
	if(strstr(filename,".jpg")!=NULL)
	{
		fb_info jpeg_inf;
    
		char path[128]="image/";
		strncat(path,filename,strlen(filename));
		path[strlen(path)-1] = '\0';

		u8_t *buf24 = decode_jpeg(path, &jpeg_inf);
		u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
		u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

		int i, j;
	
		for(i = 0; i < fb_inf.h; ++i)
		{
			for (j = 0; j < fb_inf.w; ++j)
			{
				draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
			}
		}

		free(buf24);
		free(scale_buf);
		free(buf32);
	
	}
	
	return 0;
}

/*********************************************************************
* display block
*********************************************************************/
int display_block(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for(i = 70*fb_inf.h/768; i < 565*fb_inf.h/768; ++i)
	{
		for (j = 130*fb_inf.w/1024; j < 750*fb_inf.w/1024; ++j)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/*********************************************************************
* display one picture
*********************************************************************/
int display_one(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for(i = 0; i < fb_inf.h; ++i)
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/***************************************************************
* baiyechuang left_to_right
****************************************************************/
int display_jpeg1(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;

	for( j = 0; j < fb_inf.w/8; j++)
	{
		for(i = 0;i < fb_inf.h; i++)
		{
			draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
			draw_pixel(fb_inf,j + fb_inf.w/8,i,buf32[j + fb_inf.w/8 + i * fb_inf.w]);
			draw_pixel(fb_inf,j + fb_inf.w/4,i,buf32[j + fb_inf.w/4 + i * fb_inf.w]);
			draw_pixel(fb_inf,j + (3 * fb_inf.w/8),i,buf32[j + (3 * fb_inf.w/8) + i * fb_inf.w]);
			draw_pixel(fb_inf,j + (fb_inf.w/2),i,buf32[j+fb_inf.w/2 + i * fb_inf.w]);
			draw_pixel(fb_inf,j + (5 * fb_inf.w/8),i,buf32[j + (5 * fb_inf.w/8) + i * fb_inf.w]);
			draw_pixel(fb_inf,j + (3 * fb_inf.w/4),i,buf32[j + (3 * fb_inf.w/4) + i * fb_inf.w]);
			draw_pixel(fb_inf,j + (7 * fb_inf.w/8),i,buf32[j + (7 * fb_inf.w/8) + i * fb_inf.w]);
		}

	my_delay(0xfffff);

	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/************************************************************
* up_to_down and down_to_up
*************************************************************/
int display_jpeg3(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
	int i,j;	

	for(i = 0; i < fb_inf.h; i++)
	{
  		for(j = fb_inf.w/2; j < fb_inf.w; j++)
  		{
				draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);		
			}
			
	my_delay(0xfffff);		
	}

	for(i = fb_inf.h - 1; i > 0; i--)
	{
  		for(j = 0; j < fb_inf.w/2; j++)
  		{
				draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
			}
			
	my_delay(0xfffff);		

	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/********************************************************
* chess
*********************************************************/
int display_jpeg4(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	
	int i,j,m,n;

	for(j = 0; j < fb_inf.w/16; j++) 
	{
		for(i = 0; i < fb_inf.h/12; i++) 
		{
			for(n = 0; n < 16; n += 2)
			{
				for(m = 0; m < 12; m += 2)
				{
					draw_pixel(fb_inf, j + n * fb_inf.w/16, i + m * fb_inf.h/12, buf32[j+n*fb_inf.w/16 +( i+m*fb_inf.h/12) * fb_inf.w]);
					draw_pixel(fb_inf, j + (n + 1) * fb_inf.w/16, i + (m + 1) * fb_inf.h/12, buf32[j + (n + 1) * fb_inf.w/16 + (i + (m + 1) * fb_inf.h/12) * fb_inf.w]);
				}
				
			}
			
		}

	my_delay(0xfffff);
	}

	for(j = 0; j < fb_inf.w/16; j++)    
	{
		for(i = 0; i < fb_inf.h/12; i++)    
		{
			for(n = 0; n < 16; n += 2)
			{
				for(m = 0; m < 12; m += 2)
				{
					draw_pixel(fb_inf, j + (n + 1) * fb_inf.w/16, i + m * fb_inf.h/12, buf32[j + (n + 1) * fb_inf.w/16 + (i + m * fb_inf.h/12) * fb_inf.w]);
					draw_pixel(fb_inf, j + n * fb_inf.w/16, i + (m + 1) * fb_inf.h/12, buf32[j + n * fb_inf.w/16 + (i + (m + 1) * fb_inf.h/12) * fb_inf.w]);
				}
				
			}
			
		}

		my_delay(0xfffff);
	}

  
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/**************************************************************************
* vertical carding
**************************************************************************/		
int display_jpeg5(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i,j;
	int m=fb_inf.h-1;

	for(i = 0;i < fb_inf.h; i++)
	{
		for(j = 0;j < fb_inf.w/8; j++)
		{	    
		    draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
		    draw_pixel(fb_inf,j + fb_inf.w/9,m,buf32[j + fb_inf.w/9 + m * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 2 * fb_inf.w/9,i,buf32[j + 2 * fb_inf.w/9 + i * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 3 * fb_inf.w/9,m,buf32[j + 3 * fb_inf.w/9 + m * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 4 * fb_inf.w/9,i,buf32[j + 4 * fb_inf.w/9 + i * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 5 * fb_inf.w/9,m,buf32[j + 5 * fb_inf.w/9 + m * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 6 * fb_inf.w/9,i,buf32[j + 6 * fb_inf.w/9 + i * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 7 * fb_inf.w/9,m,buf32[j + 7 * fb_inf.w/9 + m * fb_inf.w]);		
		    draw_pixel(fb_inf,j + 8 * fb_inf.w/9,i,buf32[j + 8 * fb_inf.w/9 + i * fb_inf.w]);		
		}

		m--;
		
		my_delay(0xfffff);
	}

   free(buf24);
   free(scale_buf);
   free(buf32);
	
   return 0;
}

/********************************************************
* rand
*********************************************************/
int display_jpeg6(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	srand(time(NULL));
	int i, j;
	int times;
	char a[1600]={0};
  
	for(times = 0; times < 10000; times++)
	{
		i = rand()%fb_inf.h;

		if(a[i] == 0)
		{
			a[i] = 1;

			for (j = 0; j < fb_inf.w; j++)
			{
		     		draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
			}
      
			my_delay(0xfffff);
		}
    
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/***************************************************
* up_to_dowm
****************************************************/
int display_jpeg7(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for (j = fb_inf.w/2; j < fb_inf.w; j++)
  	{
    		for(i = 0; i < fb_inf.h/2; i++)
    		  	draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);

  		    my_delay(0xaffff);
	}      
   
	for(i = fb_inf.h/2; i < fb_inf.h; i++)
	{
		for (j = fb_inf.w/2; j < fb_inf.w; j++)
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		
		my_delay(0x1fffff);
	}

	for(j = fb_inf.w/2; j > 0; j--)
	{
  		for(i = fb_inf.h/2; i < fb_inf.h; i++)
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);

		my_delay(0xaffff);        
	}      
 
	for(i = fb_inf.h/2; i > 0; i--)
	{
		for (j = 0; j < fb_inf.w/2; j++)
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);

		my_delay(0x1fffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/*********************************************************
* baiyechuang up_to_down
**********************************************************/
int display_jpeg8(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;

	for(i = 0; i < fb_inf.h/6; i++)
	{
  		for (j = 0; j < fb_inf.w; j++)
		{
   		 	draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
			draw_pixel(fb_inf, j, i	+ fb_inf.h / 6 * 1, buf32[j + (i + fb_inf.h / 6 * 1) * fb_inf.w]);
			draw_pixel(fb_inf, j, i	+ fb_inf.h / 6 * 2, buf32[j + (i + fb_inf.h / 6 * 2) * fb_inf.w]);
			draw_pixel(fb_inf, j, i	+ fb_inf.h / 6 * 3, buf32[j + (i + fb_inf.h / 6 * 3) * fb_inf.w]);
			draw_pixel(fb_inf, j, i	+ fb_inf.h / 6 * 4, buf32[j + (i + fb_inf.h / 6 * 4) * fb_inf.w]);
			draw_pixel(fb_inf, j, i	+ fb_inf.h / 6 * 5, buf32[j + (i + fb_inf.h / 6 * 5) * fb_inf.w]);
		}
    
		my_delay(0xafffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/************************************************************
* diamond
*************************************************************/
int display_jpeg9(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j,n, m = 0;

	for(i = 0; i < fb_inf.w; i++)
	{
		n = i;
  	
		if( i > fb_inf.h -1 )    
		{
			n = fb_inf.h - 1;
			m++;
		}
    
		for (j = 0; j < n + 1; j++)
		{
			draw_pixel(fb_inf, n - j + m, j, buf32[n - j + m + j * fb_inf.w]);
			draw_pixel(fb_inf, n - j + m, fb_inf.h - 1 - j, buf32[n - j + m + (fb_inf.h - 1 - j) * fb_inf.w]);
			draw_pixel(fb_inf, fb_inf.w - 1 - n + j - m, fb_inf.h - 1 -j, buf32[fb_inf.w - 1 - n + j - m + (fb_inf.h - 1 - j) * fb_inf.w]);
			draw_pixel(fb_inf, fb_inf.w - 1 - n + j - m, j, buf32[fb_inf.w - 1 - n + j - m + j * fb_inf.w]);
		}
    
		my_delay(0xffff);
		}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/********************************************************
* four_square
*********************************************************/
int display_jpeg10(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;

	for(j = 0; j < fb_inf.w; j++)
	{
		for(i = 0; i < fb_inf.h/8; i++)
		{
			draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
			draw_pixel(fb_inf,fb_inf.w - j,i + fb_inf.h/8, buf32[fb_inf.w - j + (i + fb_inf.h/8) * fb_inf.w]);
			draw_pixel(fb_inf,j,i + fb_inf.h/4, buf32[j + (i + fb_inf.h/4) * fb_inf.w]);
			draw_pixel(fb_inf,fb_inf.w - j,i + 3 * fb_inf.h/8, buf32[fb_inf.w - j + (i + 3 * fb_inf.h/8) * fb_inf.w]);
			draw_pixel(fb_inf,j,i + fb_inf.h/2, buf32[j + (i + fb_inf.h/2) * fb_inf.w]);
			draw_pixel(fb_inf,fb_inf.w - j,i + 5 * fb_inf.h/8,buf32[fb_inf.w - j + (i + 5 * fb_inf.h/8) * fb_inf.w]);
			draw_pixel(fb_inf,j,i + 3 * fb_inf.h/4,buf32[j + (i + 3 * fb_inf.h/4) * fb_inf.w]);
			draw_pixel(fb_inf,fb_inf.w - j,i + 7 * fb_inf.h/8,buf32[fb_inf.w - j + (i + 7 * fb_inf.h/8) * fb_inf.w]);     
		}     

	}

 	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}

/**********************************************************
* rand_coloumn
***********************************************************/
int display_jpeg11(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	srand(time(NULL));
	int i, j;
	int times;
	char a[2560]={0};
       
	for(times = 0; times < 10000; times++)
	{
		j = rand()%fb_inf.w;
    
		if(a[j] == 0)
		{
			a[j]=1;
      
			for (i = 0; i < fb_inf.h; i++)
			{
				draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);
			}

			my_delay(0xffff);
		}
	
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/*********************************************************
* up_to_down
**********************************************************/
int display_jpeg12(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for(i = 0; i < fb_inf.h; ++i)
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
		my_delay(0xaffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/***********************************************************
* down_to_up
************************************************************/
int display_jpeg13(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for(i = fb_inf.h - 1; i >= 0; --i)
	{
		for (j = fb_inf.w - 1;j >= 0; --j)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
		my_delay(0xaffff);

	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/**********************************************************
* left_to_right
***********************************************************/
int display_jpeg14(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for (j = 0; j < fb_inf.w; ++j)
	{
		for(i = 0; i < fb_inf.h; ++i)
		{
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
		my_delay(0xffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/************************************************************
* up/down open
*************************************************************/
int display_jpeg15(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i,j;
	int i1 = fb_inf.h/2 -1;
	
	for(i = 0; i < fb_inf.h/2; ++i)	
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			draw_pixel(fb_inf, j, i1, buf32[j + i1 * fb_inf.w]);
			draw_pixel(fb_inf, j, i + fb_inf.h/2, buf32[j + (i + fb_inf.h/2) * fb_inf.w]);
		}
		
		i1--;
		
		my_delay(0xfffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/****************************************************************
* left/right open
*****************************************************************/
int display_jpeg16(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i,j;
	int i1 = fb_inf.w/2 - 1;
	
	for (j = 0; j < fb_inf.w/2; ++j)
	{
		for(i = 0; i < fb_inf.h; ++i)	
		{
			draw_pixel(fb_inf, i1, i, buf32[i1 + i * fb_inf.w]);
			draw_pixel(fb_inf, j + fb_inf.w/2, i, buf32[j + fb_inf.w/2 + i * fb_inf.w]);
		}
		
		i1--;
		
		my_delay(0xfffff);
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/****************************************************************
* up/down close
*****************************************************************/
int display_jpeg17(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i,j;
	int i1 = fb_inf.h - 1;
	
	for(i = 0; i < fb_inf.h/2; ++i)	
	{
		for (j = 0; j < fb_inf.w; ++j)
		{
			draw_pixel(fb_inf, j, i1, buf32[j + i1 * fb_inf.w]);
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
		i1--;
		
		my_delay(0xfffff);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/********************************************************
* left/right close
*********************************************************/
int display_jpeg18(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i,j;
	int i1 = fb_inf.w - 1;
	
	for (j = 0; j < fb_inf.w/2; ++j)
	{
		for(i = 0; i < fb_inf.h; ++i)	
		{
			draw_pixel(fb_inf, i1, i, buf32[i1 + i * fb_inf.w]);
			draw_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
		
		i1--;
		
		my_delay(0xfffff);
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/**********************************************************
* he zi
***********************************************************/
int display_jpeg20(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;

	for(j = 0;j < fb_inf.w;j++)
	{
		for(i = 0;i < fb_inf.h/2;i++)
		{		
			draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);	
			draw_pixel(fb_inf,fb_inf.w - j,fb_inf.h/2 + i,buf32[fb_inf.w - j + (fb_inf.h/2 + i) * fb_inf.w]);		
		}		
	
		my_delay(100);
	}		
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/************************************************************
* left to right step 2 then right to left
*************************************************************/
int display_jpeg21(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	for(j = 0; j < fb_inf.w; j += 2)
	{
		for(i = 0; i < fb_inf.h; i++)
		{	
			draw_pixel(fb_inf,j,i,buf32[j + i * fb_inf.w]);		
		}	
		
		my_delay(0xffff);
	}		
	
	for(j = 0; j < fb_inf.w; j++)
	{
		for(i = 0; i < fb_inf.h; i++)
		{
			draw_pixel(fb_inf,fb_inf.w - j,i,buf32[fb_inf.w - j + i * fb_inf.w]);
		}
				
		my_delay(0xfffff);
	}
				
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

/**************************************************
* play pictures
***************************************************/
void play_pic(fb_info fb_inf)
{	
	FILE *pFile;
	int k ;

	srand(time(NULL));

	if((pFile = fopen("list/piclist.txt","r+")) != NULL)
	{
		while(!feof(pFile))
		{		
			char path[128] = "image/";
			char pic_name[128];

			fgets(pic_name,sizeof(pic_name),pFile);
			strncat(path,pic_name,strlen(pic_name)-1);		

			k = (rand() % 24) + 1;
			
			switch(k)
			{
				case 1:
					display_jpeg1(path, fb_inf);
			  	break;
				
				case 2:
					display_jpeg2(path, fb_inf);
			  	break;	
		
				case 3:
				 	display_jpeg3(path, fb_inf);
			  	break;	
		
				case 4:
					display_jpeg4(path, fb_inf);
			  	break;	
		
				case 5:
					display_jpeg5(path, fb_inf);
			  	break;	
			
				case 6:
					display_jpeg6(path, fb_inf);
			  	break;	
			
				case 7:
					display_jpeg7(path, fb_inf);
			  	break;	
			
				case 8:
					display_jpeg8(path, fb_inf);
			  	break;	
		
				case 9:
					display_jpeg9(path, fb_inf);
			  	break;	
			
				case 10:
					display_jpeg10(path, fb_inf);
			 	break;	
				
				case 11:
					display_jpeg11(path, fb_inf);
			  	break;
			
				case 12:
					display_jpeg12(path, fb_inf);
			  	break;
			
				case 13:
					display_jpeg13(path, fb_inf);
			  	break;
			
				case 14:
					display_jpeg14(path, fb_inf);
			  	break;
			
				case 15:
					display_jpeg15(path, fb_inf);
			  	break;
			
				case 16:
					display_jpeg16(path, fb_inf);
			  	break;
			
				case 17:
					display_jpeg17(path, fb_inf);
			  	break;
			
				case 18:
					display_jpeg18(path, fb_inf);
			  	break;
			
				case 19:
					display_jpeg19(path, fb_inf);
			  	break;
			
				case 20:
					display_jpeg20(path, fb_inf);
			  	break;
				
				case 21:
					display_jpeg21(path, fb_inf);
			  	break;
			  	
				case 22:
					display_jpeg22(path, fb_inf);
			  	break;
			
				case 23:
					display_jpeg23(path, fb_inf);
			  	break;
			
				case 24:
					display_jpeg24(path, fb_inf);
			  	break;
			
			}						
			
			if(feof(pFile))
				fseek(pFile,0,SEEK_SET);
			
			sleep(1);
		}

	fclose(pFile);	
	}  
}
