#pragma once

#include "Sandbox.h"
#include "ProcessSandbox_sharedmem.h"
#include "MyMalloc.h"

class ProcessSandbox : public Sandbox {
  public:
    ProcessSandbox();

    void* mallocInSandbox(uint32_t size);
    void freeInSandbox(void* ptr);

    void destroySandbox();

#define SIGNATURE(fnum, fname, rettype, ...) \
    rettype (inv_##fname) (__VA_ARGS__);

    FOR_EACH_LIBRARY_FUNCTION(SIGNATURE)

#undef SIGNATURE

  protected:
    sharedmem_t* sharedmem;
    MyMalloc* myMalloc;
    pid_t sandboxPID;
    static const uint64_t STACK_SIZE = 1*MBYTE;  // stack size for sandboxed process

  private:
    pid_t createSandboxProcess();
    static int startOtherside(void* arg);
    static sharedmem_t* createSharedMem();
    void invokeFunctionCall();
};
