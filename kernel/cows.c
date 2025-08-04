//lab6-1
#include "types.h"         
#include "param.h"        
#include "memlayout.h"    
#include "spinlock.h" 
#include "sleeplock.h"
#include "riscv.h"
#include "cows.h"       
#include "defs.h" 
// increase the reference count
void increfcnt(uint64 pa) {
  if (pa < KERNBASE) {
    return;
  }
  pa = (pa - KERNBASE) >> 12;
  acquire(&cows[pa].lock);
  ++cows[pa].ref_cnt;
  release(&cows[pa].lock);
}

// decrease the reference count
uint8 decrefcnt(uint64 pa) {
  uint8 ret;
  if (pa < KERNBASE) {
    return 0;
  }
  pa = (pa - KERNBASE) >> 12;
  acquire(&cows[pa].lock);
  ret = --cows[pa].ref_cnt;
  release(&cows[pa].lock);
  return ret;
}
