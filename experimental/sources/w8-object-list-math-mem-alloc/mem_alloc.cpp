// Observed memAlloc__FiPc at 0x144368, extent 492.
// First-fit allocation over the circular block list. In order:
//   1. size = max(request, 64) (signed); memCheckHeapReal("mem.cc", 502).
//   2. The scan starts at startNode (0x4868b4) and visits blocks through the next
//      links until it returns to the start. A block is skipped unless its state is
//      0. If the payload (block + 28) is not 256-byte aligned, the block must also
//      leave room for a prefix: with base = rawMin + 28 and candidate =
//      (int)(block address + base), the displacement is
//      base - (candidate - candidate/256*256) + 256 (division truncating toward
//      zero) and the needed size is size + displacement; otherwise needed = size.
//      needed is rounded up to a multiple of 8. A block fits when its size is not
//      below needed (signed).
//   3. A misaligned fit is split first: a new free header is created at
//      block + (displacement - 56) + 28, the old block keeps displacement - 56
//      bytes, and the new header becomes the block; the payload is then at
//      old block + displacement.
//   4. If the remainder (size - needed) is above rawMin (signed), a free tail
//      header is created at block + needed + 28 with size remainder - 28 and the
//      block shrinks to needed.
//   5. MEM_FreeHeap -= block size + 28; the block gets the name pointer, state 1
//      and the global tag word `mark`; movingNode becomes the block's next link
//      and the block's caller word is the return address of this call.
// With no fit after a full circle: memDumpHeap, scePrintf of the failure line with
// the return address and the clamped size, a store of zero to address 0 (an
// intentional fault in the original), then a null return if execution continues.
// Authored providers used: norrath_mem_dump_heap for memDumpHeap, scePrintf, and
// the norrath_heap_* words of memory-heap-state for startNode, movingNode,
// rawMin, MEM_FreeHeap and mark.
#include "../w8-object-list-math-common/heap_block.hpp"

extern "C" void memCheckHeapReal__FPCci(const char* file, int line);
extern "C" void norrath_mem_dump_heap();
extern "C" int scePrintf(const char* format, ...);
extern "C" HeapBlock* norrath_heap_ring_word_view;           // startNode
extern "C" HeapBlock* norrath_heap_next_fit_view;            // movingNode
extern "C" int norrath_heap_fragment_threshold_view;         // rawMin
extern "C" int norrath_heap_free_bytes_view;                 // MEM_FreeHeap
extern "C" unsigned norrath_heap_allocation_tag_view;        // mark

#if !defined(__mips__)
extern "C" void host_heap_failure_store();
#endif

namespace {
inline void store_zero_to_address_zero() {
#if defined(__mips__)
    __asm__ volatile("sw $0, 0($0)" ::: "memory");
#else
    host_heap_failure_store();
#endif
}
}  // namespace

extern "C" void* memAlloc__FiPc(int request, char* name) {
    const unsigned caller = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(__builtin_return_address(0)));
    const int size = request > 64 ? request : 64;
    memCheckHeapReal__FPCci("mem.cc", 502);

    HeapBlock* const start = norrath_heap_ring_word_view;
    HeapBlock* block = start;
    char* payload = nullptr;
    int displacement = -1;
    int needed = 0;
    bool misaligned = false;
    for (;;) {
        if (block->state == 0) {
            payload = payload_of(block);
            misaligned = (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(payload)) & 255u) != 0;
            needed = size;
            if (misaligned) {
                const int base = norrath_heap_fragment_threshold_view + kHeaderBytes;
                const int candidate = static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block)) + static_cast<unsigned>(base));
                const int quotient = candidate / 256;
                const int remainder = candidate - quotient * 256;
                displacement = base - remainder + 256;
                needed = size + displacement;
            }
            needed = (needed + 7) & -8;
            if (!(block->size < needed)) break;
        }
        block = block->next;
        if (block == start) {
            norrath_mem_dump_heap();
            scePrintf("memAlloc failed!  (ra=%x, size=%d)\n", caller, size);
            store_zero_to_address_zero();
            return nullptr;
        }
    }

    int available = block->size;
    if (misaligned) {
        const int prefix = displacement - 2 * kHeaderBytes;
        HeapBlock* split = reinterpret_cast<HeapBlock*>(reinterpret_cast<char*>(block) + prefix + kHeaderBytes);
        HeapBlock* following = block->next;
        split->prev = block;
        split->next = following;
        split->size = available - prefix - kHeaderBytes;
        payload = reinterpret_cast<char*>(block) + prefix + 2 * kHeaderBytes;
        block->next->prev = split;
        block->size = prefix;
        block->next = split;
        block = split;
        block->magic = kHeaderMagic;
        block->state = 0;
        block->name = nullptr;
        available = block->size;
    }
    const int remainder = available - needed;
    if (norrath_heap_fragment_threshold_view < remainder) {
        HeapBlock* tail = reinterpret_cast<HeapBlock*>(reinterpret_cast<char*>(block) + needed + kHeaderBytes);
        tail->next = block->next;
        available = needed;
        tail->prev = block;
        tail->size = remainder - kHeaderBytes;
        block->next->prev = tail;
        tail->magic = kHeaderMagic;
        tail->state = 0;
        block->next = tail;
        tail->name = nullptr;
        block->size = needed;
    }
    norrath_heap_free_bytes_view = norrath_heap_free_bytes_view - available - kHeaderBytes;
    block->name = name;
    block->state = 1;
    block->tag = norrath_heap_allocation_tag_view;
    norrath_heap_next_fit_view = block->next;
    block->caller = caller;
    return payload;
}
