#ifndef __MAIN_H__
#define __MAIN_H__

extern int init_sound(const char *device_name, int *fd);
extern int exit_sound(int fd);
extern int decode(unsigned char const *start, unsigned long length, int fd);



#endif
