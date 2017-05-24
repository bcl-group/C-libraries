CC	= gcc
C++     = g++
CFLAGS	= -g -O2 -Wall `imlib-config --cflags` `imlib-config --cflags-gdk`
LDFLAGS	= `imlib-config --libs-gdk` -lm -02
CFLAGS	= -O2 `imlib-config --cflags` `imlib-config --cflags-gdk`
LDFLAGS	= `imlib-config --libs-gdk` -lm

PROGRAM	= moca

all:   moca

moca: moca.o ReadRiff.o 
	$(C++) -o  $@ $(LDFLAGS) ReadRiff.o $< 

moca2: moca2.o ReadRiff.o
	$(CC) -o  $@ $(LDFLAGS) ReadRiff.o $<

moca.o: moca.cc 
	$(CC) -c -o $@ $(CFLAGS) $< 
moca2.o: moca2.cc
	$(CC) -c -o $@ $(CFLAGS) $<
ReadRiff.o: ReadRiff.cc
	$(CC) -c -o $@ $(CFLAGS) $<  

clean:
	$(RM) $(FILES) *.o *~ .[^.]*~ "#"*


txy:	txy.o  -lm        
	gcc -o txy txy.o -lm -02

angle:	angle.o  -lm        
	gcc -o angle angle.o -lm -02

angle_vel:	angle_vel.o  -lm -02	        
	gcc -o angle_vel angle_vel.o -lm -02

.c.o:
	gcc -c -o $@ $<

filter: filter.o getfp.o
	$(CC) -o  $@ $(LDFLAGS) getfp.o $<

filter.o: filter.c
	$(CC) -c -o $@ $(CFLAGS) $<

getfp.o: getfp.c
	$(CC) -c -o $@ $(CFLAGS) $<

filter2: filter2.o getfp.o
	$(CC) -o  $@ $(LDFLAGS) getfp.o $<

filter2.o: filter2.c
	$(CC) -c -o $@ $(CFLAGS) $<
 
