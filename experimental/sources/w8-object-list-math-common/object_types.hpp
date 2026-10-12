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
