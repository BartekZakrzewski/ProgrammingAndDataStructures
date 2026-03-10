all: rotating_polygon

rotating_polygon: rotating_polygon.o primlib.o
	gcc -g $^ -o $@ -lSDL2_gfx `sdl2-config --libs` -lm

.c.o:
	gcc -g -Wall -pedantic `sdl2-config --cflags` -c $<

primlib.o: primlib.c primlib.h

rotating_polygon.o: rotating_polygon.c primlib.h

clean:
	-rm primlib.o rotating_polygon.o rotating_polygon
