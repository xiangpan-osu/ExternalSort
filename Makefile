CC=g++
CFLAGS=-c -std=c++11
 
all: kv_sort
 
kv_sort: key_value_sort.o
	$(CC) key_value_sort.o -o kv_sort

key_value_sort.o: key_value_sort.cpp
	$(CC) $(CFLAGS) key_value_sort.cpp

clean:
	rm -rf *.o *.txt kv_sort
