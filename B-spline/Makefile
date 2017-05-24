# Makefile
# Author: Hiromichi Suenaga

TARGET = butterworth
DEFINE = 
CC = g++ -Wall -O2 -Wno-deprecated -g $(DEFINE)
IBS = -L../../lib -L/usr/X11R6/lib -L/usr/lib
INDS = -I. -I../../include -I/usr/X11R6/include -I/usr/include 
OPTS = -lm -lnewmat

all: $(TARGET)

B-spline-test: B-spline-test.o B-spline.o
	${CC} -o $@ B-spline-test.o B-spline.o ${Loadlibs}

B-spline-test.o: B-spline-test.cc B-spline.h
B-spline.o: B-spline.cc

.cc.o:
	${CC} ${CFLAGS} -c -o $@ $<

clean: 
	rm -f *.o *~ $(TARGET) core
