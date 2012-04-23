
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"

#define DEBUG 0
#if DEBUG
#endif

int display_jpeg1(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);//24bit color
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int x, y;
	for(x = 0; x < fb_inf.w; ++x){
		for (y = 0; y < fb_inf.h; ++y){
			fb_pixel(fb_inf, x, y, buf32[x + y * fb_inf.w]);
		}
		usleep(2*1000);
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int display_jpeg(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);//24bit color
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int x, y;
	for (y = 0; y < fb_inf.h; ++y){
		for(x = 0; x < fb_inf.w; ++x){
			fb_pixel(fb_inf, x, y, buf32[x + y * fb_inf.w]);
		}
		usleep(2*1000);
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int display_jpeg_recangle(char *filename, fb_info fb_inf, int sx, int sy, int sizeofstr, int len_of_aword)
{

	fb_info jpeg_inf;
	int ex = sx+sizeofstr*len_of_aword;
	int ey = sy;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);//24bit color
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);
	int x, y;
	for(x = sx; x < ex; ++x){
		for (y = sy - len_of_aword - 1; y <= ey + 10; ++y){
			fb_pixel(fb_inf, x, y, buf32[x + y * fb_inf.w]);
		}
	}
	
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

