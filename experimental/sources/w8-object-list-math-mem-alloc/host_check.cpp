// Host contract for memAlloc__FiPc. Not an EE plan input.
// Build: c++ -std=c++17 mem_alloc.cpp host_check.cpp
#include <cstdarg>
#include "../w8-object-list-math-common/heap_host.hpp"
using namespace host;

extern "C" void* memAlloc__FiPc(int, char*);

extern "C" HeapBlock* norrath_heap_ring_word_view = nullptr;
extern "C" HeapBlock* norrath_heap_next_fit_view = nullptr;
extern "C" int norrath_heap_fragment_threshold_view = 0;
extern "C" int norrath_heap_free_bytes_view = 0;
extern "C" unsigned norrath_heap_allocation_tag_view = 0;

static int check_calls, check_line, dump_calls, print_calls, crash_calls;
static const char* check_file;
static unsigned printed_ra;
static int printed_size;
extern "C" void memCheckHeapReal__FPCci(const char* file, int line) { ++check_calls; check_file = file; check_line = line; }
extern "C" void norrath_mem_dump_heap() { ++dump_calls; }
extern "C" int scePrintf(const char* format, ...) {
    ++print_calls;
    va_list args;
    va_start(args, format);
    printed_ra = va_arg(args, unsigned);
    printed_size = va_arg(args, int);
    va_end(args);
    return 0;
}
extern "C" void host_heap_failure_store() { ++crash_calls; }

static bool aligned256(const void* p) { return (reinterpret_cast<__UINTPTR_TYPE__>(p) & 255u) == 0; }

