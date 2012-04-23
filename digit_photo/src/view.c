#include "common.h"

/**************************************************
* show
**************************************************/
int show(char *filename, fb_info fb_inf,int k)
{
	fb_info jpeg_inf;
	fb_inf.h = fb_inf.h/2 ;
	fb_inf.w = fb_inf.w/2 ;

	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t *scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j;
	
	fb_inf.h = fb_inf.h  * 2;
	fb_inf.w = fb_inf.w  * 2;

	if(k < 2)
		for(i = 0 + 10 * fb_inf.w/1024; i < fb_inf.h/2 - 10 * fb_inf.w/1024 - 30 * fb_inf.w/1024; i++)
		{
			for (j = fb_inf.w * k/2 + 10 * fb_inf.w/1024; j < fb_inf.w/2 + fb_inf.w * k /2 - 10 * fb_inf.w/1024; j++)
			{
				draw_pixel(fb_inf, j, i, buf32[j - (fb_inf.w * k/2 + 10 * fb_inf.w/1024) + (i - 10 * fb_inf.w/1024) * (fb_inf.w/2)]);
			}
		
		}
	else
    for(i = fb_inf.h/2+10*fb_inf.w/1024-30 * fb_inf.w/1024 ; i < fb_inf.h-10 * fb_inf.w/1024-60 * fb_inf.w/1024 ; i++)
    {
			for (j = fb_inf.w * (k - 2)/2 + 10 * fb_inf.w/1024 ; j < fb_inf.w/2 + fb_inf.w * (k - 2)/2 - 10 * fb_inf.w/1024; j++)
			{
				draw_pixel(fb_inf, j, i, buf32[j-(fb_inf.w * (k - 2)/2 + 10 * fb_inf.w/1024) + (i - (fb_inf.h/2 + 10 * fb_inf.w/1024-30 * fb_inf.w/1024)) * (fb_inf.w/2)]);
			}
			
		}

	free(buf24);
	free(scale_buf);
	free(buf32);

	return 0;
}

/***********************************************************
* view pic
************************************************************/
void view_pic(fb_info fb_inf, char name[4][20])
{
	display_one("background/view.jpg",fb_inf);
    
	int size = 32 * fb_inf.w/1024;
	
	if(init_ft("font/type.ttf", size) != 0)
	{
		fprintf(stderr, "Error initial font\n");
		return;
	}

	int x = fb_inf.w/4 - size * 3/2;
	int y = fb_inf.h - 20 * fb_inf.h/768;

	display_string("上一页", x, y, fb_inf, COLOR);
	display_string("下一页", x + fb_inf.w/2, y, fb_inf, COLOR);

	int i;
   
	for(i = 0; i < 4; i++)
	{
		char path[128]="image/";
		strncat(path,name[i],strlen(name[i]) - 1);
		if(strstr(path,".jpg") != NULL)
			show(path, fb_inf, i);
	}  
	
}

/***************************************************
* get pic name
****************************************************/
int get_pic_name(char filename[4][20],int page)
{
	FILE * pFile = fopen("list/piclist.txt","r+");
	
	if(pFile == NULL)
	{
		printf("open file error\n");
		exit(0);
	}
	
	int i;
	
	for(i = 0; i <= 4 * page;i++)
		fgets(filename[0],20,pFile);
		fgets(filename[1],20,pFile);
	
	if(feof(pFile))
		filename[1][0] = '\0';	
		fgets(filename[2],20,pFile);

	if(feof(pFile))
		filename[2][0] = '\0';
		fgets(filename[3],20,pFile);
	
	if(feof(pFile))
		filename[3][0] = '\0';
	
	fclose(pFile);

	return 0;
}

/************************************************
* create list
*************************************************/
int create_list(const char *pathname,char *listname,char *type)
{
	DIR *dp;
	
	if((dp = opendir(pathname)) == NULL)
		return -1;
	
	struct dirent *sdir;

	FILE *pFile = fopen(listname,"w+");

	while((sdir = readdir(dp)) != NULL)
	{
		if((sdir->d_type == DT_REG) && (strstr(sdir->d_name,type) != NULL))
		{
			fputs(sdir->d_name,pFile);
			fputs("\n",pFile);
		}
		
	}
	
	fclose(pFile);
	closedir(dp);
	return 0;
	
}

