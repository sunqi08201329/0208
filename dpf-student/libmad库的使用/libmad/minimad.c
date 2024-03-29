/*
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2003 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: minimad.c,v 1.2 2003/06/05 02:27:07 rob Exp $
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/soundcard.h>


# include "mad.h"

#define SOUND_DEVICE_NAME "/dev/dsp"

/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.
 */
static int fd_dsp;
static int init_sound(const char *device_name, int *fd);
static int exit_sound(int fd);
static int decode(unsigned char const *, unsigned long, int);

int main(int argc, char *argv[])
{
#if 0
	if(argc < 2){
		fprintf(stderr, "Please usage: %s <name.mp3>\n", argv[0]);
		return 1;
	}
#else
	if(argv[1] == NULL)
		argv[1] = "1.mp3";

#endif
	int fd_dev_sound;
	if(init_sound(SOUND_DEVICE_NAME, &fd_dev_sound) != 0){
		fprintf(stderr, "Error initial sound device!\n");
		return 1;
	}
	int fd_mp3;
	if((fd_mp3 = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "Open %s:%s\n", argv[1], strerror(errno));
		return 1;
	}

	struct stat mp3fd_stat;
	void *fdm;
	//int fstat(int fd, struct stat *buf);
	if((fstat(fd_mp3, &mp3fd_stat) == -1) || (mp3fd_stat.st_size == 0)){
		fprintf(stderr, "Error fstat file %s size %d\n", argv[1],(int)mp3fd_stat.st_size);
		return 1;
	}
	//void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	fdm = mmap(0, mp3fd_stat.st_size, PROT_READ, MAP_SHARED, fd_mp3, 0);

	if(fdm == NULL){
		fprintf(stderr, "mmap file %s:%s\n", argv[1], strerror(errno));
		exit(1);
	}

	//mp3fd_static int decode(unsigned char const *, unsigned long, int);
	decode(fdm, mp3fd_stat.st_size, fd_dev_sound); 

	//int munmap(void *addr, size_t length);
	munmap(fdm, mp3fd_stat.st_size);  
	exit_sound(fd_dev_sound);

	return 0;
}

/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */

struct buffer {
	unsigned char const *start;
	unsigned long length;
};

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

static enum mad_flow input(void *data, struct mad_stream *stream)
{
	struct buffer *buffer = data;

	if (!buffer->length)
		return MAD_FLOW_STOP;

	mad_stream_buffer(stream, buffer->start, buffer->length);

	buffer->length = 0;

	return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline signed int scale(mad_fixed_t sample)
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

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
	unsigned int nchannels, nsamples;
	mad_fixed_t const *left_ch, *right_ch;

	/* pcm->samplerate contains the sampling frequency */

	nchannels = pcm->channels;
	nsamples  = pcm->length;
	left_ch   = pcm->samples[0];
	right_ch  = pcm->samples[1];
#if 0
	while (nsamples--) {
		signed int sample;

		/* output sample(s) in 16-bit signed little-endian PCM */

		sample = scale(*left_ch++);
		putchar((sample >> 0) & 0xff);
		putchar((sample >> 8) & 0xff);

		if (nchannels == 2) {
			sample = scale(*right_ch++);
			putchar((sample >> 0) & 0xff);
			putchar((sample >> 8) & 0xff);
		}
	}
#else
	while (nsamples--) {
		signed int sample;
		short buf;

		/* output sample(s) in 16-bit signed little-endian PCM */

		sample = scale(*left_ch++);

		buf = sample & 0XFFFF;
		write(fd_dsp, &buf, 2);

		if (nchannels == 2) {
			sample = scale(*right_ch++);
			buf = sample & 0xFFFF;
			write(fd_dsp, &buf, 2);
		}
	}

#endif

	return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or
 * libmad/stream.h) header file.
 */

static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	struct buffer *buffer = data;

	//fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	//stream->error, mad_stream_errorstr(stream),
	//stream->this_frame - buffer->start);

	/* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

	return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the
 * decoding. It instantiates a decoder object and configures it with the
 * input, output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

static int decode(unsigned char const *start, unsigned long length, int fd)
{
	struct buffer buffer;
	struct mad_decoder decoder;
	int result;

	fd_dsp = fd;
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
int init_sound(const char *device_name, int *fd)
{
	if ((*fd = open(device_name, O_WRONLY)) < 0){
		fprintf(stderr, "Error opening %s: %s\n", device_name, strerror(errno));
		return -1;
	}

	int value;

	value = 16;			  /* 16 bits by sample */
	if ((ioctl(*fd, SNDCTL_DSP_SAMPLESIZE, &value) < 0) || value != 16){
		fprintf(stderr, "Error setting SNDCTL_DSP_SAMPLESIZE \n");
		return -1;
	}

	value = 0;			  /* Mono */
	if ((ioctl(*fd, SNDCTL_DSP_STEREO, &value) < 0) || value != 0){
		fprintf(stderr, "Error setting SNDCTL_DSP_STEREO\n");
		return -1;
	}

	value = 96000;			  /* Sample Rate */
	if ((ioctl(*fd, SNDCTL_DSP_SPEED, &value) < 0) || value != 96000){
		fprintf(stderr, "Error setting SNDCTL_DSP_SPEED\n");
		return -1;
	}

	return 0;
}

int exit_sound(int fd)
{
	ioctl(fd, SNDCTL_DSP_SYNC); 
	ioctl(fd, SNDCTL_DSP_RESET);
	close(fd);

	return 0;
}
