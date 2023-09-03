CFLAGS=`sdl2-config --cflags`
LIBS=`sdl2-config --libs`

RELEASE=-O2
DEBUG=-O0 -g -Wall -Wextra

all:
	gcc LCD/LCD.c -c -o LCD.o $(CFLAGS) $(DEBUG)
	gcc debug/debug.c -c -o debug.o $(DEBUG)
	gcc handler/han.c -c -o han.o $(DEBUG)
	gcc fuckit.c debug.o LCD.o han.o -o fuckit $(CFLAGS) $(LIBS) $(DEBUG)

release:
	gcc fuckit.c -o fuckit $(CFLAGS) $(LIBS) $(RELEASE)

clean:
	rm *.o *.core fuckit

