 Memory Management
===================

The problem will focus on memory.  You will implement your own 
version of malloc() and free(), using a variety of allocation strategies.

You will be implementing a memory manager for a block of memory.  You will
implement routines for allocating and deallocating memory, and keeping track of
what memory is in use.  You will implement one of the four strategies for selecting in
which block to place a new requested memory block:

  1) First-fit: select the first suitable block with smallest address.
  2) Best-fit: select the smallest suitable block.
  3) Worst-fit: select the largest suitable block.
  4) Next-fit: select the first suitable block after
     the last block allocated (with wraparound
     from end to beginning).


Here, "suitable" means "free, and large enough to fit the new data".

Here are the functions you will need to implement:

initmem():
  Initialize memory structures.

mymalloc():
  Like malloc(), this allocates a new block of memory.

myfree():
  Like free(), this deallocates a block of memory.

mem_holes():
  How many free blocks are in memory?

mem_allocated():
  How much memory is currently allocated?

mem_free():
  How much memory is NOT allocated?

mem_largest_free():
  How large is the largest free block?

mem_small_free():
  How many small unallocated blocks are currently in memory?

mem_is_alloc():
  Is a particular byte allocated or not?

A structure has been provided for use to implement these functions.  It is a
doubly-linked list of blocks in memory (both allocated and free blocks).  Every
malloc and free can create new blocks, or combine existing blocks.  You may
modify this structure, or even use a different one entirely.  However, do not
change function prototypes or files other than mymem.c.

IMPORTANT NOTE: Regardless of how you implement memory management, make sure
that there are no adjacent free blocks.  Any such blocks should be merged into
one large block.

A few functions are given to help you monitor what happens when you
call your functions.  Most important is the try_mymem() function.  If you run
your code with "mem -try <args>", it will call this function, which you can use
to demonstrate the effects of your memory operations.  These functions have no
effect on test code, so use them to your advantage.

Running your code:

After running "make", run

1) "mem" to see the available tests and strategies.
2) "mem -test <test> <strategy>" to test your code with provided tests.
3) "mem -try <args>" to run your code with your own tests
   (the try_mymem function).

You can also use "make test" and "make stage1-test" for testing.  "make
stage1-test" only runs the tests relevant to stage 1.

Running "mem -test -f0 ..." will allow tests to run even
after previous tests have failed.  Similarly, using "all" for a test or strategy
name runs all of the tests or strategies.  Note that if "all" is selected as the
strategy, the 4 tests are shown as one.

One of the tests, "stress", runs an assortment of randomized tests on each
strategy.  The results of the tests are placed in "tests.out" .  You may want to
view this file to see the relative performance of each strategy.


Stage 1
-------

Implement all the above functions, for all the 4 strategy in a group
Use "mem -test all first" to test your implementation


Stage 2
-------
you should answer the 10 questions asked below together in a group.
The strategy is passed to initmem(), and stored in the global variable "myStrategy".
Some of your functions will need to check this variable to implement the
correct strategy.

You can test your code with "mem -test all worst", etc., or test all 4 together
with "mem -test all all".  The latter command does not test the strategies
separately; your code passes the test only if all four strategies pass.


Answer the following questions as part of your report
=====================================================

1) Why is it so important that adjacent free blocks not be left as such?  What
would happen if they were permitted?
If we want to allocate something, we need to have a way to identify  the set of free blocks.
To find the free blocks, we need a way to choose which of them to allocate.
When we have decided which to allocate and it dont fit, we should decide what to do with the left-overs fragment.
A block will only have one pointer, when it’s free and from this pointer can we find the size of the block.
When the size is found, is it important to find out if the block has any adjacent free blocks,
it is needed to know where their location is. We need the location to coalesce them into a single free block.
That means they become a single block instead of two or more.
It is important to coalesce the adjective free block because, if not, the storage becomes fragmented into
smaller and smaller blocks. Efter time the blocks will be so small,
that it doesn't have the option to allocate any larger blocks.


2) Which function(s) need to be concerned about adjacent free blocks?
mem_holes() function: this function finds out how many free blocks there are in memory.
mem_free() function: this function knows how much memory there is NOT allocated.
mem_largest_free() function : this function holds track on how large the largest free block is.



3) Name one advantage of each strategy.
First fit: First fit has faster processing, because it takes the first available memory,
as long as there is room for it.  The bad thing is that it wastes a lot of memory this way

Best fit: This is more memory efficient, since it looks for the memory with the least possible space.
But the bad thing is that it takes the longest because of the process it has to go through to find
the most memory efficient one.

Worst fit: Worst fit chooses the largest hole, which means there will be a large internal fragmentation.
When the internal fragmentation is that big, the smaller processes will also be placed in the leftover hole.

Next fit: next fit is a modified version of first fit and it has a very fast searching algorithm,
it starts searching from where it left off, therefore it is faster than first fit and best fit.


4) Run the stress test on all strategies, and look at the results (tests.out).
What is the significance of "Average largest free block"?  Which strategy
generally has the best performance in this metric?  Why do you think this is?

5) In the stress test results (see Question 4), what is the significance of
"Average number of small blocks"?  Which strategy generally has the best
performance in this metric?  Why do you think this is?

6) Eventually, the many mallocs and frees produces many small blocks scattered
across the memory pool.  There may be enough space to allocate a new block, but
not in one place.  It is possible to compact the memory, so all the free blocks
are moved to one large free block.  How would you implement this in the system
you have built?
Provided that all of these still exist in the same pool, a sorting algorithm might
be a good way to make sure that the space is used efficiently, ensuring that no non-allocated
block stands alone. From there you just fuse all these blocks into one.

7) If you did implement memory compaction, what changes would you need to make
in how such a system is invoked (i.e. from a user's perspective)?
You would need to move all empty spare memories to 1 bigger one in order to use the space more.
If a lot of memories have been used a couple of times they will have gotten too small to fit any other memories,
so we mush them together in order for there to be room for new ones.


8) How would you use the system you have built to implement realloc?  (Brief
explanation; no code)
Here you can make a function that takes a pointer to a memoryList, and the new size.

From there, you would resize the memoryList according to the received size,
and create a non-allocated block next to its next pointer, making a new empty space on the memory block.


9) Which function(s) need to know which strategy is being used?  Briefly explain
why this/these and not others.
The only function that needs to know the strategy is mymalloc,
since it directly affects how the memoryList is allocated.


10) Give one advantage of implementing memory management using a linked list
over a bit array, where every bit tells whether its corresponding byte is
allocated.
The linked list gives a more detailed view of what element exist in the pool.
If you are unable to distinguish between them, as you would be with a bit array,
you would be unable to run a sorting algorithm. Therefore memory compaction would be impossible.
