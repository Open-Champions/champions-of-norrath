// Observed msg_collision__3CatP10GameObject at 0x001eedf0, 120 bytes.
// A word of 1 at cat+964 returns immediately. Otherwise the signed half at
// cat+572 must be 0 or 13. readyToSwitch then has to return nonzero.
// switchToRun follows, then the virtual slot 184 bytes into the vtable at
// cat+72, with the signed half at that slot added to the cat and argument 3.
// The other object is not read.

#if defined(NORRATH_VSLOTS_A_HOST)
extern unsigned long long norrath_vslots_a_host_base;
#endif

extern "C" {
int readyToSwitch__3Cat(void* self);
void switchToRun__3Cat(void* self);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int read_half(const unsigned char* p) {
    unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 0x10000;
}
unsigned char* from_word(unsigned word) {
#if defined(NORRATH_VSLOTS_A_HOST)
    return reinterpret_cast<unsigned char*>(norrath_vslots_a_host_base + static_cast<long long>(static_cast<int>(word)));
#else
    return reinterpret_cast<unsigned char*>(static_cast<unsigned long long>(word));
#endif
}
}

extern "C" void msg_collision__3CatP10GameObject(void* self, void* other) {
    (void)other;
    auto* cat = static_cast<unsigned char*>(self);
    if (read_word(cat + 964) == 1u) return;
    const int state = read_half(cat + 572);
    if (state != 0 && state != 13) return;
    if (readyToSwitch__3Cat(cat) == 0) return;
    switchToRun__3Cat(cat);
    unsigned char* slot = from_word(read_word(cat + 72)) + 184;
    using SlotCall = void (*)(void*, int);
    auto* call = reinterpret_cast<SlotCall>(from_word(read_word(slot + 4)));
    call(cat + read_half(slot), 3);
}
