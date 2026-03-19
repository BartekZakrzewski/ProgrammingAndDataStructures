CC          := gcc
SDL_CFLAGS  := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_gfx

CFLAGS      := -fsanitize=undefined -Wall -Wextra -std=c11 $(SDL_CFLAGS) -I.
LDFLAGS     := -fsanitize=undefined $(SDL_LDFLAGS)

TARGET      := hanoi

SRCS        := main.c primlib.c
OBJS        := main.o primlib.o

run: clean all
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c primlib.h
	$(CC) $(CFLAGS) -c main.c

primlib.o: primlib.c primlib.h
	$(CC) $(CFLAGS) -c primlib.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
