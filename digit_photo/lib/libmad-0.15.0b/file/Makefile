src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = main
all:$(target)

$(target):$(obj)
	gcc -o $@ -g $^
%.o:%.c
	gcc -o $@ -g -c $<
clean:
	rm -f main.o
	rm -f main
