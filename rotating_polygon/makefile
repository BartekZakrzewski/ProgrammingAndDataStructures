all: rotating_polygon

rotating_polygon: rotating_polygon.o ../primlib/primlib.o
	gcc -g $^ -o $@ -lSDL2_gfx `sdl2-config --libs` -lm

.c.o:
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c $<

rotating_polygon.o: rotating_polygon.c ../primlib/primlib.h

clean:
	-rm rotating_polygon.o rotating_polygon
