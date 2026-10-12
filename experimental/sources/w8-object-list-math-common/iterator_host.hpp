#pragma once
// Host-only helpers for the safe iterator checks. Not an EE plan input.
// Build: c++ -std=c++17 -Iw8-object-list-math-common safe_iterator_construct.cpp safe_iterator_next.cpp
//        safe_iterator_destroy.cpp <unit>/host_check.cpp (the three sources are linked together).
#include <cstdio>
#include <cstring>
#include "safe_iterator.hpp"

extern "C" void __16ListSafeIteratorP8ListHeadi(ListSafeIterator*, ListHead*, int);
extern "C" void next__16ListSafeIterator(ListSafeIterator*);
extern "C" void _$_16ListSafeIterator(ListSafeIterator*, int);

namespace host {
inline int failures = 0;
inline int deallocations = 0;
inline void* deallocated = nullptr;
inline void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

// head -> nodes[0] -> ... -> nodes[count-1]; payload i is objects[i] or null.
struct Fixture {
    ListHead head;
    ListNode nodes[6];
    GameObjectLinks objects[6];
    int count;
    void build(int n, const unsigned* flags, const bool* has_payload) {
        std::memset(this, 0, sizeof *this);
        count = n;
        for (int i = 0; i < n; ++i) {
            objects[i].flags = flags[i];
            nodes[i].value = has_payload[i] ? &objects[i] : nullptr;
            nodes[i].next = i + 1 < n ? &nodes[i + 1] : nullptr;
            nodes[i].prev = i ? &nodes[i - 1] : reinterpret_cast<ListNode*>(&head);
        }
        head.first = n ? &nodes[0] : nullptr;
    }
    // Chain order with iterator markers included, as node pointers.
    int walk(ListNode** out) const {
        int n = 0;
        ListNode* previous = reinterpret_cast<ListNode*>(const_cast<ListHead*>(&head));
        for (ListNode* node = head.first; node; node = node->next) {
            if (node->prev != previous) return -1;
            out[n++] = node;
            previous = node;
        }
        return n;
    }
};
}  // namespace host

// Stand-in for the provider of the tail-called delete in the destructor.
extern "C" void norrath_animation_state_deallocate(void* allocation) {
    ++host::deallocations;
    host::deallocated = allocation;
}
