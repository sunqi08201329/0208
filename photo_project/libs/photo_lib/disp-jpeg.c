#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"
#include "disp_jpg.h"

#define DEBUG 0
#if DEBUG
#endif

//disp_jpeg disp_jpeg_func[] = {display_jpeg, display_jpeg1};

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

int display_jpeg_byc_utod (char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg (filename, &jpeg_inf);
	u8_t *scale_buf = scale24 (buf24, fb_inf, jpeg_inf);	/* 缩放 */
	u32_t *buf32 = rgb24to32 (scale_buf, fb_inf);

	int i,j,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;

	m1=fb_inf.h/16;
	m2=fb_inf.h/8;
	m3=fb_inf.h/8+fb_inf.h/16;
	m4=fb_inf.h/4;
	m5=fb_inf.h/4+fb_inf.h/16;
	m6=fb_inf.h/4+fb_inf.h/8;
	m7=fb_inf.h/4+fb_inf.h/8+fb_inf.h/16;
	m8=fb_inf.h/2;
	m9=fb_inf.h/2+fb_inf.h/16;
	m10=fb_inf.h/2+fb_inf.h/8;
	m11=fb_inf.h/2+fb_inf.h/8+fb_inf.h/16;
	m12=fb_inf.h/2+fb_inf.h/4;
	m13=fb_inf.h/2+fb_inf.h/4+fb_inf.h/16;
	m14=fb_inf.h/2+fb_inf.h/4+fb_inf.h/8;
	m15=fb_inf.h/2+fb_inf.h/4+fb_inf.h/8+fb_inf.h/16-1;

	for(j = 0;j<=fb_inf.h/16;j++, m1++,m2++,m3++,m4++,m5++,\
			m6++,m7++,m8++,m9++,m10++,m11++,m12++,m13++,m14++,m15++)
	{
		for(i = 0;i < fb_inf.w; i++)
		{
			fb_pixel(fb_inf,i,j,buf32[i+j*fb_inf.w]);
			fb_pixel(fb_inf,i,m1,buf32[i+m1*fb_inf.w]);
			fb_pixel(fb_inf,i,m2,buf32[i+m2*fb_inf.w]);
			fb_pixel(fb_inf,i,m3,buf32[i+m3*fb_inf.w]);
			fb_pixel(fb_inf,i,m4,buf32[i+m4*fb_inf.w]);
			fb_pixel(fb_inf,i,m5,buf32[i+m5*fb_inf.w]);
			fb_pixel(fb_inf,i,m6,buf32[i+m6*fb_inf.w]);
			fb_pixel(fb_inf,i,m7,buf32[i+m7*fb_inf.w]);
			fb_pixel(fb_inf,i,m8,buf32[i+m8*fb_inf.w]);
			fb_pixel(fb_inf,i,m9,buf32[i+m9*fb_inf.w]);
			fb_pixel(fb_inf,i,m10,buf32[i+m10*fb_inf.w]);
			fb_pixel(fb_inf,i,m11,buf32[i+m11*fb_inf.w]);
			fb_pixel(fb_inf,i,m12,buf32[i+m12*fb_inf.w]);
			fb_pixel(fb_inf,i,m13,buf32[i+m13*fb_inf.w]);
			fb_pixel(fb_inf,i,m14,buf32[i+m14*fb_inf.w]);
			fb_pixel(fb_inf,i,m15,buf32[i+m15*fb_inf.w]);
		}
		usleep(20000);
	}



	free(buf24);
	free(scale_buf);
	free (buf32);

	return 0;
}

