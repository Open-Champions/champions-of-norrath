#pragma once
// Host-only helpers for the heap checks. Not an EE plan input. The header size on
// the host differs from the EE's 28 bytes; every routine uses kHeaderBytes.
#include <cstdio>
#include <cstring>
#include "heap_block.hpp"

namespace host {
inline int failures = 0;
inline void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

// Lays out blocks back to back in an arena as a circular list in address order.
// sizes[i] is the payload size and states[i] the state of block i.
struct Heap {
    alignas(256) unsigned char arena[1 << 16];
    HeapBlock* blocks[16];
    int count = 0;
    void build(int n, const int* sizes, const unsigned short* states, int first_offset = 0) {
        std::memset(arena, 0x5a, sizeof arena);
        count = n;
        unsigned char* at = arena + first_offset;
        for (int i = 0; i < n; ++i) {
            HeapBlock* b = reinterpret_cast<HeapBlock*>(at);
            b->magic = kHeaderMagic;
            b->state = states[i];
            b->size = sizes[i];
            b->name = states[i] == 1 ? "owner" : nullptr;
            b->tag = 0x77;
            b->caller = 0x88;
            blocks[i] = b;
            at += kHeaderBytes + sizes[i];
        }
        for (int i = 0; i < n; ++i) {
            blocks[i]->next = blocks[(i + 1) % n];
            blocks[i]->prev = blocks[(i + n - 1) % n];
        }
    }
    // Walks from blocks[0]'s successor chain; returns the number of headers seen
    // and checks that next/prev agree.
    int ring_length(HeapBlock* start) const {
        int n = 0;
        HeapBlock* b = start;
        do {
            if (b->next->prev != b) return -1;
            ++n;
            b = b->next;
        } while (b != start && n < 64);
        return n;
    }
};
}  // namespace host
