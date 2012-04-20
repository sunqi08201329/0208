#include <sys/mman.h>
#include <stdio.h>
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
	
#if 0
	/* Test mouse */
//	test_mouse(fb_inf);

	if (init_ft("mao.ttf", 0) != 0){
		fprintf(stderr, "Error initial font\n")	;
		return 1;
	}
	
	display_string("显示文字", 32, 400, fb_inf, 0xFF0000);
#endif

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}

