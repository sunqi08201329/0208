# 2010.2.22


src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = main

CC = gcc
#CC = arm-linux-gcc
#CFLAGS = -Wall -g -c -I../jpeg-8/ -I../freetype-2.3.11/include
#LDFLAGS = -L../jpeg-8/.libs/ -ljpeg -L../freetype-2.3.11/objs/.libs -lfreetype
CFLAGS = -Wall -g -c -I../jpeg-8/ 
LDFLAGS = -L../jpeg-8/.libs/ -ljpeg 

all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
$(DIR)%.o:%.c
	$(CC) -o $@ $(CFLAGS) $< 

clean:
	-rm -f $(target)
	-rm -f $(obj)
	-rm -f *~

.PHONY: clean
test:
	@echo $(src)
	@echo $(obj)
