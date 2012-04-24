#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, const char *argv[])
{
	//func_t f[] = {a, b, c};
	srand(time(NULL));
	f[rand()%3]();
	return 0;
}
void a(void)
{
	printf("a\n");
}
void b(void)
{
	printf("b\n");
}
void c(void)
{
	printf("c\n");
}
