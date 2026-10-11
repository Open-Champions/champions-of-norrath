// Observed objectAddToMediumRunList__FP10GameObject at 0x143908, extent 100.
// Removes the node word at object+4 when it is nonzero, then adds one to
// mediumListCycle (0x483ad4, four file bytes, initial zero). The signed sum
// becomes zero when it is at least 15. listOnewayLink receives
// mediumRunList + index*4, object+4, and the object. The cycle store is in
// that call's delay slot, so the new index is visible to the callee.
// mediumRunList at 0x4fbf90 has ELF size 0. slowRunList starts 64 bytes later.
// This routine indexes 15 words. The four bytes before slowRunList are not
// indexed. listRemove and listOnewayLink stay imports.
extern "C" unsigned norrath_list_remove(unsigned char* node);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);

namespace {
using Address = __UINTPTR_TYPE__;
Address address(const void* p) { return reinterpret_cast<Address>(p); }
unsigned char* bytes(Address a) { return reinterpret_cast<unsigned char*>(a); }
void* pointer(Address a) { return reinterpret_cast<void*>(a); }
unsigned read_word(Address a) {
    const unsigned char* p = bytes(a);
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(Address a, unsigned value) {
    unsigned char* p = bytes(a);
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int signed_word(unsigned value) {
    return value <= 0x7fffffffu ? static_cast<int>(value) : -1 - static_cast<int>(0xffffffffu - value);
}
}

// Fifteen list heads. The original signed compare has no lower clamp.
// mediumListCycle is four zero file bytes and was not an earlier view.
extern "C" {
unsigned char norrath_medium_run_list_heads[15 * 4];
unsigned norrath_medium_list_cycle_word;
}

extern "C" unsigned norrath_actor_add_medium_run_list(void* object) {
    const Address a = address(object);
    const unsigned existing = read_word(a + 4u);
    if (existing != 0u) (void)norrath_list_remove(bytes(existing));
    unsigned next = read_word(address(&norrath_medium_list_cycle_word)) + 1u;
    if (signed_word(next) >= 15) next = 0u;
    write_word(address(&norrath_medium_list_cycle_word), next);
    void* head = pointer(address(norrath_medium_run_list_heads) + (next << 2));
    return static_cast<unsigned>(address(norrath_list_oneway_link(head, pointer(a + 4u), object)));
}
