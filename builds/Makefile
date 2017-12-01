CC=gcc
CXX=g++
NACL_NASM=/home/cdisselk/CSE227/NASM_NaCl/nasm
SANDBOXING_NACL_DIR=/home/cdisselk/CSE227/Sandboxing_NaCl
NACL_GCC=$(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/bin/x86_64-nacl-gcc
CFLAGS=-O3 -m32 -mno-align-double
LDFLAGS=-m32
HOST=i686-pc-linux-gnu
PRODUCTS=example_static example_dynamic_non_nacl example_dynamic_nacl
CORE=0

all : $(PRODUCTS)

run_all : run_static run_dynamic_non_nacl run_dynamic_nacl

nacl_build :
	cd .. && autoreconf -fiv
	rm -rf nacl_build
	mkdir nacl_build && \
	cd nacl_build && \
	sh ../../configure --without-simd --host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(NACL_GCC) && \
	make clean && \
	make

nacl_build_simd :
	cd .. && autoreconf -fiv
	rm -rf nacl_build_simd
	mkdir nacl_build_simd && \
	cd nacl_build_simd && \
	sh ../../configure --host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(NACL_GCC) NASM=$(NACL_NASM) NAFLAGS='-nacl -DCOMPILING_FOR_NACL' && \
	make clean && \
	make

non_nacl_build :
	cd .. && autoreconf -fiv
	rm -rf non_nacl_build
	mkdir non_nacl_build && \
	cd non_nacl_build && \
	sh ../../configure --without-simd -host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

non_nacl_build_simd :
	cd .. && autoreconf -fiv
	rm -rf non_nacl_build_simd
	mkdir non_nacl_build_simd && \
	cd non_nacl_build_simd && \
	sh ../../configure -host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

example_static : non_nacl_build non_nacl_build/.libs/libturbojpeg.a ../example.c
	$(CC) $(CFLAGS) -I./non_nacl_build -o example_static ../example.c ./non_nacl_build/.libs/libturbojpeg.a

example_dynamic_non_nacl : non_nacl_build ../example_dynamic_non_nacl.cpp
	$(CXX) $(CFLAGS) -I./non_nacl_build -o example_dynamic_non_nacl ../example_dynamic_non_nacl.cpp -ldl

example_dynamic_nacl : nacl_build ../example_dynamic_nacl.cpp
	$(CXX) -c $(CFLAGS) -I./nacl_build -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ -o example_dynamic_nacl.o ../example_dynamic_nacl.cpp && \
	$(CXX) $(CFLAGS) -o example_dynamic_nacl -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib example_dynamic_nacl.o -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

run_static : example_static
	echo && echo static: && echo && \
	taskset -c $(CORE) ./example_static ../test.jpeg ../test_out.jpeg

run_dynamic_non_nacl : example_dynamic_non_nacl
	echo && echo dynamic_non_nacl: && echo && \
	taskset -c $(CORE) ./example_dynamic_non_nacl ../test.jpeg ../test_out.jpeg ./non_nacl_build/.libs/libjpeg.so

run_dynamic_nacl : example_dynamic_nacl
	echo && echo dynamic_nacl: && echo && \
	taskset -c $(CORE) ./example_dynamic_nacl ../test.jpeg ../test_out.jpeg ./nacl_build/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

clean :
	rm -rf nacl_build nacl_build_simd non_nacl_build non_nacl_build_simd $(PRODUCTS) *.o
