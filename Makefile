CC=g++
CFLAGS=-Wall -g --std=c++11
LDFLAGS=-ljack
FILES=*.cpp

all:
	${CC} ${CFLAGS} ${FILES} -o jkeysplit ${LDFLAGS}

clean:
	rm -f *.o; rm jkeysplit
