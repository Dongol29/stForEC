CC = gcc -g -Werror -ansi -pedantic -std=c99
CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE
TARGET = ish

ish: ish.o dynarray.o
	$(CC) $(CFLAGS) -o $@ $^

ish.o: ish.c dynarray.h
	$(CC) $(CFLAGS) -c -o $@ $<

dynarray.o: dynarray.c dynarray.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
