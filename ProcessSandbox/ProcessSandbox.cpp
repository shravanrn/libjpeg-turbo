#include "ProcessSandbox.h"
#include "ProcessSandbox_otherside.h"
#include "ProcessSandbox_sharedmem.h"
#include "MyMalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <new>  // placement new
#include <signal.h>  // SIGCHLD
#include <errno.h>
#include <string.h>  // strerror()
#include <fcntl.h>

int ProcessSandbox::startOtherside(void* arg) {
  if(!execl(othersidepath, othersidepath, (char*)NULL)) {
    printf("execl failed with code %d (%s)\n", errno, strerror(errno));
    exit(1);
  }
}

pid_t ProcessSandbox::createSandboxProcess() {
  char* stack = (char*)malloc(STACK_SIZE);
  char* stackTop = stack + STACK_SIZE;
  return clone(startOtherside, stackTop,
    SIGCHLD | CLONE_FILES | CLONE_FS | CLONE_IO
    // Experiments on 10/13/17 show that removing CLONE_IO either has no effect on
    //   function call overhead, or causes a slight increase.
    // Likewise for CLONE_FILES and CLONE_FS.
    // Keep in mind this is for a 'toy' benchmark that doesn't involve much
    //   file operations or I/O.  (sandbox-benchmarking commit 03dfce3 as baseline)
    // Note 12/1/17: Use of execl() may have made this more moot; additional testing
    //   is called for
    , sharedmem);
}

// returns the file descriptor
static int createFileOfSize(const char* path, unsigned bytes) {
  int fd = shm_open(path, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
  if(fd < 0) {
    printf("shm_open with O_CREAT returned error %d (%s)\n", errno, strerror(errno));
    exit(1);
  }
  off_t offt = lseek(fd, bytes-1, SEEK_SET);
  if(offt == (off_t)-1) {
    printf("lseek returned error %d (%s)\n", errno, strerror(errno));
    exit(1);
  }
  write(fd, "", 1);  // must write to end of file to keep file from being truncated
  return fd;
}

sharedmem_t* ProcessSandbox::createSharedMem() {
  void* sharedpage_mem = mmap(SHAREDMEM_VADDR, SHAREDMEM_SIZE, PROT_READ | PROT_WRITE,
    MAP_SHARED, createFileOfSize(shmempath, SHAREDMEM_SIZE), 0);
	if((uintptr_t)sharedpage_mem != (uintptr_t)SHAREDMEM_VADDR) {
		printf("Warning: sharedpage_mem (%p) != SHAREDMEM_VADDR (%p)\n", sharedpage_mem, (void*)SHAREDMEM_VADDR);
	}
	int* dummy = (int*)sharedpage_mem;
	*dummy = 123;  // see if error
	int* dummy2 = (int*)((uintptr_t)sharedpage_mem + SHAREDMEM_SIZE - sizeof(int));  // last int in sharedpage_mem
	*dummy2 = 234;  // see if error
  sharedmem_t* sp = new (sharedpage_mem) sharedmem_t;  // "placement new" runs constructor for sharedmem_t
	printf("Created sharedmem at %p\n", sp);
  return sp;
}

ProcessSandbox::ProcessSandbox() : sharedmem(createSharedMem()), myMalloc(new MyMalloc(sharedmem->extraSpace, SHAREDMEM_SIZE-sizeof(sharedmem_t), true)) {
  sandboxPID = createSandboxProcess();
	printf("Created sandbox process %u\n", sandboxPID);
  sharedmem->invoker.invokeRight();  // allow sandbox to initialize itself, get ready
}

void ProcessSandbox::destroySandbox() {
  delete myMalloc;
  if(kill(sandboxPID, SIGTERM)) {
    printf("Failed to kill %u, error %d (%s)\n", sandboxPID, errno, strerror(errno));
  }
  munmap(SHAREDMEM_VADDR, SHAREDMEM_SIZE);
  shm_unlink(shmempath);
  remove(shmempath);
}

void* ProcessSandbox::mallocInSandbox(uint32_t size) {
  return myMalloc->malloc(size);
}

void ProcessSandbox::freeInSandbox(void* ptr) {
  myMalloc->free(ptr);
}

#define WRAPPED_FUNCTION_1ARG_VOID(fnum, fname, rettype, arg1type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1 }; \
    invokeFunctionCall(); \
  }

