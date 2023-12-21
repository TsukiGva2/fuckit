CFLAGS=`sdl2-config --cflags`
LIBS=`sdl2-config --libs` -lm

RELEASE=-O2
DEBUG=-O0 -g -Wall -Wextra -Wno-incompatible-pointer-types-discards-qualifiers

all:
	gcc LCD/LCD.c -c -o LCD.o $(CFLAGS) $(DEBUG)
	gcc debug/debug.c -c -o debug.o $(DEBUG)
	gcc handler/han.c -c -o han.o $(DEBUG)
	gcc game/game.c -c -o game.o $(CFLAGS) $(DEBUG)
	gcc game/gameobj.c -c -o gameobj.o $(CFLAGS) $(DEBUG)
	gcc map/map.c -c -o map.o $(CFLAGS) $(DEBUG)
	gcc player/player.c -c -o player.o $(CFLAGS) $(DEBUG)
	gcc fuckit.c -c -o fuckit.o $(CFLAGS) $(DEBUG)
	gcc fuckit.o debug.o LCD.o han.o game.o gameobj.o player.o map.o -o fuckit $(CFLAGS) $(LIBS) $(DEBUG)

release:
	gcc fuckit.c -o fuckit $(CFLAGS) $(LIBS) $(RELEASE)

clean:
	rm *.o *.core fuckit

