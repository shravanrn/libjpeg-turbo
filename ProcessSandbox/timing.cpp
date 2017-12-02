#include "timing.h"

uint64_t startTimer() {
  // With reference to the white paper "How to Benchmark Code Execution Times on Intel IA-32 and IA-64 Instruction Set Architectures" by Gabriele Paoloni, Intel
  unsigned highword, lowword;
  asm volatile (
      "CPUID\n\t"
      "RDTSC\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      : "=r" (highword), "=r" (lowword)
      :: "%rax", "%rbx", "%rcx", "%rdx"
  );
  return ( ((uint64_t)highword << 32) | lowword );
}

uint64_t stopTimer() {
  // With reference to the white paper "How to Benchmark Code Execution Times on Intel IA-32 and IA-64 Instruction Set Architectures" by Gabriele Paoloni, Intel
  unsigned highword, lowword;
  asm volatile (
#ifdef NO_RDTSCP
      // Paoloni's suggestion, but must be executed with higher privileges
      //"mov %%cr0, %%rax\n\t"
      //"mov %%rax, %%cr0\n\t"
      // Instead we omit these two lines, running CPUID instead
      "CPUID\n\t"
      "RDTSC\n\t"
#else
      "RDTSCP\n\t"
#endif
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
#ifdef NO_RDTSCP
#else
      "CPUID\n\t"  // run CPUID if we didn't earlier
#endif
      : "=r" (highword), "=r" (lowword)
      :: "%rax", "%rbx", "%rcx", "%rdx"
  );
  return ( ((uint64_t)highword << 32) | lowword );
}

