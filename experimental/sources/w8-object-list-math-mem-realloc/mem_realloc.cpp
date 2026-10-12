// Observed memRealloc__FPvi at 0x144278, extent 240.
// The request is raised to at least 64 (signed compare). Let old be the size
// word of the block header 28 bytes before the pointer.
//   old < request: call memAlloc(request, "") and memcpy into it, reloading the
//     header size word after the allocation, and return the new pointer. The old
//     block is not freed and the copy length is the old size.
//   old - request < 129 (signed): return the same pointer.
//   otherwise round the request up to a multiple of 8, carve a tail header at
//     block + rounded + 28 (magic 0xABCD, state 1, size old - rounded - 28, name
//     0, linked after the block), set the block's size to the rounded value, and
//     memFree the tail's payload so it merges with neighbors. Returns the same
//     pointer.
// The name argument is the empty string at 0x483b10 in the image; a local empty
// literal is used here.
#include "../w8-object-list-math-common/heap_block.hpp"

extern "C" void* memAlloc__FiPc(int size, char* name);
extern "C" void memFree__FPv(void* pointer);
extern "C" void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

extern "C" void* memRealloc__FPvi(void* pointer, int request) {
    HeapBlock* block = block_of(pointer);
    if (request < 64) request = 64;
    const int old_size = block->size;
    if (old_size < request) {
        static char empty_name[] = "";
        void* fresh = memAlloc__FiPc(request, empty_name);
        memcpy(fresh, pointer, static_cast<__SIZE_TYPE__>(block->size));
        return fresh;
    }
    if (old_size - request >= 129) {
        const int rounded = (request + 7) & -8;
        HeapBlock* tail = reinterpret_cast<HeapBlock*>(reinterpret_cast<char*>(block) + rounded + kHeaderBytes);
        tail->next = block->next;
        tail->prev = block;
        tail->size = old_size - rounded - kHeaderBytes;
        block->next->prev = tail;
        tail->magic = kHeaderMagic;
        tail->state = 1;
        block->next = tail;
        block->size = rounded;
        tail->name = nullptr;
        memFree__FPv(payload_of(tail));
    }
    return pointer;
}
