.PHONY: hello stage1 stage2 stage3 stage4 all clean

hello:
	gcc -c -o main.o main.c

stage1:
	gcc -E hello.c -o hello.i

stage2:
	gcc -S hello.i -o hello.S

stage3:
	gcc -c hello.S -o hello.o

stage4:
	gcc -o hello hello.o main.o

all:
	gcc -o hello hello.c

clean:
	rm -rf hello hello.i hello.s hello.o main.o