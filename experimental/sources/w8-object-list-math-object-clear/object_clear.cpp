// Observed objectClear__Fv at 0x140b68, extent 680. Deletes every object. In order:
//   1. lastNmBlocksFree = 100000, objectKillingEveryone = 1, slowListCycle = 0,
//      mediumListCycle = 0.
//   2. Each list is walked with a ListSafeIterator and every non-null payload
//      object is deleted through its virtual table: the entry at table + 8 holds
//      a halfword delta and, four bytes on, a function; the function is called
//      with (object + delta, 3). The lists, in order: runList (mask
//      objectIgnoreFlags), the 120 x 120 objectGrid heads row by row (mask 0), the
//      120 x 120 ghostObjectGrid heads (mask 0), then offTheGrid (mask 0). Each
//      iterator is destroyed with flags 2. The walk reads the current node from the
//      iterator and advances it after the call, so deleting the current object
//      (which unlinks its nodes) is safe.
//   3. objectStrings = 0, and the 512 words of stringHash and objectHash are
//      zeroed.
//   4. objectKillingEveryone = 0.
// Inferred: the call with argument 3 is the deleting form of the virtual
// destructor. Unknown: what the delta and the halfword after it are.
#include "../w8-object-list-math-common/safe_iterator.hpp"

struct VirtualEntry {
    short delta;
    short reserved;
    void (*function)(void* self, int flags);
};
#if defined(__mips__)
static_assert(sizeof(VirtualEntry) == 8, "virtual table entry size");
#endif

struct ObjectForClear {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
    unsigned char gap[72 - 16];
    const VirtualEntry* vtable;
};

struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};

extern "C" {
void __16ListSafeIteratorP8ListHeadi(ListSafeIterator*, ListHead*, int);
void next__16ListSafeIterator(ListSafeIterator*);
void _$_16ListSafeIterator(ListSafeIterator*, int);
extern int objectIgnoreFlags;
extern int lastNmBlocksFree;
extern int objectKillingEveryone;
extern int slowListCycle;
extern int mediumListCycle;
extern HeadSlot runList;
extern ListHead objectGrid[120][120];
extern ListHead ghostObjectGrid[120][120];
extern ListHead offTheGrid;
extern int objectStrings;
extern unsigned stringHash[512];
extern unsigned objectHash[512];
}

namespace {
void delete_all(ListHead* head, int mask) {
    ListSafeIterator iterator;
    __16ListSafeIteratorP8ListHeadi(&iterator, head, mask);
    for (ListNode* node = iterator.link.prev; node != nullptr; node = iterator.link.prev) {
        ObjectForClear* object = static_cast<ObjectForClear*>(node->value);
        if (object != nullptr) {
            const VirtualEntry& entry = object->vtable[1];
            entry.function(reinterpret_cast<char*>(object) + entry.delta, 3);
        }
        next__16ListSafeIterator(&iterator);
    }
    _$_16ListSafeIterator(&iterator, 2);
}
}  // namespace

extern "C" void objectClear__Fv() {
    lastNmBlocksFree = 100000;
    objectKillingEveryone = 1;
    slowListCycle = 0;
    mediumListCycle = 0;
    delete_all(&runList.head, objectIgnoreFlags);
    for (int row = 0; row < 120; ++row)
        for (int column = 0; column < 120; ++column) delete_all(&objectGrid[row][column], 0);
    for (int row = 0; row < 120; ++row)
        for (int column = 0; column < 120; ++column) delete_all(&ghostObjectGrid[row][column], 0);
    delete_all(&offTheGrid, 0);
    objectStrings = 0;
    for (int i = 0; i < 512; ++i) {
        stringHash[i] = 0;
        objectHash[i] = 0;
    }
    objectKillingEveryone = 0;
}
