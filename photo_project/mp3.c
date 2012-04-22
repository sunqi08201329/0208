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


//#include "mad.h"
#include "music.h"


/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.
 */

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
