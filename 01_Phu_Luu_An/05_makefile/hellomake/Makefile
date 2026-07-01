.PHONY: hello hellomake

CC := gcc
CFLAGS := -I.

INC_FILES := hello.h

%.o: %.c $(INC_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

test: hello.h hello.c
	@echo $@
	@echo $<
	@echo $^
#$@ is target
#$< is first dependance
#$^ is all dependance

hellomake: main.o hello.o
	$(CC) -o $@ main.o hello.o $(CFLAGS)

hello:
	gcc -g -o hello_exe hello.c main.c -I.

clean:
	rm -rf hello_exe