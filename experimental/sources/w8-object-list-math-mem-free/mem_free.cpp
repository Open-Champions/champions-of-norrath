// Observed memFree__FPv at 0x144558, extent 232.
// The block header sits 28 bytes before the pointer. Order of effects:
//   1. memCheckHeapReal("mem.cc", 583).
//   2. MEM_FreeHeap += size + 28; movingNode = this block; state = 0; name = 0.
//   3. If the next block is free (state 0) and at a higher address: mark that
//      header state 2, point the block after it back at this block, add its size
//      plus 28 to this block's size, and take its next link.
//   4. If the previous block is free and at a lower address: give it this
//      block's next link, add this block's size plus 28 to its size, mark this
//      header state 2, point the following block's previous link at it, and
//      move movingNode to it.
// A free next or previous block at a lower or higher address respectively (the
// ring wrap) is not merged. There is no check that the pointer was allocated.
// The authored views of MEM_FreeHeap and movingNode are the norrath_heap_*
// words from memory-heap-state.
#include "../w8-object-list-math-common/heap_block.hpp"

extern "C" void memCheckHeapReal__FPCci(const char* file, int line);
extern "C" int norrath_heap_free_bytes_view;
extern "C" HeapBlock* norrath_heap_next_fit_view;

extern "C" void memFree__FPv(void* pointer) {
    HeapBlock* block = block_of(pointer);
    memCheckHeapReal__FPCci("mem.cc", 583);
    const int size = block->size;
    HeapBlock* following = block->next;
    norrath_heap_free_bytes_view += size + kHeaderBytes;
    norrath_heap_next_fit_view = block;
    block->state = 0;
    block->name = nullptr;
    if (following->state == 0 && block < following) {
        following->state = 2;
        following->next->prev = block;
        block->size = size + following->size + kHeaderBytes;
        block->next = block->next->next;
    }
    HeapBlock* before = block->prev;
    if (before->state == 0 && before < block) {
        const int merged = block->size;
        before->next = block->next;
        block->state = 2;
        before->size += merged + kHeaderBytes;
        block->next->prev = block->prev;
        norrath_heap_next_fit_view = block->prev;
    }
}
