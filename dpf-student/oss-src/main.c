
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "main.h"

#define SOUND_DEVICE_NAME "/dev/dsp"

int main(int argc, char **argv)
{
#if 1
	if(argc < 2){
		fprintf(stderr, "Please usage: %s <name.mp3>\n", argv[0]);
		return 1;
	}
#else
	if(argv[1] == NULL)
		argv[1] = "1.mp3";
	
#endif

	int fd;
	if (init_sound(SOUND_DEVICE_NAME, &fd) != 0){
		fprintf(stderr, "Error initial sound device!\n");
		return 1;
	}

	int fd_mp3;
	if((fd_mp3 = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "Open %s:%s\n", argv[1], strerror(errno));
		return 1;
	}
	
	struct stat stat;
	void *fdm;
	if((fstat(fd_mp3, &stat) == -1) || (stat.st_size == 0)){
		fprintf(stderr, "Error stat file %s\n", argv[1]);
		return 1;
	}

	fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd_mp3, 0);
	if (fdm == NULL){
		fprintf(stderr, "mmap file %s:%s\n", argv[1], strerror(errno));
		exit(1);
	}

	decode(fdm, stat.st_size, fd);

#if 0	
	short sample;
	while (!feof(stdin))
	{
		sample =  fgetc(stdin) + (fgetc(stdin) << 8);
		if ((-1 == write(fd,&sample,2)) && errno == EINTR) {
			fprintf(stderr,"Error writing to %s\n", SOUND_DEVICE_NAME);
			return 1;
		}
	}
#endif
	
	munmap(fdm, stat.st_size);
	exit_sound(fd);
	
	return 0;
}