// bai ye chuang down to up
int display_jpeg_byc_dtou (char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg (filename, &jpeg_inf);
	u8_t *scale_buf = scale24 (buf24, fb_inf, jpeg_inf);	/* 缩放 */
	u32_t *buf32 = rgb24to32 (scale_buf, fb_inf);

	int i,j,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;
	int m15_s;
	m1=fb_inf.h/16+1;
	m2=fb_inf.h/8;
	m3=fb_inf.h/8+fb_inf.h/16;
	m4=fb_inf.h/4;
	m5=fb_inf.h/4+fb_inf.h/16;
	m6=fb_inf.h/4+fb_inf.h/8;
	m7=fb_inf.h/4+fb_inf.h/8+fb_inf.h/16;
	m8=fb_inf.h/2;
	m9=fb_inf.h/2+fb_inf.h/16;
	m10=fb_inf.h/2+fb_inf.h/8;
	m11=fb_inf.h/2+fb_inf.h/8+fb_inf.h/16;
	m12=fb_inf.h/2+fb_inf.h/4;
	m13=fb_inf.h/2+fb_inf.h/4+fb_inf.h/16;
	m14=fb_inf.h/2+fb_inf.h/4+fb_inf.h/8;
	m15=fb_inf.h/2+fb_inf.h/4+fb_inf.h/8+fb_inf.h/16;
	m15_s=m15;
	for(j = fb_inf.h-1;j >= m15_s;j--,m1--,m2--,m3--,m4--,m5--,\
			m6--,m7--,m8--,m9--,m10--,m11--,m12--,m13--,m14--,m15--)
	{
		for(i = 0;i < fb_inf.w; i++)
		{
			fb_pixel(fb_inf,i,j,buf32[i+j*fb_inf.w]);
			fb_pixel(fb_inf,i,m1,buf32[i+m1*fb_inf.w]);
			fb_pixel(fb_inf,i,m2,buf32[i+m2*fb_inf.w]);
			fb_pixel(fb_inf,i,m3,buf32[i+m3*fb_inf.w]);
			fb_pixel(fb_inf,i,m4,buf32[i+m4*fb_inf.w]);
			fb_pixel(fb_inf,i,m5,buf32[i+m5*fb_inf.w]);
			fb_pixel(fb_inf,i,m6,buf32[i+m6*fb_inf.w]);
			fb_pixel(fb_inf,i,m7,buf32[i+m7*fb_inf.w]);
			fb_pixel(fb_inf,i,m8,buf32[i+m8*fb_inf.w]);
			fb_pixel(fb_inf,i,m9,buf32[i+m9*fb_inf.w]);
			fb_pixel(fb_inf,i,m10,buf32[i+m10*fb_inf.w]);
			fb_pixel(fb_inf,i,m11,buf32[i+m11*fb_inf.w]);
			fb_pixel(fb_inf,i,m12,buf32[i+m12*fb_inf.w]);
			fb_pixel(fb_inf,i,m13,buf32[i+m13*fb_inf.w]);
			fb_pixel(fb_inf,i,m14,buf32[i+m14*fb_inf.w]);
			fb_pixel(fb_inf,i,m15,buf32[i+m15*fb_inf.w]);
		}
		usleep(20000);
	}



	free(buf24);
	free(scale_buf);
	free (buf32);

	return 0;
}


// bai ye chuang left to right
int display_jpeg_byc_ltor (char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg (filename, &jpeg_inf);
	u8_t *scale_buf = scale24 (buf24, fb_inf, jpeg_inf);	/* 缩放 */
	u32_t *buf32 = rgb24to32 (scale_buf, fb_inf);

	int i,j,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;

	m1=fb_inf.w/16;
	m2=fb_inf.w/8;
	m3=fb_inf.w/8+fb_inf.w/16;
	m4=fb_inf.w/4;
	m5=fb_inf.w/4+fb_inf.w/16;
	m6=fb_inf.w/4+fb_inf.w/8;
	m7=fb_inf.w/4+fb_inf.w/8+fb_inf.w/16;
	m8=fb_inf.w/2;
	m9=fb_inf.w/2+fb_inf.w/16;
	m10=fb_inf.w/2+fb_inf.w/8;
	m11=fb_inf.w/2+fb_inf.w/8+fb_inf.w/16;
	m12=fb_inf.w/2+fb_inf.w/4;
	m13=fb_inf.w/2+fb_inf.w/4+fb_inf.w/16;
	m14=fb_inf.w/2+fb_inf.w/4+fb_inf.w/8;
	m15=fb_inf.w/2+fb_inf.w/4+fb_inf.w/8+fb_inf.w/16-1;

	for(i = 0;i<=fb_inf.w/16;i++, m1++,m2++,m3++,m4++,m5++,\
			m6++,m7++,m8++,m9++,m10++,m11++,m12++,m13++,m14++,m15++)
	{
		for(j = 0;j < fb_inf.h; j++)

		{
			fb_pixel(fb_inf,i,j,buf32[i+j*fb_inf.w]);
			fb_pixel(fb_inf,m1,j,buf32[m1+j*fb_inf.w]);
			fb_pixel(fb_inf,m2,j,buf32[m2+j*fb_inf.w]);
			fb_pixel(fb_inf,m3,j,buf32[m3+j*fb_inf.w]);
			fb_pixel(fb_inf,m4,j,buf32[m4+j*fb_inf.w]);
			fb_pixel(fb_inf,m5,j,buf32[m5+j*fb_inf.w]);
			fb_pixel(fb_inf,m6,j,buf32[m6+j*fb_inf.w]);
			fb_pixel(fb_inf,m7,j,buf32[m7+j*fb_inf.w]);
			fb_pixel(fb_inf,m8,j,buf32[m8+j*fb_inf.w]);
			fb_pixel(fb_inf,m9,j,buf32[m9+j*fb_inf.w]);
			fb_pixel(fb_inf,m10,j,buf32[m10+j*fb_inf.w]);
			fb_pixel(fb_inf,m11,j,buf32[m11+j*fb_inf.w]);
			fb_pixel(fb_inf,m12,j,buf32[m12+j*fb_inf.w]);
			fb_pixel(fb_inf,m13,j,buf32[m13+j*fb_inf.w]);
			fb_pixel(fb_inf,m14,j,buf32[m14+j*fb_inf.w]);
			fb_pixel(fb_inf,m15,j,buf32[m15+j*fb_inf.w]);
		}
		usleep(20000);
	}



	free(buf24);
	free(scale_buf);
	free (buf32);

	return 0;
}


