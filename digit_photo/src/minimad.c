#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <sys/types.h>
#include "common.h"
#include "mad.h"

int soundfd;

void set_dsp()
{
	int rate = 44100; /* 采样频率 44.1KHz*/
	int format = AFMT_S16_LE; /* 量化位数 16*/
	int channels = 2; /* 声道数 2*/

	soundfd = open ( "/dev/dsp", O_RDWR);
	ioctl(soundfd, SNDCTL_DSP_SPEED, &rate);
	ioctl(soundfd, SNDCTL_DSP_SETFMT, &format);
	ioctl(soundfd, SNDCTL_DSP_CHANNELS, &channels);
#if 0 
	unsigned char buf[5*rate*format*channels/8];
	printf("say\n");
	read(soundfd,buf,sizeof(buf));
	printf("said\n");
	write(soundfd,buf,sizeof(buf));
	printf("music\n");
#endif
}

void set_vol(int vol)
{
	int soundvol = open("/dev/mixer",O_RDWR);
	ioctl(soundvol,MIXER_WRITE(SOUND_MIXER_VOLUME),&vol);
	close(soundvol);
}

struct buffer {
	unsigned char const *start;
	unsigned long length;
};

	static
enum mad_flow input(void *data,
		struct mad_stream *stream)
{
	struct buffer *buffer = data;

	if (!buffer->length)
		return MAD_FLOW_STOP;

	mad_stream_buffer(stream, buffer->start, buffer->length);

	buffer->length = 0;

	return MAD_FLOW_CONTINUE;
}

	static inline
signed int scale(mad_fixed_t sample)
{
	/* round */
	sample += (1L << (MAD_F_FRACBITS - 16));

	/* clip */
	if (sample >= MAD_F_ONE)
		sample = MAD_F_ONE - 1;
	else if (sample < -MAD_F_ONE)
		sample = -MAD_F_ONE;

	/* quantize */
	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

	static
enum mad_flow output(void *data,
		struct mad_header const *header,
		struct mad_pcm *pcm)
{
	unsigned int nchannels, nsamples;
	mad_fixed_t const *left_ch, *right_ch;

	/* pcm->samplerate contains the sampling frequency */

	nchannels = pcm->channels;
	nsamples  = pcm->length;
	left_ch   = pcm->samples[0];
	right_ch  = pcm->samples[1];

	while (nsamples--) {
		signed int sample;
		short buf;

		/* output sample(s) in 16-bit signed little-endian PCM */
		sample = scale(*left_ch++);

		buf = sample & 0xFFFF;
		write(soundfd, &buf, 2);

		if (nchannels == 2) {
			sample = scale(*right_ch++);
			buf = sample & 0xFFFF;
			write(soundfd, &buf, 2);
		}
	}

	return MAD_FLOW_CONTINUE;
}


	static
enum mad_flow error(void *data,
		struct mad_stream *stream,
		struct mad_frame *frame)
{
	/*  struct buffer *buffer = data;

	    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	    stream->error, mad_stream_errorstr(stream),
	    stream->this_frame - buffer->start);
	 */
	/* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

	return MAD_FLOW_CONTINUE;
}

	static
int decode(unsigned char const *start, unsigned long length)
{
	struct buffer buffer;
	struct mad_decoder decoder;
	int result;

	/* initialize our private message structure */

	buffer.start  = start;
	buffer.length = length;

	/* configure input, output, and error functions */

	mad_decoder_init(&decoder, &buffer,
			input, 0 /* header */, 0 /* filter */, output,
			error, 0 /* message */);

	/* start decoding */

	result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	/* release the decoder */

	mad_decoder_finish(&decoder);

	return result;
}
/*
   int exit_sound(int fd)
   {
   ioctl(fd, SNDCTL_DSP_SYNC); 
   ioctl(fd, SNDCTL_DSP_RESET);
   close(fd);
   return 0;
   }
 */
int play_mp3(char *mp3_name)
{
	struct stat stat;
	void *fdm;

		set_dsp();
	//	set_vol();

	int fd = open(mp3_name,O_RDONLY);

	//  if (fstat(STDIN_FILENO, &stat) == -1 ||
	if (fstat(fd, &stat) == -1 ||
			stat.st_size == 0)
		return 2;

	fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fdm == MAP_FAILED)
		return 3;

	decode(fdm, stat.st_size);

	if (munmap(fdm, stat.st_size) == -1)
		return 4;

	//	exit_sound(fd);
	
	close(soundfd);
	close(fd);

	return 0;
}

void mp3(int num,fb_info fb_inf)
{
	FILE *pFile;
	int i;
	char name[64];

	if((pFile = fopen("list/mp3list.txt","r+")) != NULL)
	{
		for(i = 0; i < num; i++)
			fgets(name,64,pFile);

		char path[128] = "music/";
		char mp3_name[128];

		fgets(mp3_name,sizeof(mp3_name),pFile);
		strncat(path,mp3_name,strlen(mp3_name)-1);

		if(strstr(path,".mp3") != NULL)
		{
			play_mp3(path);
		}
	}

	fclose(pFile);
}
