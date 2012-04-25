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

	char **file_names = NULL;
	file_names = struct_a_array(file_names);

	//file_list *read_file_name(char * path, file_list *list);
	file_list *photo_list = NULL;
	photo_list = read_file_name(PHOTO_PATH, photo_list);
	int photo_num ;
	print_list(photo_list);

	//char **jose_sort(char **file_names, file_list *list);
	photo_num = jose_sort(file_names, photo_list);


	int pid;		
	if((pid = fork()) < 0){
		fprintf(stderr, "fork error %s\n", strerror(errno));
		exit(1);
	}else if(pid == 0){
		//pid = waitpid(-1, &status, WNOHANG);

				raise(SIGINT);
		int j = 0;
		while(1){
srand(time(NULL));
			//while(1);

#if 1
			/* Test mouse */
			//	test_mouse(fb_inf);

			int wordsize = 50;

			if (init_ft("./src/font/mao.ttf", wordsize) != 0){
				fprintf(stderr, "Error initial font\n")	;
				return 1;
			}

			//disp_jpeg disp_jpeg_func[] = {display_jpeg, display_jpeg1};
			disp_jpeg_func[rand()%6](file_names[j], fb_inf);
			display_string(file_names[1], 100, 100, fb_inf, 0xFFFF00);


			int i;
			char str[1024] = "您好 AKAEDU！";
			for (i = 100; i <= fb_inf.w; i++) 
			{
				display_string(str, i*wordsize, fb_inf.h/2, fb_inf, 0xFFFF00);
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
