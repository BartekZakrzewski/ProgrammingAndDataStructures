CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = bank

SRCS = main.c table/table.c accounts/accounts.c constants/constants.c helpers/helpers.c shared/shared.c insurances/insurances.c
OBJS = $(SRCS:.c=.o)

run: clean $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c constants/constants.h helpers/helpers.h shared/shared.h
	$(CC) $(CFLAGS) -c main.c -o main.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
