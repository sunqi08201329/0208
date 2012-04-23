SRC = $(wildcard src/*.c)
objs = $(patsubst %.c,%.o,$(SRC))

CC = gcc
CFLAGS = -Wall -g -Iinclude -I./lib/jpeg-8/ -I./lib/freetype-2.3.11/include/ -I./lib/libmad-0.15.0b/
LDFLAGS = -lm -lpthread -L./lib/jpeg-8/.libs/ -ljpeg -L./lib/freetype-2.3.11/objs/.libs/ -lfreetype -L./lib/libmad-0.15.0b/.libs/ -lmad


#TARGET = main
TARGET = app/main

all:$(TARGET) 

$(TARGET):$(objs)
	$(CC) $(LDFLAGS) $^ -o $@ 

%.o:src/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -f $(TARGET)
	-rm -f $(objs)
	-rm -f src/*~
	-rm -f include/*~
	-rm -f list/*.txt
	-rm -f src/*.bak
