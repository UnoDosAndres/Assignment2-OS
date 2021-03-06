#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


typedef struct memoryList memoryList;
/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given execution;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;
	/* all implementations will need an actual block of memory to use */
	mySize = sz;
	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */
     // points to the same memory adress as the memory pool
	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */
	myMemory = malloc(sz);
	/* TODO: Initialize memory management structure. */
    head = (struct memoryList*) malloc(sizeof (struct memoryList));
    head->last = NULL;
    head->next = NULL;
    head->size = sz; // initialy the first block size is equals to the memory pool size.
    head->alloc = 0;  // not allocated
    head->ptr = myMemory;

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested)
{
    //first we initialize the pointers we're going to need when runnning the algorithms.
	assert((int)myStrategy > 0);
    struct memoryList *new = malloc(sizeof(memoryList));
    struct memoryList *current = head;
    struct memoryList *prev = NULL;
    struct memoryList *next = NULL;
    struct memoryList *best = NULL;
    struct memoryList *worst = NULL;
    new->size = requested;
    new->alloc = 1;
    if (head->size == mySize && head->alloc == 0) {
        new->next = current;
        new->last = NULL;
        current->last = new;
        head = new;
        return NULL;
    }
	switch (myStrategy)
	  {
	  case NotSet:
          // If no strategy is set we return NULL
	            return NULL;
	  case First:
          // in this strategy we loop through the memory block until we find an open space that can house our new block.
          while (1) {
              if (current->size >= new->size && current->alloc == 0) {
                  if (current->size == new->size) {
                      prev = current->last;
                      next = current->next;
                      prev->next = new;
                      new->last = prev;
                      new->next = next;
                      next->last = new;
                      free(current);
                      return NULL;
                  }
                  current->size = current->size - new->size;
                  if (current->last == NULL) {
                      head = new;
                      new->next = current;
                      current->last = new;
                      return NULL;
                  }
                  else if (current->next == NULL) {
                      prev = current->last;
                      prev->next = new;
                      current->last = new;
                      return NULL;
                  }
                  else {
                      prev = current->last;
                      prev->next = new;
                      new->last = prev;
                      new->next = current;
                      return NULL;
                  }
              }
              else {
                  if (current->next == NULL) {
                      return NULL;
                  }
                  current = current->next;
              }
          }
	  case Best:
          // In this, we loop through the block no matter what. When reaching the end, the "best" pointer will be the place in which we put our new memory block.
          while (current != NULL) {
              if (current->size >= new->size && current->alloc == 0) {
                  if (best == NULL) {
                      best = current;
                  }
                  else if (current->size < best->size) {
                    best = current;
                  }
              }
              current = current->next;
          }
              if (best != NULL) {
                  best->size = best->size - new->size;
                  if (best->last == NULL) {
                      head = new;
                      new->next = best;
                      best->last = new;
                  } else {
                      prev = best->last;
                      prev->next = new;
                      new->last = prev;
                      new->next = best;
                      best->last = new;
                  }
                  if (best->size == 0) {
                      next = best->next;
                      new->next = next;
                      next->last = new;
                      free(best);
                  }
              }
	            return NULL;
	  case Worst:
          //Worst works the same way as best but instead of switching pointer when the space is smaller, is changes when we find a place larger.
          while(1) {
              if (current->size <= new->size && current->alloc == 0) {
                  if (worst == NULL) {
                      worst = current;
                  }
                  else if (current->size > worst->size) {
                      worst = current;
                  }
              }
              if (current->next == NULL) {
                  break;
              }
              else {
                  current = current->next;
              }
          }
          if (worst != NULL) {
              worst->size = worst->size - new->size;
              if (worst->last == NULL) {
                  head = new;
                  worst->last = new;
                  new->next = worst;
              }
              else  {
                  prev = worst->last;
                  prev->next = new;
                  new->last = prev;
                  worst->last = new;
                  new->next = next;
              }
              if (worst->size == 0) {
                  next = worst->next;
                  new->next = next;
                  next->last = new;
                  free(worst);
              }
          }
	            return NULL;
	  case Next:
          // Next loops through the memory block, until it finds an allocated block. If the next block is not allocated and can house our new block, we place it there.
          while (1) {
              if (current->alloc == 0 && current->size > new->size) {
                  current->size = current->size - new->size;
                  if (current->last == NULL) {
                      head = new;
                      current->last = new;
                      new->next = current;
                  }
                  else {
                      prev = current->last;
                      prev->next = new;
                      new->last = prev;
                      current->last = new;
                      new->next = current;
                  }
                  if (current->size == 0) {
                      next = current->next;
                      next->last = new;
                      new->next = next;
                      free(current);
                  }
              }
              if (current->next == NULL) {
                  break;
              }
              else {
                  current = current->next;
              }
          }
	            return NULL;
	  }
	return NULL;
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{
    struct memoryList *current = block;
    current->alloc = 0;
    struct memoryList *prev = current->last;
    struct memoryList *next = current->next;

    if (prev->alloc == 0) {
        current->size = prev->size + current->size;
        struct memoryList *secondprev = prev->last;
        secondprev->next = current;
        current->last = secondprev;
        free(prev);
    }
    if (next->alloc == 0) {
        current->size = next->size + current->size;
        struct memoryList *secondnext = next->next;
        secondnext->last = current;
        current->next = secondnext;
        free(next);
    }
	return;
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
    int ctr = 0;
    struct memoryList *current = head;
    while(current != NULL) {
        if (current->alloc == 0) {
            ctr++;
        }
        current = current->next;
    }
	return ctr;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
    int ctr = mySize;
	return ctr;
}

/* Number of non-allocated bytes */
int mem_free()
{
    struct memoryList *current = head;
    int ctr = 0;
    while (current != NULL) {
        if (current->alloc == 0) {
            ctr = ctr + current->size;
        }
        current = current->next;
    }
	return ctr;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
    struct memoryList *current = head;
    int largest = 0;
    while (1) {
        if (current->alloc == 0) {
            if (current->size > largest) {
                largest = current->size;
            }
        }
        if (current->next == NULL) {
            break;
        }
        else {
            current = current->next;
        }
    }
	return largest;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
    int ctr = 0;
    struct memoryList *current = head;
    while (1) {
        if (current->size < size) {
            ctr++;
        }
        if (current->next == NULL) {
            break;
        }
        else {
            current = current->next;
        }
    }
	return ctr;
}       

char mem_is_alloc(void *ptr)
{
    struct memoryList *current = ptr;
        return current->alloc;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
    int ctr = 1;
    struct memoryList *current = head;
    while (1) {
        printf("%d : %d bytes", ctr, current->size);
        if (current->next == NULL) {
            break;
        }
        else {
            current = current->next;
        }
    }
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	
	initmem(strat,500);
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	
	print_memory();
	print_memory_status();
	
}
