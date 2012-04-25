/*
 *  2010.4.5
 *  mouse.h
 */

#ifndef __MOUSE_H__
#define __MOUSE_H__

typedef struct 
{
	int x;
	int y;
	int button;
}mouse_event_t;
		
int mouse_open(char *device_name, int *fd);
int mouse_parse(const u8_t *buf, mouse_event_t* mevent);

int fb_restorecursor(fb_info fb_inf, int x, int y);
int fb_drawcursor(fb_info fb_inf, int x, int y);

int display_jpeg_view (char *filename, fb_info fb_inf, int k);

int test_mouse(fb_info fb_inf);

#endif

