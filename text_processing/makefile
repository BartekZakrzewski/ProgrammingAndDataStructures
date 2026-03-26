CC					:= gcc

CFLAGS			:= -Wall -Wextra -std=c11 -I.

TARGET			:= text_processing

PROGRAM			:= main
PROGRAM_C		:= $(PROGRAM).c
PROGRAM_O		:= $(PROGRAM).o

SRCS				:= $(PROGRAM_C)
OBJS				:= $(PROGRAM_O)

run: clean all
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -g $(OBJS) -o $(TARGET)

$(PROGRAM_O): $(PROGRAM_C)
	$(CC) -g $(CFLAGS) -c $(PROGRAM_C)

clean:
	rm -f $(OBJS) $(TARGET)
