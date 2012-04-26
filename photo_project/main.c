#include "common.h"
#include "disp_jpg.h"
#include "mouse.h"
#include "music.h"
#include "filename_list.h"

#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//#define PHOTO_PATH "./src/photo/"

//typedef int (*disp_jpeg)(char *, fb_info);


//void handler(int signo);

/* main function */
int main(int argc, char *argv[]) 
{

	fb_info fb_inf;

	if (init_fb(&fb_inf) < 0){
		fprintf(stderr, "Error initial framebuffer\n");
		return 1;
	}
//****************************photo
	char **file_names = NULL;
	file_names = struct_a_array(file_names);

	//file_list *read_file_name(char * path, file_list *list);
	file_list *photo_list = NULL;
	photo_list = read_file_name(PHOTO_PATH, photo_list);
	int photo_num ;
	print_list(photo_list);

	//char **jose_sort(char **file_names, file_list *list);
	photo_num = jose_sort(file_names, photo_list);
//* ***************************font */
	char **font_names = NULL;
	font_names = struct_a_array(font_names);

	//file_list *read_file_name(char * path, file_list *list);
	file_list *font_list = NULL;
	font_list = read_file_name(FONT_PATH, font_list);
	int font_num ;
	print_list(font_list);

	//char **jose_sort(char **file_names, file_list *list);
	font_num = jose_sort(font_names, font_list);
//* **************************************** */

	int pid;		
	if((pid = fork()) < 0){
		fprintf(stderr, "fork error %s\n", strerror(errno));
		exit(1);
	}else if(pid == 0){
		//pid = waitpid(-1, &status, WNOHANG);

		//raise(SIGINT);
		int j = 0;
		int wordsize = 50;
		while(1){
srand(time(NULL));
			//while(1);

#if 1
			/* Test mouse */
			//	test_mouse(fb_inf);

			wordsize = 50;

			if (init_ft("./src/font/DroidSansFallback.ttf", wordsize) != 0){// "./src/font/DroidSansFallback.ttf",
				fprintf(stderr, "Error initial font\n")	;
				return 1;
			}
			char str[1024] = " ♪ ♫ ▶ ◀ ☺ ☻ ▣ ";
			display_string(str, 10, fb_inf.h/2, fb_inf, 0xFFFFFF);
			//disp_jpeg disp_jpeg_func[] = {display_jpeg, display_jpeg1};
			disp_jpeg_func[rand()%6](file_names[j], fb_inf);
			display_string(file_names[1], 100, 100, fb_inf, 0xFFFF00);
			

			if (init_ft(font_names[6], wordsize) != 0){// "./src/font/DroidSansFallback.ttf",
				fprintf(stderr, "Error initial font\n")	;
				return 1;
			}

			wordsize = 20;
			int i;
			char str1[1024] = "hello akaedu!";
			for (i = 100; i <= fb_inf.w; i++) 
			{
				display_string(str1, i*wordsize, fb_inf.h/2, fb_inf, 0xFFFFFF);
				usleep(1000*1000);
				display_jpeg_recangle(file_names[j], fb_inf, i*wordsize, fb_inf.h/2, strlen(str), wordsize);
				if(time(NULL)%5 ==0)
					break;
			}
			disp_jpeg_func[rand()%6](file_names[rand()%photo_num], fb_inf);
			display_string(file_names[rand()%photo_num], 100, 100, fb_inf, 0xFFFF00);
			j++;
			if(j == photo_num)
				j = 0;
#endif
		}
				display_string("3s 后结束thanks", 100, fb_inf.h/2, fb_inf, 0xFFFF00);
		sleep(3);
		//void free_list(file_list * list);
		//free_list(photo_list);
		//char **struct_a_array(char **file_names);
		destruct_array(file_names);
	}
	else{
		int pid1 = fork();
		//signal(SIGCHLD, handler);
		if(pid1 == 0)
			test_music("./src/music/1.mp3");
		else{	
			//int test_mouse(fb_info fb_inf);
			int status;
			printf("pid1 %d\n", pid1);
			printf("pid %d\n", pid);
			printf("getpid %d\n", getpid());

			while(waitpid(-1, &status, WNOHANG) > 0)
			{
				//kill(0, SIGINT);
				//raise(SIGINT);
			}
			test_mouse(fb_inf);
		}

	}

	munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);

	return 0;
}
//void handler(int signo)
//{
//int status;
//waitpid(-1, &status, WNOHANG);
////printf("child %d died, parent will die\n",pid);
//return;
//////////}
