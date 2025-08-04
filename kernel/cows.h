//lab6-1
// #include "types.h"        
// #include "defs.h"         
// #include "param.h"        
// #include "memlayout.h"    
// #include "spinlock.h" 
struct {
  uint8 ref_cnt;
  struct spinlock lock;
} cows[(PHYSTOP - KERNBASE) >> 12];
