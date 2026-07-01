.PHONY: all clean

all:
	gcc src/server.c -o bin/server -lrt -pthread -I./inc
	gcc src/client.c -o bin/client -lrt -pthread -I./inc

clean:
	rm -rf bin/*
