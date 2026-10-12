// Host contract for memRealloc__FPvi. Not an EE plan input.
// Build: c++ -std=c++17 mem_realloc.cpp host_check.cpp
#include "../w8-object-list-math-common/heap_host.hpp"
using namespace host;

extern "C" void* memRealloc__FPvi(void*, int);

static int alloc_calls, free_calls, alloc_request, alloc_name_empty, shrink_old_size_before_copy = -1;
static void* alloc_result;
static void* freed;
static HeapBlock* old_block_for_mutation;
extern "C" void* memAlloc__FiPc(int size, char* name) {
    ++alloc_calls;
    alloc_request = size;
    alloc_name_empty = name != nullptr && name[0] == 0;
    if (shrink_old_size_before_copy >= 0) old_block_for_mutation->size = shrink_old_size_before_copy;
    return alloc_result;
}
extern "C" void memFree__FPv(void* p) { ++free_calls; freed = p; }

int main() {
    Heap heap;
    const int h = kHeaderBytes;
    static unsigned char fresh[2048];

    // Clamp to 64: a request below 64 of a 64-byte block is a no-op.
    {
        const int sizes[] = {64, 64};
        const unsigned short states[] = {1, 1};
        heap.build(2, sizes, states);
        void* p = payload_of(heap.blocks[0]);
        expect(memRealloc__FPvi(p, 1) == p && alloc_calls == 0 && free_calls == 0, "tiny request on a 64-byte block changes nothing");
        expect(memRealloc__FPvi(p, -5) == p && alloc_calls == 0, "negative request is raised to 64 too");
    }
    // Growth: a new block, the old payload copied with the old size, nothing freed.
    {
        const int sizes[] = {100, 64};
        const unsigned short states[] = {1, 1};
        heap.build(2, sizes, states);
        HeapBlock* b = heap.blocks[0];
        char* p = payload_of(b);
        for (int i = 0; i < 100; ++i) p[i] = static_cast<char>(i + 1);
        std::memset(fresh, 0, sizeof fresh);
        alloc_result = fresh;
        void* result = memRealloc__FPvi(p, 101);
        expect(result == fresh && alloc_calls == 1 && alloc_request == 101 && alloc_name_empty == 1, "memAlloc(request, \"\") and its pointer returned");
        expect(std::memcmp(fresh, p, 100) == 0 && fresh[100] == 0, "old size bytes copied, no more");
        expect(free_calls == 0 && b->size == 100, "old block neither freed nor changed");
        // A request of 70 against 64 also grows, clamped value 70.
        const int sizes2[] = {64, 64};
        heap.build(2, sizes2, states);
        alloc_calls = 0;
        memRealloc__FPvi(payload_of(heap.blocks[0]), 70);
        expect(alloc_calls == 1 && alloc_request == 70, "request above the old size allocates");
        // The copy length is the size word read after memAlloc returns.
        const int sizes3[] = {100, 64};
        heap.build(2, sizes3, states);
        b = heap.blocks[0];
        p = payload_of(b);
        std::memset(p, 0x33, 100);
        std::memset(fresh, 0, sizeof fresh);
        old_block_for_mutation = b;
        shrink_old_size_before_copy = 10;
        memRealloc__FPvi(p, 128);
        shrink_old_size_before_copy = -1;
        expect(fresh[9] == 0x33 && fresh[10] == 0, "length is reloaded from the header after the allocation");
    }
    // Slack of 128 or less keeps the block as is; 129 splits.
    {
        const int sizes[] = {300, 64};
        const unsigned short states[] = {1, 1};
        heap.build(2, sizes, states);
        HeapBlock* b = heap.blocks[0];
        free_calls = 0;
        expect(memRealloc__FPvi(payload_of(b), 172) == payload_of(b) && free_calls == 0 && b->size == 300, "slack 128 returns the same pointer, no split");
        void* p = memRealloc__FPvi(payload_of(b), 171);
        expect(p == payload_of(b) && free_calls == 1, "slack 129 splits");
        const int rounded = 176;  // 171 rounded up to a multiple of 8
        HeapBlock* tail = heap.blocks[0]->next;
        expect(b->size == rounded && reinterpret_cast<char*>(tail) == reinterpret_cast<char*>(b) + rounded + h, "block shrinks to the rounded size, tail follows");
        expect(tail->size == 300 - rounded - h && tail->magic == kHeaderMagic && tail->state == 1 && tail->name == nullptr, "tail header is built as allocated, then freed");
        expect(freed == payload_of(tail) && tail->prev == b && tail->next == heap.blocks[1] && heap.blocks[1]->prev == tail, "tail linked into the ring and passed to memFree");
    }
    // Rounding: an exact multiple of eight is kept.
    {
        const int sizes[] = {1000, 64};
        const unsigned short states[] = {1, 1};
        heap.build(2, sizes, states);
        memRealloc__FPvi(payload_of(heap.blocks[0]), 200);
        expect(heap.blocks[0]->size == 200 && heap.blocks[0]->next->size == 1000 - 200 - h, "multiples of eight are not rounded up");
    }
    if (failures) return 1;
    std::printf("memRealloc contract passed\n");
    return 0;
}
