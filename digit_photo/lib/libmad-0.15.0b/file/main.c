#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE * fsrc,* fdest;int c;

	fsrc = fopen((char *)argv[1], "r");
	fdest = fopen((char *)argv[2], "w");
	while((c = fgetc(fsrc)) != EOF)
	{
		printf("%s","aaa");
		fputc(c,fdest);
	}
	fclose(fsrc);
	fclose(fdest);

	return 0;
}
