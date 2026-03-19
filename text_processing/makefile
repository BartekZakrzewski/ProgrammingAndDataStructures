CC					:= gcc

CFLAGS			:= -Wall -Wextra -std=c11 -I.

TARGET			:= text_processing

PROGRAM			:= main
PROGRAM_C		:= $(PROGRAM).c
PROGRAM_O		:= $(PROGRAM).o

SRCS				:= $(PROGRAM_C)
OBJS				:= $(PROGRAM_O)

all: $(TARGET)

$(Target): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(PROGRAM_O): $(PROGRAM_C)
	$(CC) $(CFLAGS) -c $(PROGRAM_C)

clean:
	rm -f $(OBJS) $(TARGET)
