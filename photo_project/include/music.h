#ifndef __MUSIC__H__
#define __MUSIC__H__ 

#include "mad.h"

#define SOUND_DEVICE_NAME "/dev/dsp"

struct buffer {
	unsigned char const *start;
	unsigned long length;
};

int init_sound(const char *device_name, int *fd);
int exit_sound(int fd);
int decode(unsigned char const *, unsigned long, int);
enum mad_flow input(void *data, struct mad_stream *stream);
inline signed int scale(mad_fixed_t sample);
enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm);
enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame);

int test_music(char *musicname);

#endif // __MUSIC__H__
