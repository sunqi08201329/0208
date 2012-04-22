
src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

CFLAGS = -Wall -g
CC = gcc
CLEAN_FLAGS = -rm -rf
MACRO = 

all:$(target) tags

$(target):%:%.c
	$(CC) $(CFLAGS) -L../libmad-0.15.0b/.libs -lmad $< -o $@
tags:
	ctags -R *
clean:
	$(CLEAN_FLAGS) $(target)
.PHONY:all clean
