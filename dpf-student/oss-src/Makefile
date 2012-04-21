# Makefile V0.1 2010.02.20
# Author:	Panda.Guo
# Description:	Makefile for mp3


src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = mp3

CC = -gcc
CFLAGS = -Wall -g -c -I./
LDFLAGS = -L./ -lmad -static


all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o:%.c
	$(CC) -o $@ $< $(CFLAGS) 

clean:
	-rm -f $(target) $(obj)
distclean: clean
	-rm -f *~ *.out

.PHONY: clean clean distclean

