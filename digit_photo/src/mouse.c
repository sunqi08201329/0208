#include "common.h"

#define SCREEN_W fb_inf.w/1024
#define SCREEN_H fb_inf.h/768
#define MOUSE 0x0000CD

int SaveColor[40][40];
int vol = 90;
int play_flag = 1;
int num = 0;
int start = 1;
int state = 0;
int mp3_page = 0;

/*****************************************************
 *save mouse
 *****************************************************/
int save_mouse(fb_info fb_inf ,int size,int row,int col)
{
	int i,j;

	for(i = 0;i < size;i++)
	{
		for(j = 0;j < size;j++)
		{
			if((2 * i >= j) && (2 * j >= i))
			{
				SaveColor[i][j] = ((int *)fb_inf.fbmem)[fb_inf.w * (col + j) + (row + i)];
			}
			
		}
		
	}

	return 0;
}

/****************************************************
 *draw mouse
 *******************************************************/
int draw_mouse(fb_info fb_inf,int size,int row,int col)
{
	int i,j;

	for(i = 0;i < size;i++)
	{
		for(j = 0; j < size;j++)
		{
			if((2 * i >= j) && (2 * j >= i))
			{
				draw_pixel(fb_inf,row + i, col + j, MOUSE);
			}
			
		}
		
	}

	return 0;
}

/****************************************************
 *restore mouse
 ******************************************************/
int restore_mouse(fb_info fb_inf,int size,int row,int col)
{
	int i,j;
	
	for(i = 0;i < size;i++)
	{
		for(j = 0;j < size;j++)
		{
			if((2 * i >= j) && (2 * j >= i))
			{
				draw_pixel(fb_inf,row + i, col + j,SaveColor[i][j]);
			}
			
		}
		
	}

	return 0;
}

/***********************************************************
*welcome menu
***********************************************************/
int welcome_menu(fb_info fb_inf,pid_t pid[2],int flag[5],int row,int col,char filename[4][20])
{
	if((row > 188 * SCREEN_W)&&(row < 256 * SCREEN_W)&&(col > 700 * SCREEN_H - 32 * SCREEN_W)&&(col < 700 * SCREEN_H))
	{
		pid[0] = fork();
	
		if (pid[0] == 0)
		{
			display_one("background/view.jpg",fb_inf);
			sleep(1);

			while(1)			
				play_pic(fb_inf);
		}	
		state = 1;
		flag[0]=0;
		flag[3]=1;
	}else if((row > 60 * SCREEN_W) && (row < 124 * SCREEN_W) && (col > 700 * SCREEN_H - 32 * SCREEN_W) && (col < 700 * SCREEN_H))
		{
			view_pic(fb_inf,filename);
			
			flag[0]=0;
			flag[1]=1;
		}else if((row > 316 * SCREEN_W) && (row < 380 * SCREEN_W) && (col > 700 * SCREEN_H - 32 * SCREEN_W) && (col < 700 * SCREEN_H))
			{
				display_one("background/music.jpg",fb_inf);
				
				play_list(fb_inf,mp3_page);
				
				play_fun(fb_inf);
				
				if(play_flag == 2)
					display_string("循环",610 * fb_inf.w/1024,722 * fb_inf.h/768,fb_inf,0xff0000);
				if(play_flag == 3)
					display_string("随机",750 * fb_inf.w/1024,720 * fb_inf.h/768,fb_inf,0xff0000);
				
				if(start == 1)
					display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
				else
					display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
	
				flag[0] = 0;
				flag[4] = 1;
				
			}else if((row > 444 * SCREEN_W) && (row < 504 * SCREEN_W) && (col > 700*SCREEN_H - 32*SCREEN_W) && (col < 700 * SCREEN_H))
				{
					goodbye(fb_inf);
					kill(pid[1],SIGTERM);
					wait(NULL);
					
					exit(0);
				}		
		
	return 0;
}

