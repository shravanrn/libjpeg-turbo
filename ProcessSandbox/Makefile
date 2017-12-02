CC=g++
override CFLAGS += -std=gnu++11 -O3
override LIBFLAGS += -L. -ljpeg -lpthread -lrt
SHARED_LIB_FLAGS=-shared -fPIC
SHARED_LIB_LDFLAGS=-ldl
PRODUCTS=ProcessSandbox.a ProcessSandbox_otherside

.PHONY : all clean

all : $(PRODUCTS)

ProcessSandbox.a : ProcessSandbox.o Sandbox.o synch.o timing.o MyMalloc.o
	ar crv $@ $^
	ranlib $@

ProcessSandbox_otherside : ProcessSandbox_otherside.o synch.o MyMalloc.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBFLAGS)

ProcessSandbox_otherside.o : ProcessSandbox_otherside.cpp ProcessSandbox_otherside.h ProcessSandbox_sharedmem.h synch.h MyMalloc.h jpeglib_helpers.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

ProcessSandbox.o : ProcessSandbox.cpp ProcessSandbox.h ProcessSandbox_sharedmem.h Sandbox.h synch.h MyMalloc.h jpeglib_helpers.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

Sandbox.o : Sandbox.cpp Sandbox.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

synch.o : synch.cpp synch.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

timing.o : timing.cpp timing.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

MyMalloc.o : MyMalloc.cpp MyMalloc.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBFLAGS)

clean:
	rm $(PRODUCTS) *.o
