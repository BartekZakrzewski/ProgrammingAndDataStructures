CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = bank

SRCS = main.c
OBJS = $(SRCS:.c=.o)

run: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
