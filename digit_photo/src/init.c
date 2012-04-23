#include "common.h"

#define FB_DEVICE "/dev/fb0"
#define MOUSE_DEVICE "/dev/input/mice"

/********************************************************
 * init framebuffer
 ********************************************************/
int init_fb(fb_info *fb_inf)
{
	int fd;

	if((fd = open(FB_DEVICE,O_RDWR)) < 0)
	{
		fprintf(stderr,"error open %s,%s\n",FB_DEVICE,strerror(errno));
		exit(0);
	}

	struct fb_var_screeninfo fb_var;

	if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var) < 0)
	{
		fprintf(stderr,"error ioctl,%s\n",strerror(errno));
		exit(1);
	}

	fb_inf->w = fb_var.xres;
	fb_inf->h = fb_var.yres;
	fb_inf->bpp = fb_var.bits_per_pixel;
	int length = fb_inf->w * fb_inf->h * fb_inf->bpp / 8;

	//	printf("%d,%d,%d\n",fb_inf->w,fb_inf->h,fb_inf->bpp);

	fb_inf->fbmem = mmap(0,length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	if(fb_inf->fbmem == MAP_FAILED)
	{
		fprintf(stderr,"error mmap,%s\n",strerror(errno));
		exit(2);
	}

	close(fd);

	return 0;
}

int init_mouse(void)
{
	int fd = open(MOUSE_DEVICE,O_RDWR);

	return fd;
}