#define WRAPPED_FUNCTION_2ARG_VOID(fnum, fname, rettype, arg1type, arg2type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2 }; \
    invokeFunctionCall(); \
  }

#define WRAPPED_FUNCTION_3ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3 }; \
    invokeFunctionCall(); \
  }

#define WRAPPED_FUNCTION_4ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3, arg4type arg4) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3, arg4 }; \
    invokeFunctionCall(); \
  }

#define WRAPPED_FUNCTION_5ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type, arg5type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3, arg4type arg4, arg5type arg5) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3, arg4, arg5 }; \
    invokeFunctionCall(); \
  }

#define WRAPPED_FUNCTION_1ARG_NONVOID(fnum, fname, rettype, arg1type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1 }; \
    invokeFunctionCall(); \
    return sharedmem->retval.as_##rettype; \
  }

#define WRAPPED_FUNCTION_2ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2 }; \
    invokeFunctionCall(); \
    return sharedmem->retval.as_##rettype; \
  }

#define WRAPPED_FUNCTION_3ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3 }; \
    invokeFunctionCall(); \
    return sharedmem->retval.as_##rettype; \
  }

#define WRAPPED_FUNCTION_4ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3, arg4type arg4) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3, arg4 }; \
    invokeFunctionCall(); \
    return sharedmem->retval.as_##rettype; \
  }

#define WRAPPED_FUNCTION_5ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type, arg5type) \
  rettype (ProcessSandbox::inv_##fname) (arg1type arg1, arg2type arg2, arg3type arg3, arg4type arg4, arg5type arg5) { \
    sharedmem->funcnum = fnum; \
    sharedmem->fname##_args = { arg1, arg2, arg3, arg4, arg5 }; \
    invokeFunctionCall(); \
    return sharedmem->retval.as_##rettype; \
  }

FOR_EACH_1ARG_VOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_1ARG_VOID)
FOR_EACH_2ARG_VOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_2ARG_VOID)
FOR_EACH_3ARG_VOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_3ARG_VOID)
FOR_EACH_4ARG_VOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_4ARG_VOID)
FOR_EACH_5ARG_VOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_5ARG_VOID)
FOR_EACH_1ARG_NONVOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_1ARG_NONVOID)
FOR_EACH_2ARG_NONVOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_2ARG_NONVOID)
FOR_EACH_3ARG_NONVOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_3ARG_NONVOID)
FOR_EACH_4ARG_NONVOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_4ARG_NONVOID)
FOR_EACH_5ARG_NONVOID_LIBRARY_FUNCTION(WRAPPED_FUNCTION_5ARG_NONVOID)

#undef WRAPPED_FUNCTION_1ARG_VOID
#undef WRAPPED_FUNCTION_2ARG_VOID
#undef WRAPPED_FUNCTION_3ARG_VOID
#undef WRAPPED_FUNCTION_4ARG_VOID
#undef WRAPPED_FUNCTION_5ARG_VOID
#undef WRAPPED_FUNCTION_1ARG_NONVOID
#undef WRAPPED_FUNCTION_2ARG_NONVOID
#undef WRAPPED_FUNCTION_3ARG_NONVOID
#undef WRAPPED_FUNCTION_4ARG_NONVOID
#undef WRAPPED_FUNCTION_5ARG_NONVOID

// Assumes that sharedmem->funcnum and sharedmem->args have already been set appropriately.
void ProcessSandbox::invokeFunctionCall() {
  sharedmem->invoker.invokeRight();
}
