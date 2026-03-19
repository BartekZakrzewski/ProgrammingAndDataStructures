CC          := gcc
SDL_CFLAGS  := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_gfx

CFLAGS      := -fsanitize=undefined -Wall -Wextra -std=c11 $(SDL_CFLAGS) -I.
LDFLAGS     := -fsanitize=undefined $(SDL_LDFLAGS)

TARGET      := tetris

PROGRAM     := main
PROGRAM_C		:= $(PROGRAM).c
PROGRAM_O   := $(PROGRAM).o

LIB 				:= primlib
LIB_C  			:= $(LIB).c
LIB_O			  := $(LIB).o
LIB_HEADER  := $(LIB).h

GLOBAL_VARIABLES := global_variables
GLOBAL_VARIABLES_C := $(GLOBAL_VARIABLES).c
GLOBAL_VARIABLES_O := $(GLOBAL_VARIABLES).o
GLoBAL_VARIABLES_HEADER := $(GLOBAL_VARIABLES).h

SRCS        := $(PROGRAM_C) $(LIB_C) $(GLOBAL_VARIABLES_C)
OBJS        := $(PROGRAM_O) $(LIB_O) $(GLOBAL_VARIABLES_O)
HEADERS			:= $(LIB_HEADER) $(GLOBAL_VARIABLES_HEADER)

run: clean all
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(PROGRAM_O): $(PROGRAM_C) $(HEADERS)
	$(CC) $(CFLAGS) -c $(PROGRAM_C)

$(LIB_O): $(LIB_C) $(LIB_HEADER)
	$(CC) $(CFLAGS) -c $(LIB_C)

$(GLOBAL_VARIABLES_O): $(GLOBAL_VARIABLES_C) $(GLOBAL_VARIABLES_HEADER)
	$(CC) $(CFLAGS) -c $(GLOBAL_VARIABLES_C)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
