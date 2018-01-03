CC=gcc
CXX=g++
override CFLAGS += -O3 -mno-align-double
CFLAGS32=$(CFLAGS) -m32
CFLAGS64=$(CFLAGS) -m64
LDFLAGS32=$(LDFLAGS) -m32
LDFLAGS64=$(LDFLAGS) -m64
HOST32=i686-pc-linux-gnu
HOST64=x86_64-linux-gnu

# Folder names to store 32-bit and 64-bit builds respectively
BUILDS32=x86
BUILDS64=x64

# paths to various external resources
NACL_NASM=/home/cdisselk/CSE227/NASM_NaCl/nasm
SANDBOXING_NACL_DIR=/home/cdisselk/CSE227/Sandboxing_NaCl
NACL_GCC=$(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/bin/x86_64-nacl-gcc
PROCESS_SANDBOX_DIR=/home/cdisselk/CSE227/poc
PS_SRCS=$(wildcard $(PROCESS_SANDBOX_DIR)/*.cpp $(PROCESS_SANDBOX_DIR)/*.h)

# paths to the appropriate ProcessSandbox_otherside executables
OTHERSIDE_EXEC32 ?= $(PROCESS_SANDBOX_DIR)/ProcessSandbox_otherside_jpeg32
OTHERSIDE_EXEC64 ?= $(PROCESS_SANDBOX_DIR)/ProcessSandbox_otherside_jpeg64

# Username you are running the 'run' or 'dbg' targets under, for the purposes of the kill_old_processes target
USER=cdisselk

# Core to pin main process (or only process) to, for the 'run' and 'debug' targets
MAINCORE=0
# Core to pin sandbox process to, for the 'run_dynamic_ps' and 'debug_dynamic_ps' targets
SANDBOXCORE=2

PRODUCTS_BASENAMES=example_dynamic_non_nacl example_dynamic_nacl example_dynamic_ps
PRODUCTS32=$(addprefix $(BUILDS32)/,$(PRODUCTS_BASENAMES))
PRODUCTS64=$(addprefix $(BUILDS64)/,$(PRODUCTS_BASENAMES))
PRODUCTS=$(PRODUCTS32) $(PRODUCTS64)

RUN_BASENAMES=run_dynamic_non_nacl run_dynamic_nacl run_dynamic_ps
RUN32=$(addprefix $(BUILDS32)/,$(RUN_BASENAMES))
RUN64=$(addprefix $(BUILDS64)/,$(RUN_BASENAMES))

.PHONY : all all32 all64 run_all run_all32 run_all64 $(RUN32) $(RUN64) dbg_dynamic_non_nacl dbg_dynamic_nacl dbg_dynamic_ps debug clean kill_old_processes

all : $(PRODUCTS)
all32 : $(PRODUCTS32)
all64 : $(PRODUCTS64)

run_all32 : $(RUN32)
run_all64 : $(RUN64)
run_all : run_all32 run_all64

define prepare_build =
	cd .. && autoreconf -fiv
	rm -rf $(1)
	mkdir $(1)
	cd $(1)
endef

$(BUILDS32)/nacl_build :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd --host $(HOST32) CFLAGS='$(CFLAGS32) -DUSE_NACL' LDFLAGS='$(LDFLAGS32)' CC=$(NACL_GCC) && \
	make clean && \
	make

$(BUILDS64)/nacl_build :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd --host $(HOST64) CFLAGS='$(CFLAGS64) -DUSE_NACL' LDFLAGS='$(LDFLAGS64)' CC=$(NACL_GCC) && \
	make clean && \
	make

$(BUILDS32)/nacl_build_simd :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure --host $(HOST32) CFLAGS='$(CFLAGS32) -DUSE_NACL' LDFLAGS='$(LDFLAGS32)' CC=$(NACL_GCC) NASM=$(NACL_NASM) NAFLAGS='-nacl -DCOMPILING_FOR_NACL' && \
	make clean && \
	make

$(BUILDS64)/nacl_build_simd :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure --host $(HOST64) CFLAGS='$(CFLAGS64) -DUSE_NACL' LDFLAGS='$(LDFLAGS64)' CC=$(NACL_GCC) NASM=$(NACL_NASM) NAFLAGS='-nacl -DCOMPILING_FOR_NACL' && \
	make clean && \
	make

$(BUILDS32)/ps_build :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd --host $(HOST32) CFLAGS='$(CFLAGS32) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS32)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS64)/ps_build :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd --host $(HOST64) CFLAGS='$(CFLAGS64) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS64)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS32)/ps_build_simd :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure --host $(HOST32) CFLAGS='$(CFLAGS32) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS32)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS64)/ps_build_simd :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure --host $(HOST64) CFLAGS='$(CFLAGS64) -DUSE_PROCESS' LDFLAGS='$(LDFLAGS64)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS32)/non_nacl_build :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd -host $(HOST32) CFLAGS='$(CFLAGS32)' LDFLAGS='$(LDFLAGS32)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS64)/non_nacl_build :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure --without-simd -host $(HOST64) CFLAGS='$(CFLAGS64)' LDFLAGS='$(LDFLAGS64)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS32)/non_nacl_build_simd :
	-mkdir $(BUILDS32)
	$(call prepare_build,$@) && \
	sh ../../../configure -host $(HOST32) CFLAGS='$(CFLAGS32)' LDFLAGS='$(LDFLAGS32)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS64)/non_nacl_build_simd :
	-mkdir $(BUILDS64)
	$(call prepare_build,$@) && \
	sh ../../../configure -host $(HOST64) CFLAGS='$(CFLAGS64)' LDFLAGS='$(LDFLAGS64)' CC=$(CC) && \
	make clean && \
	make

$(BUILDS32)/example_dynamic_non_nacl : ../example_dynamic_non_nacl.cpp $(BUILDS32)/non_nacl_build
	$(CXX) $(CFLAGS32) -I$(BUILDS32)/non_nacl_build $< -o $@ -ldl

$(BUILDS64)/example_dynamic_non_nacl : ../example_dynamic_non_nacl.cpp $(BUILDS64)/non_nacl_build
	$(CXX) $(CFLAGS64) -I$(BUILDS64)/non_nacl_build $< -o $@ -ldl

$(BUILDS32)/example_dynamic_non_nacl_simd : ../example_dynamic_non_nacl.cpp $(BUILDS32)/non_nacl_build_simd
	$(CXX) $(CFLAGS32) -I$(BUILDS32)/non_nacl_build_simd $< -o $@ -ldl

$(BUILDS64)/example_dynamic_non_nacl_simd : ../example_dynamic_non_nacl.cpp $(BUILDS64)/non_nacl_build_simd
	$(CXX) $(CFLAGS64) -I$(BUILDS64)/non_nacl_build_simd $< -o $@ -ldl

$(BUILDS32)/example_dynamic_nacl.o : ../example_dynamic_nacl.cpp $(BUILDS32)/nacl_build
	$(CXX) -c $(CFLAGS32) -DUSE_NACL -I$(BUILDS32)/nacl_build -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ $< -o $@

$(BUILDS64)/example_dynamic_nacl.o : ../example_dynamic_nacl.cpp $(BUILDS64)/nacl_build
	$(CXX) -c $(CFLAGS64) -DUSE_NACL -I$(BUILDS64)/nacl_build -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ $< -o $@

$(BUILDS32)/example_dynamic_nacl_simd.o : ../example_dynamic_nacl.cpp $(BUILDS32)/nacl_build_simd
	$(CXX) -c $(CFLAGS32) -DUSE_NACL -I$(BUILDS32)/nacl_build_simd -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ $< -o $@

$(BUILDS64)/example_dynamic_nacl_simd.o : ../example_dynamic_nacl.cpp $(BUILDS64)/nacl_build_simd
	$(CXX) -c $(CFLAGS64) -DUSE_NACL -I$(BUILDS64)/nacl_build_simd -I$(SANDBOXING_NACL_DIR)/native_client/src/trusted/dyn_ldr/ $< -o $@

$(BUILDS32)/example_dynamic_nacl : $(BUILDS32)/example_dynamic_nacl.o
	$(CXX) $(CFLAGS32) $< -o $@ -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

$(BUILDS64)/example_dynamic_nacl : $(BUILDS64)/example_dynamic_nacl.o
	$(CXX) $(CFLAGS64) $< -o $@ -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

$(BUILDS32)/example_dynamic_nacl_simd : $(BUILDS32)/example_dynamic_nacl_simd.o
	$(CXX) $(CFLAGS32) $< -o $@ -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

$(BUILDS64)/example_dynamic_nacl_simd : $(BUILDS64)/example_dynamic_nacl_simd.o
	$(CXX) $(CFLAGS64) $< -o $@ -Wl,-rpath=$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -L$(SANDBOXING_NACL_DIR)/native_client/scons-out/opt-linux-x86-32/lib -ldyn_ldr -lsel -lnacl_error_code -lenv_cleanser -lnrd_xfer -lnacl_perf_counter -lnacl_base -limc -lnacl_fault_inject -lnacl_interval -lplatform_qual_lib -lvalidators -ldfa_validate_caller_x86_32 -lcpu_features -lvalidation_cache -lplatform -lgio -lnccopy_x86_32 -lrt -lpthread

$(BUILDS32)/example_dynamic_ps.o : ../example_dynamic_nacl.cpp $(BUILDS32)/ps_build
	$(CXX) -c $(CFLAGS32) -DUSE_PROCESS -I$(BUILDS32)/ps_build -I$(PROCESS_SANDBOX_DIR) -DUSE_LIBJPEG $< -o $@

$(BUILDS64)/example_dynamic_ps.o : ../example_dynamic_nacl.cpp $(BUILDS64)/ps_build
	$(CXX) -c $(CFLAGS64) -DUSE_PROCESS -I$(BUILDS64)/ps_build -I$(PROCESS_SANDBOX_DIR) -DUSE_LIBJPEG $< -o $@

$(BUILDS32)/example_dynamic_ps_simd.o : ../example_dynamic_nacl.cpp $(BUILDS32)/ps_build_simd
	$(CXX) -c $(CFLAGS32) -DUSE_PROCESS -I$(BUILDS32)/ps_build_simd -I$(PROCESS_SANDBOX_DIR) -DUSE_LIBJPEG $< -o $@

$(BUILDS64)/example_dynamic_ps_simd.o : ../example_dynamic_nacl.cpp $(BUILDS64)/ps_build_simd
	$(CXX) -c $(CFLAGS64) -DUSE_PROCESS -I$(BUILDS64)/ps_build_simd -I$(PROCESS_SANDBOX_DIR) -DUSE_LIBJPEG $< -o $@

$(BUILDS32)/example_dynamic_ps : $(BUILDS32)/example_dynamic_ps.o $(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg32.a
	$(CXX) $(CFLAGS32) $^ -o $@ -lrt -lpthread

$(BUILDS64)/example_dynamic_ps : $(BUILDS64)/example_dynamic_ps.o $(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg64.a
	$(CXX) $(CFLAGS64) $^ -o $@ -lrt -lpthread

$(BUILDS32)/example_dynamic_ps_simd : $(BUILDS32)/example_dynamic_ps_simd.o $(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg32.a
	$(CXX) $(CFLAGS32) $^ -o $@ -lrt -lpthread

$(BUILDS64)/example_dynamic_ps_simd : $(BUILDS64)/example_dynamic_ps_simd.o $(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg64.a
	$(CXX) $(CFLAGS64) $^ -o $@ -lrt -lpthread

# Assumes non-simd
$(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg32.a : $(PS_SRCS) $(BUILDS32)/ps_build
	$(MAKE) -C $(PROCESS_SANDBOX_DIR) CFLAGS='$(CFLAGS32) -I.. -I$(BUILDS32)/ps_build' LIBFLAGS='-L.. -L$(BUILDS32)/ps_build/.libs' $(@F)

# Assumes non-simd
$(PROCESS_SANDBOX_DIR)/ProcessSandbox_jpeg64.a : $(PS_SRCS) $(BUILDS64)/ps_build
	$(MAKE) -C $(PROCESS_SANDBOX_DIR) CFLAGS='$(CFLAGS64) -I.. -I$(BUILDS64)/ps_build' LIBFLAGS='-L.. -L$(BUILDS64)/ps_build/.libs' $(@F)

# Assumes non-simd
$(PROCESS_SANDBOX_DIR)/ProcessSandbox_otherside_jpeg32 : $(PS_SRCS) $(BUILDS32)/ps_build
	$(MAKE) -C $(PROCESS_SANDBOX_DIR) CFLAGS='$(CFLAGS32) -I.. -I$(BUILDS32)/ps_build' LIBFLAGS='-L.. -L$(BUILDS32)/ps_build/.libs' $(@F)

# Assumes non-simd
$(PROCESS_SANDBOX_DIR)/ProcessSandbox_otherside_jpeg64 : $(PS_SRCS) $(BUILDS64)/ps_build
	$(MAKE) -C $(PROCESS_SANDBOX_DIR) CFLAGS='$(CFLAGS64) -I.. -I$(BUILDS64)/ps_build' LIBFLAGS='-L.. -L$(BUILDS64)/ps_build/.libs' $(@F)

$(BUILDS32)/run_dynamic_non_nacl : $(BUILDS32)/example_dynamic_non_nacl
	echo && echo dynamic_non_nacl 32-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS32)/example_dynamic_non_nacl ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/non_nacl_build/.libs/libjpeg.so

$(BUILDS64)/run_dynamic_non_nacl : $(BUILDS64)/example_dynamic_non_nacl
	echo && echo dynamic_non_nacl 64-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS64)/example_dynamic_non_nacl ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/non_nacl_build/.libs/libjpeg.so

$(BUILDS32)/run_dynamic_non_nacl_simd : $(BUILDS32)/example_dynamic_non_nacl_simd
	echo && echo dynamic_non_nacl_simd 32-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS32)/example_dynamic_non_nacl_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/non_nacl_build_simd/.libs/libjpeg.so

$(BUILDS64)/run_dynamic_non_nacl_simd : $(BUILDS64)/example_dynamic_non_nacl_simd
	echo && echo dynamic_non_nacl_simd 64-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS64)/example_dynamic_non_nacl_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/non_nacl_build_simd/.libs/libjpeg.so

$(BUILDS32)/run_dynamic_nacl : $(BUILDS32)/example_dynamic_nacl
	echo && echo dynamic_nacl 32-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS32)/example_dynamic_nacl ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/nacl_build/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

$(BUILDS64)/run_dynamic_nacl : $(BUILDS64)/example_dynamic_nacl
	echo && echo dynamic_nacl 64-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS64)/example_dynamic_nacl ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/nacl_build/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

$(BUILDS32)/run_dynamic_nacl_simd : $(BUILDS32)/example_dynamic_nacl_simd
	echo && echo dynamic_nacl_simd 32-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS32)/example_dynamic_nacl_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/nacl_build_simd/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

$(BUILDS64)/run_dynamic_nacl_simd : $(BUILDS64)/example_dynamic_nacl_simd
	echo && echo dynamic_nacl_simd 64-bit: && echo && \
	taskset -c $(MAINCORE) $(GDB) ./$(BUILDS64)/example_dynamic_nacl_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/nacl_build_simd/.libs/libjpeg.so $(SANDBOXING_NACL_DIR)/native_client/toolchain/linux_x86/nacl_x86_glibc/x86_64-nacl/lib32/ $(SANDBOXING_NACL_DIR)/native_client/scons-out/nacl-x86-32-glibc/staging/dyn_ldr_sandbox_init.nexe

$(BUILDS32)/run_dynamic_ps : $(BUILDS32)/example_dynamic_ps $(OTHERSIDE_EXEC32) kill_old_processes
	echo && echo dynamic_ps 32-bit: && echo && \
	LD_LIBRARY_PATH=$(BUILDS32)/ps_build/.libs:$(LD_LIBRARY_PATH) \
	$(GDB) ./$(BUILDS32)/example_dynamic_ps ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/ps_build/.libs/libjpeg.so $(OTHERSIDE_EXEC32) $(MAINCORE) $(SANDBOXCORE)

$(BUILDS64)/run_dynamic_ps : $(BUILDS64)/example_dynamic_ps $(OTHERSIDE_EXEC64) kill_old_processes
	echo && echo dynamic_ps 64-bit: && echo && \
	LD_LIBRARY_PATH=$(BUILDS64)/ps_build/.libs:$(LD_LIBRARY_PATH) \
	$(GDB) ./$(BUILDS64)/example_dynamic_ps ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/ps_build/.libs/libjpeg.so $(OTHERSIDE_EXEC64) $(MAINCORE) $(SANDBOXCORE)

$(BUILDS32)/run_dynamic_ps_simd : $(BUILDS32)/example_dynamic_ps_simd $(OTHERSIDE_EXEC32) kill_old_processes
	echo && echo dynamic_ps_simd 32-bit: && echo && \
	LD_LIBRARY_PATH=$(BUILDS32)/ps_build_simd/.libs:$(LD_LIBRARY_PATH) \
	$(GDB) ./$(BUILDS32)/example_dynamic_ps_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS32)/ps_build_simd/.libs/libjpeg.so $(OTHERSIDE_EXEC32) $(MAINCORE) $(SANDBOXCORE)

$(BUILDS64)/run_dynamic_ps_simd : $(BUILDS64)/example_dynamic_ps_simd $(OTHERSIDE_EXEC64) kill_old_processes
	echo && echo dynamic_ps_simd 64-bit: && echo && \
	LD_LIBRARY_PATH=$(BUILDS64)/ps_build_simd/.libs:$(LD_LIBRARY_PATH) \
	$(GDB) ./$(BUILDS64)/example_dynamic_ps_simd ../test.jpeg ../test_out.jpeg ./$(BUILDS64)/ps_build_simd/.libs/libjpeg.so $(OTHERSIDE_EXEC64) $(MAINCORE) $(SANDBOXCORE)

$(BUILDS32)/dbg_dynamic_non_nacl : debug $(BUILDS32)/run_dynamic_non_nacl
$(BUILDS32)/dbg_dynamic_non_nacl_simd : debug $(BUILDS32)/run_dynamic_non_nacl_simd
$(BUILDS32)/dbg_dynamic_nacl : debug $(BUILDS32)/run_dynamic_nacl
$(BUILDS32)/dbg_dynamic_nacl_simd : debug $(BUILDS32)/run_dynamic_nacl_simd
$(BUILDS32)/dbg_dynamic_ps : debug $(BUILDS32)/run_dynamic_ps
$(BUILDS32)/dbg_dynamic_ps_simd : debug $(BUILDS32)/run_dynamic_ps_simd
$(BUILDS64)/dbg_dynamic_non_nacl : debug $(BUILDS64)/run_dynamic_non_nacl
$(BUILDS64)/dbg_dynamic_non_nacl_simd : debug $(BUILDS64)/run_dynamic_non_nacl_simd
$(BUILDS64)/dbg_dynamic_nacl : debug $(BUILDS64)/run_dynamic_nacl
$(BUILDS64)/dbg_dynamic_nacl_simd : debug $(BUILDS64)/run_dynamic_nacl_simd
$(BUILDS64)/dbg_dynamic_ps : debug $(BUILDS64)/run_dynamic_ps
$(BUILDS64)/dbg_dynamic_ps_simd : debug $(BUILDS64)/run_dynamic_ps_simd

debug :
	$(eval GDB ?= gdb --args)
	$(eval CFLAGS32 += -g)
	$(eval CFLAGS64 += -g)

kill_old_processes :
	-pkill -u $(USER) -f ProcessSandbox_otherside

clean :
	-rm -rf $(BUILDS32) $(BUILDS64) *.o
	-$(MAKE) -C $(PROCESS_SANDBOX_DIR) clean
