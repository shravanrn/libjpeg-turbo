#include "ProcessSandbox_sharedmem.h"
#include "synch.h"

#include <sys/mman.h>
#include <errno.h>
#include <string.h>  // strerror()
#include <stdlib.h>  // exit()
#include <signal.h>
#include <malloc.h>  // malloc hooks
#include "MyMalloc.h"

#include <stdio.h>  // fopen()
#include <string.h>  // strcmp()
#include <fcntl.h>

static void gotSegfault(int signum, siginfo_t* siginfo, void* ucontext) {
  fprintf(stderr, "Sandbox thread got segfault\n");
  exit(1);
}

static MyMalloc* myMalloc = NULL;

// see gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
//   for more on malloc hooks
//typedef void* (*malloc_hook_t)(size_t, const void*);
//typedef void (*free_hook_t)(void*, const void*);
//static malloc_hook_t oldMallocHook;
//static free_hook_t oldFreeHook;

static void* myMallocHook(size_t size, const void* caller) {
	return myMalloc->malloc(size);
}

static void myFreeHook(void* ptr, const void* caller) {
	return myMalloc->free(ptr);
}

static void initMallocHooks() {
	//oldMallocHook = __malloc_hook;
	//oldFreeHook = __free_hook;
	__malloc_hook = myMallocHook;
	__free_hook = myFreeHook;
}

int main(int argc, char* argv[]) {
  int fd = shm_open(shmempath, O_RDWR, 0);
  void* sharedmem = mmap(SHAREDMEM_VADDR, SHAREDMEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  sharedmem_t* sm = (sharedmem_t*) sharedmem;
  printf("Sandbox thread sees sharedmem=%p\n", sm);

  struct sigaction segvsa;
  segvsa.sa_sigaction = &gotSegfault;
  sigaction(SIGSEGV, &segvsa, NULL);

  // This is allocated before malloc hooks are in place, and that's fine
  // We don't need to worry about synchronizing operations with the other MyMalloc
  //   instance (in main process) (see MyMalloc.h) because the synch scheme (see
  //   synch.h) guarantees that we and the main process can't call malloc() or free()
  //   simultaneously
  myMalloc = new MyMalloc(sm->extraSpace, SHAREDMEM_SIZE-sizeof(sharedmem_t), false);
  initMallocHooks();

  while(true) {
    sm->invoker.invokeLeft();
    // now that invokeLeft has returned, we have been invoked again
    switch(sm->funcnum) {

#define DISPATCH_1ARG_NONVOID(fnum, fname, rettype, arg1type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    sm->retval.as_##rettype = fname(args.arg1); \
    break; \
  }

#define DISPATCH_2ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    sm->retval.as_##rettype = fname(args.arg1, args.arg2); \
    break; \
  }

#define DISPATCH_3ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    sm->retval.as_##rettype = fname(args.arg1, args.arg2, args.arg3); \
    break; \
  }

#define DISPATCH_4ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    sm->retval.as_##rettype = fname(args.arg1, args.arg2, args.arg3, args.arg4); \
    break; \
  }

#define DISPATCH_5ARG_NONVOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type, arg5type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    sm->retval.as_##rettype = fname(args.arg1, args.arg2, args.arg3, args.arg4, args.arg5); \
    break; \
  }

#define DISPATCH_1ARG_VOID(fnum, fname, rettype, arg1type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    fname(args.arg1); \
    break; \
  }

#define DISPATCH_2ARG_VOID(fnum, fname, rettype, arg1type, arg2type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    fname(args.arg1, args.arg2); \
    break; \
  }

#define DISPATCH_3ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    fname(args.arg1, args.arg2, args.arg3); \
    break; \
  }

#define DISPATCH_4ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    fname(args.arg1, args.arg2, args.arg3, args.arg4); \
    break; \
  }

#define DISPATCH_5ARG_VOID(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type, arg5type) \
  case fnum: { \
    struct fname##_args& args = sm->fname##_args; \
    fname(args.arg1, args.arg2, args.arg3, args.arg4, args.arg5); \
    break; \
  }

      FOR_EACH_1ARG_VOID_LIBRARY_FUNCTION(DISPATCH_1ARG_VOID)
      FOR_EACH_2ARG_VOID_LIBRARY_FUNCTION(DISPATCH_2ARG_VOID)
      FOR_EACH_3ARG_VOID_LIBRARY_FUNCTION(DISPATCH_3ARG_VOID)
      FOR_EACH_4ARG_VOID_LIBRARY_FUNCTION(DISPATCH_4ARG_VOID)
      FOR_EACH_5ARG_VOID_LIBRARY_FUNCTION(DISPATCH_5ARG_VOID)
      FOR_EACH_1ARG_NONVOID_LIBRARY_FUNCTION(DISPATCH_1ARG_NONVOID)
      FOR_EACH_2ARG_NONVOID_LIBRARY_FUNCTION(DISPATCH_2ARG_NONVOID)
      FOR_EACH_3ARG_NONVOID_LIBRARY_FUNCTION(DISPATCH_3ARG_NONVOID)
      FOR_EACH_4ARG_NONVOID_LIBRARY_FUNCTION(DISPATCH_4ARG_NONVOID)
      FOR_EACH_5ARG_NONVOID_LIBRARY_FUNCTION(DISPATCH_5ARG_NONVOID)

#undef DISPATCH_1ARG_VOID
#undef DISPATCH_2ARG_VOID
#undef DISPATCH_3ARG_VOID
#undef DISPATCH_4ARG_VOID
#undef DISPATCH_5ARG_VOID
#undef DISPATCH_1ARG_NONVOID
#undef DISPATCH_2ARG_NONVOID
#undef DISPATCH_3ARG_NONVOID
#undef DISPATCH_4ARG_NONVOID
#undef DISPATCH_5ARG_NONVOID

      default: {
        printf("Unknown function number %u\n", sm->funcnum);
        break;
      }
    }
  }
  return 0;
}
