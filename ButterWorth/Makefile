# Makefile
# Author: Hiromichi Suenaga

TARGET = butterworth
DEFINE = 
CC = g++ -Wall -O2 -Wno-deprecated -g $(DEFINE)
IBS = -L../../lib -L/usr/X11R6/lib -L/usr/lib
INDS = -I. -I../../include -I/usr/X11R6/include -I/usr/include 
OPTS = -lm -lnewmat

all: $(TARGET)

# バターワースフィルタプログラム
butterworth: butterworth.o chk.o
	${CC} -o butterworth butterworth.o chk.o ${OPTS}

butterworth.o: butterworth.cpp
	${CC} -c -o butterworth.o butterworth.cpp

# チェック関数ライブラリ 
chk.o: chk.cpp
	$(CC) -c -o chk.o chk.cpp

.cc.o:
	${CC} ${CFLAGS} -c -o $@ $<

clean: 
	rm -f *.o *~ $(TARGET) core
