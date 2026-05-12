CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic

TARGET = bank

SRCS = main.c helpers/helpers.c options/options.c actions/actions.c table/table.c 
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

reset_db:
	rm db/accounts.dat
	rm db/insurances.dat
	touch db/accounts.dat
	touch db/insurances.dat

run: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c helpers/helpers.h actions/actions.h options/options.h 
	$(CC) $(CFLAGS) -c main.c -o main.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
