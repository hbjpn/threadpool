.cpp.o:
	g++ -c $< -std=c++0x -lpthread -o $@ -O3 -fPIC

libthreadpool.a: ThreadPool.o
	ar rv $@ $<
	mv $@ ../lib/

libthreadpool.so.1.0: ThreadPool.o
	gcc -shared -Wl,-soname,libthreadpool.so.1 -o $@ $<
	mv $@ ../lib/
	ln -s ../lib/$@ ../lib/libthreadpool.so

all: libthreadpool.a libthreadpool.so.1.0
	cp ThreadPool.h ../include/
	
clean:
	rm -rf *.o *.a *.so.*
	rm -rf ../lib/*
	rm -rf ../include/*
