 /*********************************************************
 *                                                         *
 *                                                         *
 * main.c -  Digital Photo Frame                           *
 *                                                         *
 * Author:   Group 8                                       *
 *                                                         *
 * Date:     2010-10-13                                    *
 *                                                         *
 *                                Version: 3.0             *
 **********************************************************/
#include "common.h"

int main(int argc,char *argv[])
{
	
	fb_info fb_inf;
/*
	pthread_t nid;
	pthread_create(&nid,NULL,mp3,NULL);
*/
	if(init_fb(&fb_inf) < 0)
	{
		fprintf(stderr,"init error\n");
		exit(0);
	}

	int size = 32*fb_inf.w/1024;
	init_ft("./font/type.ttf",size);
	
	mouse(fb_inf);

	munmap(fb_inf.fbmem,fb_inf.w*fb_inf.h*fb_inf.bpp/8);

	return 0;
}
