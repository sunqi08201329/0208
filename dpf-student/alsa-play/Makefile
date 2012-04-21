# 2011-09-07

src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

target = main

CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g -lasound libmad.a

all: $(target)

$(target): $(obj)
	$(CC)  -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	-rm -f $(obj) $(target)

distclean: clean


.PHONY: all clean distclean