/*****************************************************
* music menu
******************************************************/
void music_menu(fb_info fb_inf,pid_t pid[2],int flag[5],int row,int col,int music_count)
{
	char name[64] = "music/";
	char mp3_name[64];
	int mode = 0;
	int size = 32 * fb_inf.w/1024;
	int x_start = 200 * fb_inf.w/1024;
	int y_start = 120 * fb_inf.h/768; 
	int i,count;
	
	if(music_count % 8 != 0)
		count = music_count/8;
	else
	{
		count = music_count/8;
		count--;	
	}
	
	FILE *pFile;
	
	srand(time(NULL));
	
	if((row >= 340*fb_inf.w/1024)&&(row <= 380*fb_inf.w/1024) && (col >= 590*fb_inf.h/768) &&(col <= 640*fb_inf.h/768))
	{
		display_string("<",355 * fb_inf.w/1024,620 * fb_inf.h/768,fb_inf,COLOR);
		display_string(">",410 * fb_inf.w/1024,610 * fb_inf.h/768,fb_inf,COLOR);
		if(mp3_page > 0)
		{
			mp3_page--;
			display_block("background/music.jpg",fb_inf);
			play_list(fb_inf,mp3_page);
			display_string("<",355 * fb_inf.w/1024,620 * fb_inf.h/768,fb_inf,0xff0000);
		}
	}else if((row >= 400*fb_inf.w/1024)&&(row <= 435*fb_inf.w/1024) && (col >= 590*fb_inf.h/768) &&(col <= 640*fb_inf.h/768))
	{
		display_string("<",355 * fb_inf.w/1024,620 * fb_inf.h/768,fb_inf,COLOR);
		display_string(">",410 * fb_inf.w/1024,610 * fb_inf.h/768,fb_inf,COLOR);
		if(mp3_page < count)
		{
			mp3_page++;
			display_block("background/music.jpg",fb_inf);
			play_list(fb_inf,mp3_page);
			display_string(">",410 * fb_inf.w/1024,610 * fb_inf.h/768,fb_inf,0xff0000);
		}
	}

	if((row >= 285*fb_inf.w/1024)&&(row <= 285*fb_inf.w/1024 + 3*size) && (col >= 725*fb_inf.h/768 - size) &&(col <= 725*fb_inf.h/768))
	{
		if(start == 0)
		{				
			kill(pid[1],SIGCONT);
			start = 1;
		}
		
		mode = 1;
		play_flag = 1;
		num--;

		if(num < 0)
			num = music_count - 1;
		play_list(fb_inf,mp3_page);
		play_fun(fb_inf);
	
		if(start == 1)
			display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
		else
			display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);	

		display_string("上一首",285 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);			


	}else if((row >= 422 * fb_inf.w/1024)&&(row <= 422 * fb_inf.w/1024 + 3 * size)&&(col >= 725 * fb_inf.h/768 - size) &&(col <= 725 * fb_inf.h/768))
		{
			if(start == 0)
			{				
				kill(pid[1],SIGCONT);
				start = 1;
			}

			mode = 1;
			play_flag = 1;
			num++;
		
			if(num > music_count - 1)
				num = 0;
		
			play_list(fb_inf,mp3_page);
			play_fun(fb_inf);
				
			if(start == 1)
				display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
			else
				display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);	
				display_string("下一首",422 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
	}
	
	if((row >= 610 * fb_inf.w/1024)&&(row <= 610 * fb_inf.w/1024 + 2 * size)&&(col >= 722 * fb_inf.h/768 - size) &&(col <= 722 * fb_inf.h/768))
	{
		if(start == 0)
		{				
			kill(pid[1],SIGCONT);
			start = 1;
		}

		mode = 1;		
		play_flag = 2;
		
		play_list(fb_inf,mp3_page);
		play_fun(fb_inf);
		
		if(start == 1)
			display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
		else
			display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);	
			display_string("循环",610 * fb_inf.w/1024,722 * fb_inf.h/768,fb_inf,0xff0000);
	}else if((row >= 750 * fb_inf.w/1024)&&(row <= 750 * fb_inf.w/1024 + 2 * size) && (col >= 720 * fb_inf.h/768 - size) &&(col <= 720 * fb_inf.h/768))
		{
			if(start == 0)
			{				
				kill(pid[1],SIGCONT);
				start = 1;
			}
			
			mode = 1;
			play_flag = 3;
			
			play_list(fb_inf,mp3_page);
			play_fun(fb_inf);
		
			if(start == 1)
				display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
			else
				display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
				display_string("随机",750 * fb_inf.w/1024,720 * fb_inf.h/768,fb_inf,0xff0000);
		}		

	if((row >= 15 * fb_inf.w/1024)&&(row <= 15 * fb_inf.w/1024 + 3 * size)&&(col >= 725 * fb_inf.h/768 - size) &&(col <= 725 * fb_inf.h/768))	
	{
		play_fun(fb_inf);
		display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);

		if(play_flag == 2)
			display_string("循环",610 * fb_inf.w/1024,722 * fb_inf.h/768,fb_inf,0xff0000);
		if(play_flag == 3)
			display_string("随机",750 * fb_inf.w/1024,720 * fb_inf.h/768,fb_inf,0xff0000);
		if(start == 0)
		{
			kill(pid[1],SIGCONT);
			start = 1;

			return;	
		}
		
	}else if((row >= 140 * fb_inf.w/1024)&&(row <= 140 * fb_inf.w/1024 + 3 * size)&&(col >= 725 * fb_inf.h/768 - size) && (col <= 725 * fb_inf.h/768))	
		{
			play_fun(fb_inf);
			
			display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);

			if(play_flag == 2)
				display_string("循环",610 * fb_inf.w/1024,722 * fb_inf.h/768,fb_inf,0xff0000);
			if(play_flag == 3)
				display_string("随机",750 * fb_inf.w/1024,720 * fb_inf.h/768,fb_inf,0xff0000);
			if(start == 1)
			{
				kill(pid[1],SIGSTOP);
				start = 0;
		
				return;
			}
			
		}

	if((row >= 45 * fb_inf.w/1024)&&(row <= 80 * fb_inf.w/1024)&&(col >= 595*fb_inf.h/768)&&(col <= 650*fb_inf.h/768))
	{
		display_string("+",105 * fb_inf.w/1024,605 * fb_inf.h/768,fb_inf,COLOR);
		if(vol > 0)
		{
			vol -= 10;
			
			set_vol(vol);
			display_string("-",55 * fb_inf.w/1024,630 * fb_inf.h/768,fb_inf,0xff0000);
			
			return;
		}

	}else if((row >= 90 * fb_inf.w/1024)&&(row <= 135 * fb_inf.w/1024)&&(col >= 585 * fb_inf.h/768)&&(col <= 635 * fb_inf.h/768))
	{
		display_string("-",55 * fb_inf.w/1024,630 * fb_inf.h/768,fb_inf,COLOR);		
		
		if(vol < 100)
		{
			vol += 10;
			
			set_vol(vol);
			display_string("+",105 * fb_inf.w/1024,605 * fb_inf.h/768,fb_inf,0xff0000);			
			
			return;
		}
		
	}

	if((pFile = fopen("list/mp3list.txt","r+")) != NULL)
	{
		for(i = 0; i < 8 * mp3_page; i++)
		{
			fgets(mp3_name,64,pFile);
		}

		int max = 8;
			
		if(mp3_page == count)
			max = music_count % 8;

		for(i = 1; i <= max; i++)
		{		
			fgets(mp3_name,64,pFile);
			
			if((row >= x_start)&&(row <= x_start + strlen(mp3_name)/3 * size)&&(col >= y_start - size)&&(col <= y_start))
			{
				strncat(name,mp3_name,strlen(mp3_name)-1);
				num = mp3_page * 8 + i - 1;
				
				play_list(fb_inf,mp3_page);
				play_fun(fb_inf);
				
				if(start == 0)
				{				
					kill(pid[1],SIGCONT);
					start = 1;
				}
				
				if(start == 1)
					display_string("播放",15 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);
				else
					display_string("暂停",140 * fb_inf.w/1024,725 * fb_inf.h/768,fb_inf,0xff0000);	
				
				mp3_name[strlen(mp3_name) - 5] = '\0';
				
				display_string(mp3_name,x_start,y_start,fb_inf,0xff0000);
				
				fclose(pFile);
				
				mode = 1;
				play_flag = 1;
			
				break;
			}
			
			y_start = y_start + 20*fb_inf.h/768 + size;		
			
		}
		
	}	
		
	if(mode == 1)
	{
		kill(pid[1],SIGTERM);
		wait(NULL);
		pid[1] = fork();
		
		if(pid[1] == 0)
		{
			while(play_flag == 1)
			{
				mp3(num,fb_inf);
			}
						
			play_list(fb_inf,mp3_page);	
			
			while(play_flag == 2)
			{				
				while(num < music_count)
				{
					mp3(num,fb_inf);
					num++;
					
					if(num == music_count)
						num = 0;
				}				
			}
			
			while(play_flag == 3)
			{
				num = rand()%(music_count + 1);
				
				mp3(num,fb_inf);
			}			
		}							
	}
}

