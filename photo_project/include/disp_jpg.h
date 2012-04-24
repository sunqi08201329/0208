#ifndef __display__jpg__
#define  __display__jpg__

typedef int (*disp_jpeg)(char *, fb_info);

int display_jpeg(char *filename, fb_info fb_inf);
int display_jpeg1(char *filename, fb_info fb_inf);
int display_jpeg_byc_utod (char *filename, fb_info fb_inf);
int display_jpeg_byc_dtou (char *filename, fb_info fb_inf);
int display_jpeg_byc_ltor (char *filename, fb_info fb_inf);
int display_jpeg_byc_rtol (char *filename, fb_info fb_inf);

disp_jpeg disp_jpeg_func[] = {display_jpeg, display_jpeg1, display_jpeg_byc_utod, display_jpeg_byc_dtou, display_jpeg_byc_ltor, display_jpeg_byc_rtol};

int display_jpeg_recangle(char *filename, fb_info fb_inf, int sx, int sy, int sizeofstr, int len_of_aword);
#endif  //__display__jpg__
