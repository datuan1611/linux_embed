.PHONY: static shared clean

export LD_LIBRARY_PATH=$(pwd)/lib/shared

static:
#create object
	gcc -c main.c -o obj/main.o -I./inc
	gcc -c src/hello_A.c -o obj/static/hello_A.o -I./inc
	gcc -c src/hello_B.c -o obj/static/hello_B.o -I./inc
#create static libraries
	ar rcs lib/static/libhello.a obj/static/hello_A.o obj/static/hello_B.o
#link static libraries
	gcc obj/main.o -Llib/static -lhello -o bin/static_exec

shared:
#create object
	gcc -c main.c -o obj/main.o -I./inc
	gcc -c -fPIC src/hello_A.c -o obj/shared/hello_A.o -I./inc
	gcc -c -fPIC src/hello_B.c -o obj/shared/hello_B.o -I./inc
#create shared libraries
	gcc -shared  obj/shared/hello_A.o obj/shared/hello_B.o -o lib/shared/libhello.so
#link shared libraries
	gcc obj/main.o -Llib/shared -lhello -o bin/shared_exec

clean:
	rm -rf obj/static/* obj/shared/* lib/static/* lib/shared/* bin/*
