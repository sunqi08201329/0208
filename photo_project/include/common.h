#ifndef __COMMON__H__
#define __COMMON__H__

#define PHOTO_PATH "./src/photo/"
#define MUSIC_PATH "./src/music/"
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;

typedef struct 
{
	int w;				  /* width */
	int h;				  /* high */
	int bpp;				  /* bits per pixel */
	u8_t *fbmem;
}fb_info;

/* ******************** fb.c ******************** */

/* initial framebuf */
extern int init_fb(fb_info *fb_inf);

/* drawing pixel */
extern int fb_pixel(fb_info fb_inf, int x, int y, u32_t color);

/* Framebuffer Test */
int fb_test(void);


/* ******************** jpeg.c ******************** */

/*This returns an array for a 24 bit image.*/
extern u8_t *decode_jpeg (const char *filename, fb_info *jpeg_inf);

extern u32_t * rgb24to32(u8_t *buf24, fb_info jpeg_inf);
extern u16_t * rgb24to16(u8_t *buf24, fb_info jpeg_inf);

extern u8_t * scale24(u8_t *buf24, fb_info new_inf, fb_info jpeg_inf);


/* ******************** disp_jpeg.c ******************** */
/* display jpeg */
extern int display_jpeg(char *filename, fb_info fb_inf);
extern int display_jpeg1(char *filename, fb_info fb_inf);
extern int display_jpeg_recangle(char *filename, fb_info fb_inf, int sx, int sy, int sizeofstr, int len_of_aword);


/* ******************** font.c ******************** */
extern int init_ft (const char *file, int size);
extern int display_string (const char *buf, int x, int y, fb_info fb_inf,u32_t color);


/* ******************** disp-basic.c ******************** */
/* create jpeg middle data file */
extern int create_jpeg(const char *filejpeg, const char *newfile, fb_info fb_inf);
/* use middle data file to display */
extern int basic_disp(const char * filename, fb_info fb_inf);


#endif
