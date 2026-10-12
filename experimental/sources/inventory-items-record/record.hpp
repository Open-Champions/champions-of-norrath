#pragma once
// Layout of the player record lists read by the inventory routines.
// Observed: player records are 23984 bytes apart in `player`. Three lists start
// at record+328, each 4616 bytes: 64 slots of 72 bytes, a signed count at
// list+4608, and 4 bytes that no routine here reads. Slot+0 is a pointer to a
// description whose first word points to the item name; slot+4 is a signed
// halfword. Unknown: the other slot fields and what distinguishes the lists.
// The record words are 32 bits. A host check cannot place objects below 4 GiB,
// so it defines NORRATH_INVENTORY_HOST and supplies host_resolve to turn a
// word back into a host pointer.
extern "C" unsigned char player[];

namespace inventory_items {
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

inline List* player_lists(int owner) {
    return reinterpret_cast<List*>(player + owner * 23984 + 328);
}

#if defined(NORRATH_INVENTORY_HOST)
const void* host_resolve(unsigned word);
template <class T> const T* resolve(unsigned word) { return static_cast<const T*>(host_resolve(word)); }
#else
template <class T> const T* resolve(unsigned word) { return reinterpret_cast<const T*>(word); }
#endif
inline const char* item_name(const Slot& slot) {
    return resolve<char>(resolve<Description>(slot.description)->name);
}
}
