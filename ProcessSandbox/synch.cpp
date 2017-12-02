#include "synch.h"
#include <stdlib.h>
#include <stdio.h>

static pthread_mutexattr_t* getMutexAttr() {
  pthread_mutexattr_t* mutexattr = (pthread_mutexattr_t*)malloc(sizeof(pthread_mutexattr_t));
  pthread_mutexattr_init(mutexattr);
  pthread_mutexattr_setpshared(mutexattr, PTHREAD_PROCESS_SHARED);  // allow mutex to be used across processes
  pthread_mutexattr_settype(mutexattr, PTHREAD_MUTEX_NORMAL);  // don't need reentrant
  pthread_mutexattr_setrobust(mutexattr, PTHREAD_MUTEX_STALLED);  // deadlock if the owner dies, that's fine
  pthread_mutexattr_setprotocol(mutexattr, PTHREAD_PRIO_PROTECT);  // Experiments on 10/13/17 show that PTHREAD_PRIO_PROTECT gives about 21% speedup in overall function call overhead vs PTHREAD_PRIO_NONE, and PTHREAD_PRIO_NONE gives about 12% speedup vs PTHREAD_PRIO_INHERIT, for a total speedup of about 36% from INHERIT to PROTECT.  Some things have changed since, so it's possible these numbers have changed now.
  return mutexattr;
}

static pthread_condattr_t* getCondAttr() {
  pthread_condattr_t* condattr = (pthread_condattr_t*)malloc(sizeof(pthread_condattr_t));
  pthread_condattr_init(condattr);
  pthread_condattr_setpshared(condattr, PTHREAD_PROCESS_SHARED);
  return condattr;
}

pthread_mutexattr_t* Invoker::mutexattr = NULL;
pthread_condattr_t* Invoker::condattr = NULL;

Invoker::Invoker() {
  invokeLeftFlag = false;
  invokeRightFlag = false;
  if(!mutexattr) mutexattr = getMutexAttr();
  if(!condattr) condattr = getCondAttr();
  pthread_mutex_init(&mutex, mutexattr);
  pthread_cond_init(&left, condattr);
  pthread_cond_init(&right, condattr);
}

void Invoker::invokeLeft() {
  pthread_mutex_lock(&mutex);

  // signal Left to run
  invokeLeftFlag = true;
  pthread_cond_signal(&left);

  // wait for Left to finish running (and signal us, Right)
  while(!invokeRightFlag) {
    pthread_cond_wait(&right, &mutex);  // this also releases the mutex so Left can run
  }

  invokeRightFlag = false;  // acknowledge that we have been invoked again
  pthread_mutex_unlock(&mutex);
}

void Invoker::invokeRight() {
  pthread_mutex_lock(&mutex);

  // signal Right to run
  invokeRightFlag = true;
  pthread_cond_signal(&right);

  // wait for Right to finish running (and signal us, Left)
  while(!invokeLeftFlag) {
    pthread_cond_wait(&left, &mutex);  // this also releases the mutex so Right can run
  }

  invokeLeftFlag = false;  // acknowledge that we have been invoked again
  pthread_mutex_unlock(&mutex);
}