// bai ye chuang right to left
int display_jpeg_byc_rtol (char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;

	u8_t *buf24 = decode_jpeg (filename, &jpeg_inf);
	u8_t *scale_buf = scale24 (buf24, fb_inf, jpeg_inf);	/* 缩放 */
	u32_t *buf32 = rgb24to32 (scale_buf, fb_inf);

	int i,j,m1,m2,m3,m4,m5,m6,m7,m8,m9,m10,m11,m12,m13,m14,m15;
	int m15_s;
	m1=fb_inf.w/16+1;
	m2=fb_inf.w/8;
	m3=fb_inf.w/8+fb_inf.w/16;
	m4=fb_inf.w/4;
	m5=fb_inf.w/4+fb_inf.w/16;
	m6=fb_inf.w/4+fb_inf.w/8;
	m7=fb_inf.w/4+fb_inf.w/8+fb_inf.w/16;
	m8=fb_inf.w/2;
	m9=fb_inf.w/2+fb_inf.w/16;
	m10=fb_inf.w/2+fb_inf.w/8;
	m11=fb_inf.w/2+fb_inf.w/8+fb_inf.w/16;
	m12=fb_inf.w/2+fb_inf.w/4;
	m13=fb_inf.w/2+fb_inf.w/4+fb_inf.w/16;
	m14=fb_inf.w/2+fb_inf.w/4+fb_inf.w/8;
	m15=fb_inf.w/2+fb_inf.w/4+fb_inf.w/8+fb_inf.w/16;
	m15_s=m15;
	for(i = fb_inf.w-1;i>=m15_s;i--,m1--,m2--,m3--,m4--,m5--,\
			m6--,m7--,m8--,m9--,m10--,m11--,m12--,m13--,m14--,m15--)
	{
		for(j = 0;j < fb_inf.h; j++)

		{
			fb_pixel(fb_inf,i,j,buf32[i+j*fb_inf.w]);
			fb_pixel(fb_inf,m1,j,buf32[m1+j*fb_inf.w]);
			fb_pixel(fb_inf,m2,j,buf32[m2+j*fb_inf.w]);
			fb_pixel(fb_inf,m3,j,buf32[m3+j*fb_inf.w]);
			fb_pixel(fb_inf,m4,j,buf32[m4+j*fb_inf.w]);
			fb_pixel(fb_inf,m5,j,buf32[m5+j*fb_inf.w]);
			fb_pixel(fb_inf,m6,j,buf32[m6+j*fb_inf.w]);
			fb_pixel(fb_inf,m7,j,buf32[m7+j*fb_inf.w]);
			fb_pixel(fb_inf,m8,j,buf32[m8+j*fb_inf.w]);
			fb_pixel(fb_inf,m9,j,buf32[m9+j*fb_inf.w]);
			fb_pixel(fb_inf,m10,j,buf32[m10+j*fb_inf.w]);
			fb_pixel(fb_inf,m11,j,buf32[m11+j*fb_inf.w]);
			fb_pixel(fb_inf,m12,j,buf32[m12+j*fb_inf.w]);
			fb_pixel(fb_inf,m13,j,buf32[m13+j*fb_inf.w]);
			fb_pixel(fb_inf,m14,j,buf32[m14+j*fb_inf.w]);
			fb_pixel(fb_inf,m15,j,buf32[m15+j*fb_inf.w]);
		}
		usleep(20000);
	}



	free(buf24);
	free(scale_buf);
	free (buf32);

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
		for (y = sy - len_of_aword - 20; y <= ey + 20; ++y){
			fb_pixel(fb_inf, x, y, buf32[x + y * fb_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}

