CC=g++
CFLAGS=-Wall -c
EXECUTABLE=c8dis

all:main.o Disassembler.o
	$(CC) -o $(EXECUTABLE) $^

%.o:%.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *o $(EXECUTABLE)
