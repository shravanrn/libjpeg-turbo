#include "MyMalloc.h"
#include <stdlib.h>  // NULL

MyMalloc::MyMalloc(void* baseaddr, uintptr_t sizeInBytes, bool init)
  : baseaddr((struct Slot*)baseaddr), endaddr((uintptr_t)baseaddr + sizeInBytes)
{
	if(init) {
		this->baseaddr->inUse = false;
		this->baseaddr->previousSlot = NULL;
		this->baseaddr->nextSlot = NULL;
	}
}

void* MyMalloc::malloc(uintptr_t sizeInBytes) {
  // round up to next multiple of granularity
  sizeInBytes = (((sizeInBytes-1)/granularity)+1)*granularity;

  Slot* slot = baseaddr;
  while(slot != NULL) {
    uintptr_t size = sizeOfSlot(slot);
    if(!slot->inUse && size >= sizeInBytes) {
      slot->inUse = true;
      if(size >= sizeInBytes + sizeof(Slot) + granularity) {
        // enough room to make a new Slot with the leftover space
        Slot* oldNextSlot = slot->nextSlot;
        Slot* newNextSlot = (Slot*) ((uintptr_t)&slot->data + sizeInBytes);
        slot->nextSlot = newNextSlot;
        newNextSlot->inUse = false;
        newNextSlot->previousSlot = slot;
        newNextSlot->nextSlot = oldNextSlot;
        if(oldNextSlot) oldNextSlot->previousSlot = newNextSlot;
      } else {
        // just give this Slot all the space
        // no need to adjust anyone's nextSlot or previousSlot
      }
      break;
    }
    slot = slot->nextSlot;
  }
  if(slot == NULL) return NULL;  // not enough room
  return &slot->data;
}

void MyMalloc::free(void* ptr) {
  Slot* slot = (Slot*) ((uintptr_t)ptr-sizeof(Slot));
    // assume 'slot' is a valid Slot, i.e. assume 'ptr' was previously returned
    //   from malloc() and thus is the ->data field of a valid Slot

  Slot* previousSlot = slot->previousSlot;
  if(previousSlot == NULL || previousSlot->inUse) {
    // don't actually delete the Slot structure, just mark it free
    slot->inUse = false;
  } else {
    // give all this slot's space, plus the space taken up by its Slot structure,
    //   to previous Slot
    Slot* nextSlot = slot->nextSlot;
    previousSlot->nextSlot = nextSlot;
    if(nextSlot) nextSlot->previousSlot = previousSlot;
  }
}

uintptr_t MyMalloc::sizeOfSlot(struct MyMalloc::Slot* slot) {
  Slot* nextSlot = slot->nextSlot;
  if(nextSlot) {
    return ((uintptr_t)nextSlot - (uintptr_t)slot->data);
  } else {
    return (endaddr - (uintptr_t)slot->data);
  }
}
