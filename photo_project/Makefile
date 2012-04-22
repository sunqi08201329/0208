src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

CFLAGS = -Wall -g
CC = gcc
CLEAN_FLAGS = -rm -rf
MACRO = 

INC_PATH = ./include/
LIB_PATH = ./libs/photo_lib/
lib_name = photo_project 

LD_HEAS = -I./libs/jpeg-8/ -I./libs/freetype-2.3.11/include -I./libs/libmad-0.15.0b/ -I$(INC_PATH)
LD_LIBS = -L./libs/jpeg-8/.libs/ -ljpeg -L./libs/freetype-2.3.11/objs/.libs -lfreetype -L./libs/libmad-0.15.0b/.libs -lmad -L$(LIB_PATH) -l$(lib_name)
#LD_LIB_JPEG = -I/home/akaedu/github/0208/photo_project/libs/jpeg-8/.libs:
#LD_LIB_FREETYPE = $LD_LIBRARY_PATH:/home/akaedu/github/0208/photo_project/libs/freetype-2.3.11/objs/.libs


all:$(target) tags

$(target):%:%.c
	$(CC) $(CFLAGS) $< $(LD_LIBS) -o $@ $(LD_HEAS) 
tags:
	ctags -R *
clean:
	$(CLEAN_FLAGS) $(target)

makelib:
	make -C $(LIB_PATH)

vimlib:
	vim $(LIB_PATH)*.c

viminc:
	vim $(INC_PATH)*.h
.PHONY:all clean makelib makelib vimlib viminc
