// Observed energyCostRamp4__5Skillii at 0x00313788, 96 bytes (symbol size).
// The receiver holds the address of a table of 8-byte slots at +96. Each slot is a signed halfword
// (added to the receiver before the call) at +0 and a function word at +4. The routine makes two
// calls through that table:
//  1. slot at table+120 (slot 15, tweekRank__5Skillii in the class tables) with the two integer arguments
//     passed through unchanged; its result is "adjusted";
//  2. slot at table+88 (slot 11, evaluateRampAtRank__5Skillii) with the argument 4 and "adjusted".
// The integer result of the second call is converted to float and returned. Nothing is written.
// Inferred: the energy cost is the fifth ramp (index 4) evaluated at the adjusted rank.
// Unknown: the meaning of the first argument beyond being forwarded; nothing here was executed on a console.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_word_pointer(unsigned word);
extern "C" int norrath_host_call_slot(unsigned function, unsigned char* self, int first, int second);
static unsigned char* object_at(unsigned word) { return norrath_host_word_pointer(word); }
static int call_word(unsigned word, unsigned char* self, int first, int second) {
    return norrath_host_call_slot(word, self, first, second);
}
#else
static unsigned char* object_at(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
static int call_word(unsigned word, unsigned char* self, int first, int second) {
    using Function = int (*)(unsigned char*, int, int);
    return reinterpret_cast<Function>(static_cast<__UINTPTR_TYPE__>(word))(self, first, second);
}
#endif

struct Slot {
    short adjust;
    unsigned short pad;
    unsigned function;
};

__attribute__((always_inline)) static inline int call_slot(unsigned char* self, unsigned offset, int first, int second) {
    const Slot* slot = reinterpret_cast<const Slot*>(
        object_at(*reinterpret_cast<const unsigned*>(self + 96)) + offset);
    return call_word(slot->function, self + slot->adjust, first, second);
}

extern "C" float energy_cost_ramp4(void*, int, int) asm("energyCostRamp4__5Skillii");
extern "C" float energy_cost_ramp4(void* self, int first, int second) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const int adjusted = call_slot(bytes, 120, first, second);
    return static_cast<float>(call_slot(bytes, 88, 4, adjusted));
}
