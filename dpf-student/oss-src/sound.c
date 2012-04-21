
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
 

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


