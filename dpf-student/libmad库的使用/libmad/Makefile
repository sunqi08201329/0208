
src = $(wildcard *.c)
target = $(patsubst %.c, %, $(src))

CFLAGS = -Wall -g
CC = gcc
CLEAN_FLAGS = -rm -rf
MACRO = 

all:$(target) tags

$(target):%:%.c
	$(CC) $(CFLAGS) -lmad $< -o $@
tags:
	ctags -R *
clean:
	$(CLEAN_FLAGS) $(target)
.PHONY:all clean
