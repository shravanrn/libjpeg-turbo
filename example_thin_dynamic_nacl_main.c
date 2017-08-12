
//For dynamic lib loading
#include <dlfcn.h>

//For NaCl sandboxing
#include "dyn_ldr_lib.h"

int invokeMainFunc(NaClSandbox* sandbox, void* mainFuncPtr, char* input, char* output)
{
  printf("input: %s\n", input);
  printf("output: %s\n", output);

  char* inputPtr = (char*) mallocInSandbox(sandbox, sizeof(char) * (strlen(input) + 1));
  char* outputPtr = (char*) mallocInSandbox(sandbox, sizeof(char) * (strlen(output) + 1));
  strcpy(inputPtr, input);
  strcpy(outputPtr, output);

  preFunctionCall(sandbox, sizeof(inputPtr) + sizeof(outputPtr), 0);
  PUSH_PTR_TO_STACK(sandbox, char*, inputPtr);
  PUSH_PTR_TO_STACK(sandbox, char*, outputPtr);
  invokeFunctionCall(sandbox, mainFuncPtr);

  freeInSandbox(sandbox, inputPtr);
  freeInSandbox(sandbox, outputPtr);

  return (int)functionCallReturnRawPrimitiveInt(sandbox);

  // preFunctionCall(sandbox, sizeof(input) + sizeof(output), STRING_SIZE(input) + STRING_SIZE(output));
  // PUSH_STRING_TO_STACK(sandbox, input);
  // PUSH_STRING_TO_STACK(sandbox, output);
  // invokeFunctionCall(sandbox, mainFuncPtr);
  // return (int)functionCallReturnRawPrimitiveInt(sandbox);
}

char* invokeAddTest(NaClSandbox* sandbox, void* addTestPtr, char* input, char* output)
{
  preFunctionCall(sandbox, sizeof(input) + sizeof(output), STRING_SIZE(input) + STRING_SIZE(output));
  PUSH_STRING_TO_STACK(sandbox, input);
  PUSH_STRING_TO_STACK(sandbox, output);  
  invokeFunctionCall(sandbox, addTestPtr);
  return (char*)functionCallReturnPtr(sandbox);
}

int main(int argc, char** argv)
{
  if(argc < 6)
  {
    printf("No io files specified. Expected arg example input.jpeg output.jpeg example_thin_dynamic_nacl_main.so naclLibraryPath sandboxInitApp\n");
    return 1;
  }

  printf("Creating NaCl Sandbox");

  initializeDlSandboxCreator(0 /* Should enable detailed logging */);
  NaClSandbox* sandbox = createDlSandbox(argv[4], argv[5]);

  if(!sandbox)
  {
    printf("Error creating sandbox");
    return 0;
  }

  printf("Loading dynamic library %s\n", argv[3]);

  void* dlPtr = dlopenInSandbox(sandbox, argv[3], RTLD_LAZY);

  if(!dlPtr)
  {
    printf("Loading of dynamic library %s has failed. Err: %s\n", argv[3], dlerrorInSandbox(sandbox));
    return 0;
  }

  printf("Loaded dynamic library %s\n", argv[3]);

  void* addTestPtr = dlsymInSandbox(sandbox, dlPtr, "addTest");
  if(addTestPtr == NULL) 
  { 
  	printf("Symbol resolution failed for addTest\n"); 
  	dlcloseInSandbox(sandbox, dlPtr);
  	return 1; 
  }

  char* res = invokeAddTest(sandbox, addTestPtr, argv[1], argv[2]);
  printf("Result: %s\n", res);
  // if(res == 11) { printf("Succeeded\n"); } else { printf("Failed\n"); }
  

  // void* mainFuncPtr = dlsymInSandbox(sandbox, dlPtr, "mainFunc");
  // if(mainFuncPtr == NULL) 
  // { 
  // 	printf("Symbol resolution failed for mainFuncPtr\n"); 
  // 	dlcloseInSandbox(sandbox, dlPtr);
  // 	return 1; 
  // }

  // int ret = invokeMainFunc(sandbox, mainFuncPtr, argv[1], argv[2]);
  int ret = 0;
  dlcloseInSandbox(sandbox, dlPtr);
  return ret;
}