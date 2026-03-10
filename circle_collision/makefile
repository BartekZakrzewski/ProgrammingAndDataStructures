all: circle_collision

rotating_polygon: circle_collision.o ../primlib/primlib.o
	gcc -g $^ -o $@ -lSDL2_gfx `sdl2-config --libs` -lm

.c.o:
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c $<

rotating_polygon.o: circle_collision.c ../primlib/primlib.h

clean:
	-rm circle_collision.o circle_collision
