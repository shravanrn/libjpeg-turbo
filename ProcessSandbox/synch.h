#pragma once

#include <pthread.h>

class Invoker {
  public:
    Invoker();
    void invokeLeft();  // only called by Right, to pass control to Left
    void invokeRight();  // only called by Left, to pass control to Right

  protected:
    pthread_mutex_t mutex;  // must hold this to change any state here, signal anything, or wait on anything
    pthread_cond_t left;  // condition variable to wait for an invokeLeft()
    pthread_cond_t right;  // condition variable to wait for an invokeRight()
    bool invokeLeftFlag;
    bool invokeRightFlag;
    static pthread_mutexattr_t* mutexattr;
    static pthread_condattr_t* condattr;
};
