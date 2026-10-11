// Observed msg_collision__11SmallSpiderP10GameObject at 0x0020f918, 120 bytes.
// A non-null other object, a signed half at spider+572 other than 10, and
// bit 0x40 in the other's word at +12 select the virtual call. That call
// reads the spider vtable at +72, uses the slot 184 bytes in, adds the
// signed half at the slot to the spider, and passes 10. Creature collision
// then runs with the same two pointers, including when the slot was skipped.

#if defined(NORRATH_VSLOTS_A_HOST)
extern unsigned long long norrath_vslots_a_host_base;
#endif

extern "C" void msg_collision__8CreatureP10GameObject(void* self, void* other);

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

extern "C" void msg_collision__11SmallSpiderP10GameObject(void* self, void* other) {
    auto* spider = static_cast<unsigned char*>(self);
    if (other != nullptr && read_half(spider + 572) != 10) {
        const auto* target = static_cast<const unsigned char*>(other);
        if ((read_word(target + 12) & 0x40u) != 0u) {
            unsigned char* slot = from_word(read_word(spider + 72)) + 184;
            using SlotCall = void (*)(void*, int);
            auto* call = reinterpret_cast<SlotCall>(from_word(read_word(slot + 4)));
            call(spider + read_half(slot), 10);
        }
    }
    msg_collision__8CreatureP10GameObject(spider, other);
}
