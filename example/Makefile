all: example

example: example.cpp
	g++ $< ../lib/libthreadpool.a -std=c++0x -I../include -lpthread -o example

clean:
	rm -f example