/*****************************************************
*view_menu
*****************************************************/
int view_menu(fb_info fb_inf,int flag[5],int row,int col, char filename[4][20],int *page,int count)
{
	if(((row >= 10 * SCREEN_W)&&(col >= 10 * SCREEN_H)&&(row < 502 * SCREEN_W)&&(col < 344 * SCREEN_H)&&(strstr(filename[0],".jpg") != NULL)))
	{	    
		display_jpeg(filename[0],fb_inf);
		flag[1] = 0;
		flag[2] = 1;
	}else if((strstr(filename[1],".jpg") != NULL)&&(row >= 522 * SCREEN_W)&&(col >= 10 * SCREEN_H) && (row < 1014 * SCREEN_W) && (col < 344 * SCREEN_H))
		{
			display_jpeg(filename[1],fb_inf);
			flag[1] = 0;
			flag[2] = 1;
		}else if((strstr(filename[2],".jpg") != NULL)&&(row >= 10 * SCREEN_W)&&(col >= 364 * SCREEN_H)&&(row < 502 * SCREEN_W)&&(col < fb_inf.h - 70 * SCREEN_H))
		{
			display_jpeg(filename[2],fb_inf);
			flag[1] = 0;
			flag[2] = 1;
		}else if((strstr(filename[3],".jpg") != NULL)&&(row >= 522 * SCREEN_W)&&(col >= 364 * SCREEN_H)&&(row < 1014 * SCREEN_W)&&(col < 698 * SCREEN_H))
			{
				display_jpeg(filename[3],fb_inf);
				flag[1] = 0;
				flag[2] = 1;
			}else if((row >= 208 * SCREEN_W)&&(col >= 720*SCREEN_H)&&(row < 304 * SCREEN_W)&&( col < 748 * SCREEN_H))
				{
					if(*page > 0)
					{
						*page = *page - 1;
						get_pic_name(filename,*page);
						view_pic(fb_inf,filename);
					}
				}else if((row >= 720 * SCREEN_W)&&(col >= 720 * SCREEN_H)&&(row < 816 * SCREEN_W)&&(col < 748 * SCREEN_H))
					{
						if(*page < count)
						{
							*page = *page + 1;
							
							get_pic_name(filename,*page);
							view_pic(fb_inf,filename);
						}
					}

	return 0;
}

