#pragma once
#include "list_types.hpp"
// Observed GameObject word offsets used by the object list routines. Names are
// inferred from use, not from original debug names; the rest of the object is
// not described here.
//   +4   the run-list node (zero when the object is on no run list)
//   +8   the grid node; its partner node is the off-grid twin
//   +12  the flag word the list iterator tests against its mask
struct GameObjectLinks {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(GameObjectLinks, run_node) == 4 && __builtin_offsetof(GameObjectLinks, grid_node) == 8 &&
                  __builtin_offsetof(GameObjectLinks, flags) == 12, "object offsets");
#endif

// Signature observed in the mangled names of the importers.
struct Point3 {
    float x, y, z;
};

// Observed: the pointer to the object's virtual table is the word at +72, and
// the routines that ask for an object's class call the function at word 1 of that
// table (byte +4) with no argument; it returns a pointer to a type record whose
// first word is the class name pointer. Everything between the link words and
// +72 is not described here.
struct TypeRecord {
    const char* name;
};
struct GameObjectVirtual {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
    unsigned char gap[72 - 16];
    const void* const* vtable;
};
using TypeRecordGetter = const TypeRecord* (*)();
#if defined(__mips__)
static_assert(sizeof(GameObjectLinks) == 16 && __builtin_offsetof(GameObjectVirtual, vtable) == 72, "vtable offset");
#endif
