#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

char photo_path[32] = "./src/photo/";
char font_path[32] = "./src/font/";
char music_path[32] = "./src/music/";

/* main function */
int main(int argc, char *argv[]) 
{

	if (argc < 2)
		argv[1] = strcat(photo_path, "test.jpg");

	fb_info fb_inf;
	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}
	int pid;		
	if((pid = fork()) < 0){
		fprintf(stderr, "fork error %s\n", strerror(errno));
		exit(1);
	}else if(pid == 0){
	int j = 6;
		while(j){

			display_jpeg("./src/photo/test.jpg", fb_inf);

			//while(1);

#if 1
			/* Test mouse */
			//	test_mouse(fb_inf);

			int wordsize = 50;

			if (init_ft("./src/font/mao.ttf", wordsize) != 0){
				fprintf(stderr, "Error initial font\n")	;
				return 1;
			}

			int i;
			char str[1024] = "您好 AKAEDU！";
			for (i = 100; i <= fb_inf.w; i++) 
			{
				display_string(str, i*wordsize, fb_inf.h/2, fb_inf, 0xFFFF00);
				usleep(1000*1000);
				display_jpeg_recangle("./src/photo/test.jpg", fb_inf, i*wordsize, fb_inf.h/2, strlen(str), wordsize);
				if(time(NULL)%5 ==0)
					break;
			}
			display_jpeg1("./src/photo/test3.jpg", fb_inf);
			j--;
#endif
		}
	}
	else{
		int status;
		if((pid = waitpid(-1, &status, WNOHANG)) > 0)
			exit(1);
		system("./mp3 1.mp3");
	}

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}

