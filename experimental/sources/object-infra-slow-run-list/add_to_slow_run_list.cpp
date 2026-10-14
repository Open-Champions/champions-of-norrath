// Observed objectAddToSlowRunList__FP10GameObject at 0x1438a0, extent 100.
// The 60 heads are slowRunList at 0x4fbfd0, extent 240 through objectGrid.
// The cycle word is the existing actor-package view of slowListCycle at 0x483ad0.
// listRemove and listOnewayLink stay imports. No second copy of the cycle word.
extern "C" unsigned char norrath_actor_view_483ad0[];
extern "C" unsigned norrath_list_remove(unsigned char* node);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);

namespace {
// Pointer-width addresses. On the reviewed EE target this is 32 bits, so the
// head displacement wraps the same way as the original unsigned add.
using Address = __UINTPTR_TYPE__;
Address address(const void* p) {
    return reinterpret_cast<Address>(p);
}
unsigned char* bytes(Address a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
void* pointer(Address a) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(a));
}
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value)
                                : -1 - static_cast<int>(0xffffffffu - value);
}
}

// Sixty list heads. Index 0..59; the original signed compare has no lower clamp.
extern "C" {
unsigned char norrath_object_slow_run_list_view[60 * 4];
}

extern "C" void* norrath_object_add_to_slow_run_list(void* object) {
    const Address a = address(object);
    const unsigned existing = read_word(a + 4u);
    if (existing != 0u) (void)norrath_list_remove(bytes(existing));
    unsigned next = read_word(address(norrath_actor_view_483ad0)) + 1u;
    if (signed_word(next) >= 60) next = 0u;
    // The cycle store is in the original call's delay slot, so it is visible
    // to listOnewayLink. The returned word is that call's primary node.
    write_word(address(norrath_actor_view_483ad0), next);
    void* head = pointer(address(norrath_object_slow_run_list_view) + (next << 2));
    return norrath_list_oneway_link(head, pointer(a + 4u), object);
}
