# List your *.h files (if you do not have them in your project then leave the variable "headers" empty):
headers = rand_malloc.h

# List your *.c files:
sources = text_processing.c rand_malloc.c

# Specify name of your program:
target = text_processing

$(target): $(headers) $(sources)
	gcc -g -Wall -pedantic $(sources) -o $(target)

.PHONY: clean
clean:
	rm $(target)

.PHONY: check
check: $(target)
	valgrind --leak-check=full --track-origins=yes ./$(target)
