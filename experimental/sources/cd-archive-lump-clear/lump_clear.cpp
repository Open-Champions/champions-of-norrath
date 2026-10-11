// Observed lumpClear__Fi at 0x001663f0, 188 bytes. The first byte of
// discardable at 0x006cc680 is cleared before soundClear__Fi. While nmLumps
// is positive and the argument is signed-greater-or-equal to the last
// record's lock at offset 32, lumpUnload__FPv receives that record's pointer
// at offset 16. resourceFrame at 0x00483f88, already defined as
// norrath_constructor_resource_frame_view, increments on every path.
extern "C" {
extern unsigned char discardable[];
extern unsigned char norrath_constructor_lump_cache_view[];
extern unsigned char norrath_constructor_lump_count_view[4];
extern unsigned char norrath_constructor_resource_frame_view[4];
void soundClear__Fi(int value);
void lumpUnload__FPv(void* lump);
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static int signed_word(unsigned value) {
    return static_cast<int>(value);
}
void lumpClear__Fi(int value) {
    discardable[0] = 0;
    soundClear__Fi(value);
    int count = signed_word(load_word(norrath_constructor_lump_count_view));
    if (count > 0) {
        unsigned char* const cache = norrath_constructor_lump_cache_view;
        int last_lock = signed_word(load_word(cache + static_cast<unsigned>(count) * 40u - 8u));
        if (!(value < last_lock)) {
            for (;;) {
                count = signed_word(load_word(norrath_constructor_lump_count_view));
                const unsigned end = static_cast<unsigned>(count) * 40u;
                const unsigned data = load_word(cache + end - 24u);
                lumpUnload__FPv(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(data)));
                count = signed_word(load_word(norrath_constructor_lump_count_view));
                if (count <= 0) break;
                last_lock = signed_word(load_word(cache + static_cast<unsigned>(count) * 40u - 8u));
                if (value < last_lock) break;
            }
        }
    }
    const unsigned frame = load_word(norrath_constructor_resource_frame_view);
    store_word(norrath_constructor_resource_frame_view, frame + 1u);
}
}
