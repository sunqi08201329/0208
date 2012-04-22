src = $(wildcard *.c)
objs = $(patsubst %.c, %.o, $(src))

CFLAGS = -Wall -g
CC = gcc
CLEAN_FLAGS = -rm -rf
STATIC_LINK = ar rcs

INC_PATH = ../../include/
LIB_PATH = ./

CFLAGS = -I../jpeg-8/ -I../freetype-2.3.11/include -I../libmad-0.15.0b/
#LDFLAGS = -L../jpeg-8/.libs/ -ljpeg -L../freetype-2.3.11/objs/.libs -lfreetype
lib_name = photo_project

all:$(objs) link clean 
$(objs):%.o:%.c 
	$(CC) -c $< -o $@ -I$(INC_PATH)  $(CFLAGS)

link:
	$(STATIC_LINK) lib$(lib_name).a $(objs)
	$(CC) -shared -o lib$(lib_name).so $(objs)	
	#$(CC) main.c -L$(LIBRARY_PATH) -l $(lib_name) -o main -I$(INCLUDE_PATH)

clean:
	-rm -rf *.o 

.PHONY:all link clean
	
