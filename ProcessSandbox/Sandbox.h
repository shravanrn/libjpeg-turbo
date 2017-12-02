#pragma once

#include <stdint.h>
#include "jpeglib_helpers.h"

class Sandbox {
  public:
    Sandbox();

    // Returns NULL on failure
    virtual void* mallocInSandbox(uint32_t size) = 0;

    virtual void freeInSandbox(void* ptr) = 0;

    virtual void destroySandbox() = 0;

    // Wrapped versions of the library functions
    // Call these to "invoke" each library function
#define SIGNATURE(fnum, fname, rettype, ...) \
    virtual rettype (inv_##fname) (__VA_ARGS__) = 0;

    FOR_EACH_LIBRARY_FUNCTION(SIGNATURE)

#undef SIGNATURE
};
