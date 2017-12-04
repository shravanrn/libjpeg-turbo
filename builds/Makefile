CC=gcc
CXX=g++
NACL_NASM=/home/cdisselk/CSE227/NASM_NaCl/nasm
SANDBOXING_NACL_DIR=/home/cdisselk/CSE227/Sandboxing_NaCl
NACL_GCC=$(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/bin/x86_64-nacl-gcc
CFLAGS=-O3 -m32 -mno-align-double
LDFLAGS=-m32
HOST=i686-pc-linux-gnu
PRODUCTS=example_static example_dynamic_non_nacl example_dynamic_nacl example_dynamic_ps
PS_SRCS=$(wildcard ../ProcessSandbox/*.cpp ../ProcessSandbox/*.h)
USER=cdisselk
CORE=0

.PHONY : all run_all run_static run_dynamic_non_nacl run_dynamic_nacl run_dynamic_ps dbg_static dbg_dynamic_non_nacl dbg_dynamic_nacl dbg_dynamic_ps debug clean kill_old_processes

all : $(PRODUCTS)

run_all : run_static run_dynamic_non_nacl run_dynamic_nacl run_dynamic_ps

define prepare_build =
	cd .. && autoreconf -fiv
	rm -rf $(1)
	mkdir $(1)
	cd $(1)
endef

nacl_build :
	$(call prepare_build,$@) && \
	sh ../../configure --without-simd --host $(HOST) CFLAGS='$(CFLAGS) -DUSE_NACL' LDFLAGS='$(LDFLAGS)' CC=$(NACL_GCC) && \
	make clean && \
	make

nacl_build_simd :
	$(call prepare_build,$@) && \
	sh ../../configure --host $(HOST) CFLAGS='$(CFLAGS) -DUSE_NACL' LDFLAGS='$(LDFLAGS)' CC=$(NACL_GCC) NASM=$(NACL_NASM) NAFLAGS='-nacl -DCOMPILING_FOR_NACL' && \
	make clean && \
	make

ps_build :
	$(call prepare_build,$@) && \
	sh ../../configure --without-simd --host $(HOST) CFLAGS='$(CFLAGS) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

ps_build_simd :
	$(call prepare_build,$@) && \
	sh ../../configure --host $(HOST) CFLAGS='$(CFLAGS) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

non_nacl_build :
	$(call prepare_build,$@) && \
	sh ../../configure --without-simd -host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

non_nacl_build_simd :
	$(call prepare_build,$@) && \
	sh ../../configure -host $(HOST) CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' CC=$(CC) && \
	make clean && \
	make

example_static : non_nacl_build non_nacl_build/.libs/libturbojpeg.a ../example.c
	$(CC) $(CFLAGS) -I./non_nacl_build -o $@ ../example.c ./non_nacl_build/.libs/libturbojpeg.a

example_dynamic_non_nacl : ../example_dynamic_non_nacl.cpp non_nacl_build
	$(CXX) $(CFLAGS) -I./non_nacl_build $< -o $@ -ldl

example_dynamic_nacl.o : ../example_dynamic_nacl.cpp nacl_build
	$(CXX) -c $(CFLAGS) -DUSE_NACL -I./nacl_build -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ $< -o $@

example_dynamic_nacl : example_dynamic_nacl.o
	$(CXX) $(CFLAGS) $< -o $@ -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

example_dynamic_ps.o : ../example_dynamic_nacl.cpp ps_build
	$(CXX) -c $(CFLAGS) -DUSE_PROCESS -I./ps_build -I../ProcessSandbox $< -o $@

example_dynamic_ps : example_dynamic_ps.o ../ProcessSandbox/ProcessSandbox.a
	$(CXX) $(CFLAGS) $^ -o $@ -lrt -lpthread

# Assumes non-simd
../ProcessSandbox/ProcessSandbox.a : $(PS_SRCS) ps_build
	$(MAKE) -C ../ProcessSandbox CFLAGS='$(CFLAGS) -I.. -I../builds/ps_build' LIBFLAGS='-L.. -L../builds/ps_build/.libs' $(@F)

# Assumes non-simd
../ProcessSandbox/ProcessSandbox_otherside : $(PS_SRCS) ps_build
	$(MAKE) -C ../ProcessSandbox CFLAGS='$(CFLAGS) -I.. -I../builds/ps_build' LIBFLAGS='-L.. -L../builds/ps_build/.libs' $(@F)

run_static : example_static
	echo && echo static: && echo && \
	taskset -c $(CORE) $(GDB) ./example_static ../test.jpeg ../test_out.jpeg

run_dynamic_non_nacl : example_dynamic_non_nacl
	echo && echo dynamic_non_nacl: && echo && \
	taskset -c $(CORE) $(GDB) ./example_dynamic_non_nacl ../test.jpeg ../test_out.jpeg ./non_nacl_build/.libs/libjpeg.so

run_dynamic_nacl : example_dynamic_nacl
	echo && echo dynamic_nacl: && echo && \
	taskset -c $(CORE) $(GDB) ./example_dynamic_nacl ../test.jpeg ../test_out.jpeg ./nacl_build/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

run_dynamic_ps : example_dynamic_ps ../ProcessSandbox/ProcessSandbox_otherside kill_old_processes
	echo && echo dynamic_ps: && echo && \
	LD_LIBRARY_PATH=../builds/ps_build/.libs:$(LD_LIBRARY_PATH) \
	taskset -c $(CORE) $(GDB) ./example_dynamic_ps ../test.jpeg ../test_out.jpeg ./ps_build/.libs/libjpeg.so notused notused

dbg_static : debug run_static
dbg_dynamic_non_nacl : debug run_dynamic_non_nacl
dbg_dynamic_nacl : debug run_dynamic_nacl
dbg_dynamic_ps : debug run_dynamic_ps

debug :
	$(eval GDB ?= gdb --args)
	$(eval CFLAGS += -g)

kill_old_processes :
	-pkill -u $(USER) -f ProcessSandbox_otherside

clean :
	-rm -rf nacl_build nacl_build_simd non_nacl_build non_nacl_build_simd ps_build ps_build_simd $(PRODUCTS) *.o
	-$(MAKE) -C ../ProcessSandbox clean