/******************************************************
*pic_menu
*********************************************************/
int pic_menu(fb_info fb_inf,int flag[5],int row,int col,int page,char filename[4][20])
{
	view_pic(fb_inf,filename);

	flag[1] = 1;
	flag[2] = 0;

	return 0;
}

/*******************************************************
 *mouse
 **********************************************************/
int mouse(fb_info fb_inf)
{
	int size = 16 * SCREEN_W;
	int row = fb_inf.w/2;
	int col = fb_inf.h/2;
	int mice_fd = init_mouse();	
	int flag[5] = {1,0,0,0,0};	
	int page = 0;
	char buf[5],filename[4][20]; 
	pid_t pid[2];
	
	set_vol(vol);

	create_list("music/","list/mp3list.txt",".mp3");
	create_list("image/","list/piclist.txt",".jpg");

	FILE *pFile = fopen("list/piclist.txt","r+");
	int count = -1;
	char str[64];

	while(!feof(pFile))
	{
		fgets(str,sizeof(str),pFile);
		count++;
	}
	
	if(count % 4 == 0)
		count -= 1;
	
	count /= 4;	
	fclose(pFile);

	pFile = fopen("list/mp3list.txt","r+");
	int music_count = -1;

	while(!feof(pFile))
	{
		fgets(str,sizeof(str),pFile);
		music_count++;
	}
	
	fclose(pFile);

	get_pic_name(filename,0);

	welcome(fb_inf);

	pid[1] = fork();
	
	if(pid[1] == 0)
		while(1)
			mp3(num,fb_inf);
		
	save_mouse(fb_inf,size,row,col);
	draw_mouse(fb_inf,size,row,col);

	while(1)
	{		
		read(mice_fd,buf,3);

		if(flag[3] == 0)
			restore_mouse(fb_inf,size,row,col);
				
		if (((row + buf[1]) >= 0)&&(row + buf[1]) < fb_inf.w - size)
		{
			row += buf[1];
		}

		if(((col-buf[2]) >= 0)&&(col-buf[2]) < fb_inf.h - size)
		{
			col -= buf[2];
		}
		
		mouse_come(fb_inf,row,col,flag);
		mouse_leave(fb_inf,row,col,flag);

		if((buf[0]&1) == 1)	
		{
			if(flag[3] == 1)
			{
				if(state == 1)
				{				
					kill(pid[0],SIGSTOP);
					state = 0;
				}else if(state == 0)
				{
					kill(pid[0],SIGCONT);			
					state = 1;
				}
			}
			
			if(flag[1] == 1)
			{
				view_menu(fb_inf,flag,row,col,filename,&page,count);
			}else if((flag[2] == 1))
				{
					pic_menu(fb_inf,flag,row,col,page,filename);
				}else if(flag[4] == 1)
					{
						music_menu(fb_inf,pid,flag,row,col,music_count);
					}		
								
			if(flag[0] == 1)
			{
				welcome_menu(fb_inf,pid,flag,row,col,filename);
			}
					
		}
		
		
		
		if(((buf[0]&3) == 2)&&(flag[0] == 0)&&(flag[2] == 0))
		{	
			if (flag[3] == 1)
			{
				if(state == 0)
					kill(pid[0],SIGCONT);
				kill(pid[0],SIGTERM);
				state = 0;
				wait(NULL);
			}
					
			play_welcome(fb_inf);

			flag[0] = 1;
			flag[1] = 0;
			flag[3] = 0;
			flag[4] = 0;
		}
				
		save_mouse(fb_inf,size,row,col);

		if(flag[3] == 0)
			draw_mouse(fb_inf,size,row,col);
	}

	return 0;
}
