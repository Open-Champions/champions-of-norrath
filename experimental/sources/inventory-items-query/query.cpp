// Observed queryInventoryItem__FPci at 0x001b1b68, 184 bytes.
// Player records are 23984 bytes apart. Three lists start at record+328, each
// 4616 bytes: 64 slots of 72 bytes, then a signed count at list+4608. A count
// at or below zero skips the list. Slot+0 is a pointer to a description whose
// first word is the name passed to strcmp. A match adds the signed halfword at
// slot+4. The result is the sum across all three lists.
// Unknown: the slot fields past offset 6 and the meaning of the lists.
// The record words are 32 bits. A host check cannot place objects below 4 GiB,
// so it defines NORRATH_INVENTORY_HOST and supplies host_resolve to turn a
// word back into a host pointer.
extern "C" unsigned char player[];
extern "C" int strcmp(const char* left, const char* right);

namespace {
struct Slot {
    unsigned description;
    short quantity;
    unsigned char unread[66];
};
struct Description {
    unsigned name;
};
struct List {
    Slot slots[64];
    int count;
    unsigned char unread[4];
};
static_assert(sizeof(Slot) == 72 && sizeof(List) == 4616, "observed strides");

#if defined(NORRATH_INVENTORY_HOST)
const void* host_resolve(unsigned word);
template <class T> const T* resolve(unsigned word) { return static_cast<const T*>(host_resolve(word)); }
#else
template <class T> const T* resolve(unsigned word) { return reinterpret_cast<const T*>(word); }
#endif
}

extern "C" int queryInventoryItem__FPci(char* name, int owner) {
    int sum = 0;
    const List* list = reinterpret_cast<const List*>(player + owner * 23984 + 328);
    for (int pass = 0; pass < 3; ++pass, ++list) {
        for (int index = 0; index < list->count; ++index) {
            const Slot& slot = list->slots[index];
            if (strcmp(resolve<char>(resolve<Description>(slot.description)->name), name) == 0) sum += slot.quantity;
        }
    }
    return sum;
}
