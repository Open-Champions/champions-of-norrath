// Host contract for memFree__FPv. Not an EE plan input.
// Build: c++ -std=c++17 mem_free.cpp host_check.cpp
#include "../w8-object-list-math-common/heap_host.hpp"
using namespace host;

extern "C" int norrath_heap_free_bytes_view = 1000;
extern "C" HeapBlock* norrath_heap_next_fit_view = nullptr;
extern "C" void memFree__FPv(void*);

static const char* check_file;
static int check_line, check_calls;
extern "C" void memCheckHeapReal__FPCci(const char* file, int line) { check_file = file; check_line = line; ++check_calls; }

int main() {
    Heap heap;
    const int h = kHeaderBytes;

    // free, used, free: freeing the middle merges both neighbours.
    {
        const int sizes[] = {128, 256, 64, 96};
        const unsigned short states[] = {0, 1, 0, 1};
        heap.build(4, sizes, states);
        HeapBlock *a = heap.blocks[0], *b = heap.blocks[1], *c = heap.blocks[2], *d = heap.blocks[3];
        norrath_heap_free_bytes_view = 1000;
        memFree__FPv(payload_of(b));
        expect(check_calls == 1 && std::strcmp(check_file, "mem.cc") == 0 && check_line == 583, "heap check is requested first with mem.cc, 583");
        expect(norrath_heap_free_bytes_view == 1000 + 256 + h, "free bytes grow by size + header");
        expect(c->state == 2 && b->state == 2, "absorbed headers are marked state 2");
        expect(a->state == 0 && a->size == 128 + (256 + 64 + h) + h, "previous block absorbs the merged block and its header");
        expect(a->next == d && d->prev == a, "ring links skip the absorbed headers");
        expect(norrath_heap_next_fit_view == a, "movingNode moves to the surviving block");
        expect(heap.ring_length(a) == 2, "two headers remain");
    }
    // used, used, free, used: only the following block merges.
    {
        const int sizes[] = {64, 200, 80, 64};
        const unsigned short states[] = {1, 1, 0, 1};
        heap.build(4, sizes, states);
        HeapBlock *b = heap.blocks[1], *c = heap.blocks[2], *d = heap.blocks[3];
        memFree__FPv(payload_of(b));
        expect(b->state == 0 && b->name == nullptr && c->state == 2, "block freed, following free block absorbed");
        expect(b->size == 200 + 80 + h && b->next == d && d->prev == b, "size and links updated");
        expect(norrath_heap_next_fit_view == b, "movingNode is the freed block when only the next merges");
    }
    // used, free, used: only the previous block merges.
    {
        const int sizes[] = {64, 64, 64};
        const unsigned short states[] = {1, 0, 1};
        heap.build(3, sizes, states);
        HeapBlock *a = heap.blocks[0], *b = heap.blocks[1], *c = heap.blocks[2];
        memFree__FPv(payload_of(c));
        expect(b->size == 64 + 64 + h && b->next == a && a->prev == b && c->state == 2, "previous block absorbs the freed one");
        expect(norrath_heap_next_fit_view == b, "movingNode is the previous block");
    }
    // Ring wrap: the last block's successor is the first block at a lower address.
    {
        const int sizes[] = {64, 64, 64};
        const unsigned short states[] = {0, 1, 1};
        heap.build(3, sizes, states);
        HeapBlock *a = heap.blocks[0], *c = heap.blocks[2];
        memFree__FPv(payload_of(c));
        expect(c->state == 0 && a->state == 0 && a->size == 64 && c->next == a, "a free successor at a lower address is not merged");
        const unsigned short states2[] = {1, 1, 0};
        heap.build(3, sizes, states2);
        a = heap.blocks[0];
        memFree__FPv(payload_of(a));
        expect(a->state == 0 && heap.blocks[2]->state == 0 && heap.blocks[2]->size == 64 && a->prev == heap.blocks[2],
               "a free predecessor at a higher address is not merged");
    }
    // Nothing free around: a plain free.
    {
        const int sizes[] = {64, 64, 64};
        const unsigned short states[] = {1, 1, 1};
        heap.build(3, sizes, states);
        norrath_heap_free_bytes_view = 5;
        memFree__FPv(payload_of(heap.blocks[1]));
        expect(heap.blocks[1]->state == 0 && heap.blocks[1]->size == 64 && heap.ring_length(heap.blocks[0]) == 3 &&
                   norrath_heap_free_bytes_view == 5 + 64 + h,
               "isolated free changes only the freed header");
    }
    if (failures) return 1;
    std::printf("memFree contract passed\n");
    return 0;
}
