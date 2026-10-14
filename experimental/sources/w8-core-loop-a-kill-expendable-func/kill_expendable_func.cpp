// Observed killExpendableFunc__FP10GameObject at 0x0017fe48, 84 bytes (file-local in the original). It reads the
// word at object byte 12. Unless (word & 0x00101000) == 0x00001000 it returns. When a non-null object passes,
// it calls the virtual slot at vtable byte 8: the vtable pointer is the word at object +72, the this-adjustment
// is the halfword at vtable +8, the function pointer is the word at vtable +12, and the arguments are
// (object + adjustment, 3). cleanUp__Fv passes it to objectIterateGrid.
// Inferred: slot 3 is the call the other reap-style natives make with argument 3 (see natives-c reap-enemies);
//   no meaning for the argument or the flag bits is claimed.
// Unknown: what the flag bits 0x1000 and 0x100000 stand for, and the slot's name.
namespace {
unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

short load_half(const unsigned char* bytes) {
    return static_cast<short>(static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8));
}

unsigned char* load_pointer(const unsigned char* bytes) {
    unsigned char* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

using Slot = void (*)(void* self, int argument);

Slot load_slot(const unsigned char* bytes) {
    Slot value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}
}

extern "C" void killExpendableFunc__FP10GameObject(unsigned char* object) {
    if ((load_word(object + 12) & 0x00101000u) != 0x00001000u) return;
    if (object == nullptr) return;
    const unsigned char* vtable = load_pointer(object + 72);
    const short adjust = load_half(vtable + 8);
    load_slot(vtable + 12)(object + adjust, 3);
}
