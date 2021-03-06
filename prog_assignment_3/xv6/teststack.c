/* set up stack correctly (and without extra items) */
#include "types.h"
#include "user.h"

int ppid;
#define PGSIZE (4096)
#define NULL 0

volatile int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED...............\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   printf(1, "Stack test starting ...............\n");
   ppid = getpid();
   void *stack = malloc(PGSIZE*2);
   assert(stack != NULL);
   if((uint)stack % PGSIZE){
     stack = stack + (4096 - (uint)stack % PGSIZE);
   }
   printf(1,"passing stack = %d\n",(uint)stack);
   int clone_pid = clone(worker, stack, stack);
   assert(clone_pid > 0);
   while(global != 5);
   printf(1, "TEST PASSED...............\n");
   exit();
}

void
worker(void *arg_ptr) {
	printf(1,"stack value %d, expected = %d\n",(uint)&arg_ptr,((uint)arg_ptr + PGSIZE - 4));
   assert((uint)&arg_ptr == ((uint)arg_ptr + PGSIZE - 4));
   assert(*((uint*) (arg_ptr + PGSIZE - 8)) == 0xffffffff);
   global = 5;
   exit();
}
