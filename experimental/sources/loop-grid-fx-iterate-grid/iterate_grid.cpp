// Observed objectIterateGrid__FPFP10GameObject_v at 0x13ec40, extent 424.
// Visits every object in every cell list. Order: objectGrid (0x4fc0c0) rows
// 0 to 119, each with cells 0 to 119 (row stride 480 bytes), then the same for
// ghostObjectGrid (0x50a1c0), then the single offTheGrid head (0x486890). Each
// cell gets a ListSafeIterator over the head with the word at 0x483ac0 as its
// mask (reread for every cell), the callback receives the value word at
// node+12 for every node, the iterator advances after the call, and the
// iterator is destroyed with flag 2 after the cell. The iterator object is 32
// bytes with the current node at +4; the original keeps three, one per phase.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char offTheGrid[];
extern unsigned char norrath_grid_ignore_flags_view[];
void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask);
void norrath_list_safe_iterator_next(unsigned char* iterator);
void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
using Callback = void (*)(unsigned char*);
unsigned char* bytes(Address a) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(static_cast<unsigned>(a)));
#else
    return reinterpret_cast<unsigned char*>(a);
#endif
}
unsigned read_direct(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
unsigned read_word(Address a) { return read_direct(bytes(a)); }
int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}

void visit_list(unsigned char* iterator, unsigned char* head, Callback callback) {
    norrath_list_safe_iterator_construct(iterator, head, signed_word(read_direct(norrath_grid_ignore_flags_view)));
    unsigned current = read_direct(iterator + 4u);
    while (current != 0u) {
        callback(reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(read_word(current + 12u))));
        norrath_list_safe_iterator_next(iterator);
        current = read_direct(iterator + 4u);
    }
    norrath_list_safe_iterator_destroy(iterator, 2);
}

void visit_grid(unsigned char* iterator, unsigned char* grid, Callback callback) {
    for (int row = 0; row < 120; ++row)
        for (int column = 0; column < 120; ++column)
            visit_list(iterator, grid + (row * 120 + column) * 4, callback);
}
}

extern "C" void objectIterateGrid__FPFP10GameObject_v(void (*callback)(void*)) {
    alignas(8) unsigned char iterator[32];
    const Callback call = reinterpret_cast<Callback>(callback);
    visit_grid(iterator, objectGrid, call);
    visit_grid(iterator, ghostObjectGrid, call);
    visit_list(iterator, offTheGrid, call);
}
