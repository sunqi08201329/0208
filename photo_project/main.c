#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

/* main function */
int main(int argc, char *argv[]) 
{

	if (argc < 2)
		argv[1] = "test.jpg";

	fb_info fb_inf;
	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}
	
	display_jpeg(argv[1], fb_inf);
//	system("./mp3 2.mp3");
	
//while(1);
	
#if 1
	/* Test mouse */
//	test_mouse(fb_inf);

	int wordsize = 50;

	if (init_ft("mao.ttf", wordsize) != 0){
		fprintf(stderr, "Error initial font\n")	;
		return 1;
	}
	
	int i;
	char str[1024] = "您好 AKAEDU！";
	for (i = 100; i <= fb_inf.w; i++) 
	{
		//display_jpeg1(argv[1], fb_inf);
		//int display_jpeg_recangle(char *filename, fb_info fb_inf, int sx, int sy, int sizeofstr, int len_of_aword)
		//display_jpeg_recangle(argv[1], fb_inf, i*wordsize, fb_inf.h/2, strlen(str), wordsize);
		//sleep(1);
		display_string(str, i*wordsize, fb_inf.h/2, fb_inf, 0xFFFF00);
		usleep(1000*1000);
		display_jpeg_recangle(argv[1], fb_inf, i*wordsize, fb_inf.h/2, strlen(str), wordsize);
		//display_jpeg_recangle(argv[1], fb_inf, i*wordsize, fb_inf.h/2, 1, 1);
		if(time(NULL)%5 ==0)
			break;
	}
	display_jpeg1(argv[2], fb_inf);
#endif

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}

