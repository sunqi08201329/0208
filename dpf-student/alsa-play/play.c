/*
Example 3 - Simple sound playback

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.
*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

#define MAX 80

extern int minimad(int fd_mp3, FILE *fp_tmp);


int init_alsa(snd_pcm_t *handle,int sample,int channel,int format )
{

	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	int rc;
	snd_pcm_uframes_t frames;
 
	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(handle, params);


	/* Interleaved mode */
	snd_pcm_hw_params_set_access(handle, params,
				     SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params,
				     format);

	/* Two channels (stereo) */
	snd_pcm_hw_params_set_channels(handle, params, channel);

	/* 44100 bits/second sampling rate (CD quality) */
	val = sample;
	snd_pcm_hw_params_set_rate_near(handle, params, 
					&val, &dir);

	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle,params, &frames,
					       &dir);

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(handle, params);
	if (rc < 0) {
		fprintf(stderr, 
			"unable to set hw parameters: %s\n",
			snd_strerror(rc));
		exit(1);
	}

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(params, &frames,
					  &dir);

	return frames;

}

int main (int argc, char *argv[]) 
{
	int rc;
	int size;
	snd_pcm_t *handle;
	char *buffer;
	int sample ,channel ,format;
	snd_pcm_uframes_t frames;

	if (argc < 2){
		fprintf(stderr, "Please input: %s <mp3_file>\n",
			argv[0]);
		return 0;
	}
	int fd_mp3 = open(argv[1], O_RDONLY); /* open mp3 file */
	if (fd_mp3 < 0){
		fprintf(stderr, "Error for open %s: %s\n", argv[1],
			strerror(errno));
		return 1;
	}

	/* Create temporary file  */
	FILE *fp_tmp = tmpfile();

	/* 对mp3文件解码， 结果存放于 临时文件中中 */
	if (minimad(fd_mp3, fp_tmp) != 0){
		printf("Decode Error \n");
		exit(1);
	}
	rewind(fp_tmp);
	
	close(fd_mp3);
	
	int fd_tmp = fp_tmp->_fileno;
	

	/* Open PCM device for playback. */
	rc = snd_pcm_open(&handle, "default", 
			  SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0) {
		fprintf(stderr, 
			"unable to open pcm device: %s\n",
			snd_strerror(rc));
		exit(1);
	}

	sample  = 44100;
	channel = 2;
	format  = SND_PCM_FORMAT_S16_LE;  

	frames = init_alsa(handle,sample,channel,format);

	size = frames * 4; /* 2 bytes/sample, 2 channels */
	buffer = (char *) malloc(size);


	while (1) {
		rc = read(fd_tmp, buffer, size);

		if (rc == 0) {
			fprintf(stderr, "end of file on input\n");
			break;
		}
 
		rc = snd_pcm_writei(handle, buffer, size/4);
		if (rc != (int)size/4) {
			fprintf(stderr, 
				"short write, write %d frames\n", rc);
		}
	}

	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	free(buffer);
	fclose(fp_tmp);
	
	return 0;
}