int main() {
    Heap heap;
    const int h = kHeaderBytes;
    char name[] = "test-owner";

    // Aligned block (payload already on a 256 boundary), exact fit, no split.
    {
        const int sizes[] = {512, 64};
        const unsigned short states[] = {0, 1};
        heap.build(2, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        norrath_heap_fragment_threshold_view = 0;
        norrath_heap_free_bytes_view = 10000;
        norrath_heap_allocation_tag_view = 0x1234;
        void* p = memAlloc__FiPc(512, name);
        expect(check_calls == 1 && std::strcmp(check_file, "mem.cc") == 0 && check_line == 502, "heap check first: mem.cc, 502");
        expect(p == payload_of(heap.blocks[0]) && aligned256(p), "aligned block is used in place");
        HeapBlock* b = heap.blocks[0];
        expect(b->state == 1 && b->name == name && b->tag == 0x1234 && b->caller != 0, "state, name, tag and caller recorded");
        expect(b->size == 512 && heap.ring_length(b) == 2, "exact fit is not split");
        expect(norrath_heap_free_bytes_view == 10000 - 512 - h, "free bytes drop by size + header");
        expect(norrath_heap_next_fit_view == b->next, "movingNode is the block's next link");
    }
    // Split tail with threshold: a remainder equal to rawMin is kept whole, one more splits.
    {
        for (int slack = 0; slack < 2; ++slack) {
            const int threshold = 40;
            const int sizes[] = {512 + threshold + slack, 64};
            const unsigned short states[] = {0, 1};
            heap.build(2, sizes, states, 256 - h);
            norrath_heap_ring_word_view = heap.blocks[0];
            norrath_heap_fragment_threshold_view = threshold;
            norrath_heap_free_bytes_view = 100000;
            // Note: with rawMin nonzero the misaligned test still uses payload alignment only.
            void* p = memAlloc__FiPc(512, name);
            HeapBlock* b = heap.blocks[0];
            if (slack == 0) {
                expect(b->size == 512 + threshold && heap.ring_length(b) == 2, "remainder equal to rawMin does not split (signed compare)");
                expect(norrath_heap_free_bytes_view == 100000 - (512 + threshold) - h, "charged the whole block");
            } else {
                HeapBlock* tail = b->next;
                expect(b->size == 512 && reinterpret_cast<char*>(tail) == reinterpret_cast<char*>(b) + 512 + h, "split at needed + header");
                expect(tail->state == 0 && tail->size == threshold + 1 - h && tail->magic == kHeaderMagic && tail->name == nullptr, "free tail header built");
                expect(tail->prev == b && tail->next == heap.blocks[1] && heap.blocks[1]->prev == tail, "tail linked in");
                expect(norrath_heap_free_bytes_view == 100000 - 512 - h, "charged the shrunk size");
            }
            (void)p;
        }
    }
    // Small and unaligned requests: clamp to 64, round to a multiple of 8.
    {
        const int sizes[] = {1024, 64};
        const unsigned short states[] = {0, 1};
        heap.build(2, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        norrath_heap_fragment_threshold_view = 8;
        norrath_heap_free_bytes_view = 0;
        memAlloc__FiPc(3, name);
        expect(heap.blocks[0]->size == 64, "tiny request becomes 64");
        heap.build(2, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        memAlloc__FiPc(100, name);
        expect(heap.blocks[0]->size == 104, "100 rounds up to 104");
        heap.build(2, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        memAlloc__FiPc(-50, name);
        expect(heap.blocks[0]->size == 64, "negative request becomes 64");
    }
    // Misaligned block: a free prefix block is split off and the payload lands on 256.
    {
        const int sizes[] = {2048, 64};
        const unsigned short states[] = {0, 1};
        heap.build(2, sizes, states, 0);
        HeapBlock* original = heap.blocks[0];
        norrath_heap_ring_word_view = original;
        norrath_heap_fragment_threshold_view = 0;
        norrath_heap_free_bytes_view = 0;
        void* p = memAlloc__FiPc(300, name);
        expect(aligned256(p), "returned payload is 256-aligned");
        HeapBlock* got = block_of(p);
        expect(got != original && got->state == 1 && got->name == name, "allocation lives in a new header");
        expect(original->state == 0 && original->next == got && got->prev == original, "the remainder in front stays a free block linked before it");
        expect(original->size == static_cast<int>(reinterpret_cast<char*>(got) - payload_of(original)), "prefix block size spans up to the new header");
        expect(got->size >= 300 && (got->size & 7) == 0, "size at least the request, multiple of 8");
        expect(heap.ring_length(original) >= 3, "ring has the prefix, the allocation, and the rest");
        // Total bytes in the ring are conserved.
        long total = 0;
        HeapBlock* b = original;
        do { total += h + b->size; b = b->next; } while (b != original);
        expect(total == (h + 2048) + (h + 64), "headers plus payloads are conserved");
    }
    // First fit from startNode: skip used and too-small blocks.
    {
        const int sizes[] = {64, 128, 1024, 512, 64};
        const unsigned short states[] = {1, 0, 1, 0, 1};
        heap.build(5, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        norrath_heap_fragment_threshold_view = 64;
        void* p = memAlloc__FiPc(200, name);
        expect(block_of(p)->name == name && block_of(p)->prev != nullptr, "an allocation was made");
        HeapBlock* got = block_of(p);
        HeapBlock* fourth = heap.blocks[3];
        bool in_fourth = got == fourth || got->prev == fourth || (got > fourth && got < heap.blocks[4]);
        expect(in_fourth && heap.blocks[1]->state == 0 && heap.blocks[1]->size == 128, "the 128-byte block was too small and was skipped");
        // Starting after the big block, the ring wraps to find space.
        heap.build(5, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[4];
        p = memAlloc__FiPc(100, name);
        expect(block_of(p) > heap.blocks[1] && block_of(p) < heap.blocks[4] && block_of(p)->name == name, "search wraps past the end of the ring and fits in the 512-byte block");
    }
    // Failure: a full circle without a fit.
    {
        const int sizes[] = {64, 64, 64};
        const unsigned short states[] = {1, 0, 1};
        heap.build(3, sizes, states, 256 - h);
        norrath_heap_ring_word_view = heap.blocks[0];
        norrath_heap_free_bytes_view = 77;
        void* p = memAlloc__FiPc(100, name);
        expect(p == nullptr && dump_calls == 1 && print_calls == 1 && crash_calls == 1, "dump, report, fault hook, null result");
        expect(printed_size == 100 && printed_ra != 0, "the report carries the return address and the clamped size");
        expect(norrath_heap_free_bytes_view == 77, "no accounting on failure");
        // With the free block's payload on a 256 boundary the same ring serves a 64-byte request.
        heap.build(3, sizes, states, 256 - 2 * h - 64);
        norrath_heap_ring_word_view = heap.blocks[0];
        p = memAlloc__FiPc(4, name);
        expect(p == payload_of(heap.blocks[1]) && heap.blocks[1]->size == 64, "an aligned 64-byte free block satisfies a tiny request");
    }
    if (failures) return 1;
    std::printf("memAlloc contract passed\n");
    return 0;
}
