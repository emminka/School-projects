CC=g++
CFLAGS=-pedantic -Wall -Wextra  -g -lpcap

all:
	$(CC) $(CFLAGS) flow.cpp -o flow

clean:
	rm -f flow

pack:
	tar cvf xkromp00.tar flow.cpp makefile flow.1 manual.pdf
