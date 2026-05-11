CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = bsearch

SRCS = main.c func/func.c
HEADERS = func/func.h
OBJS = $(SRCS:.c=.o)

run: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
