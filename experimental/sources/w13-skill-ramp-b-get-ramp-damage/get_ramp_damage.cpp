// Observed getRampDamage__5SkilliifiPiT5 at 0x00313290, 692 bytes (symbol size).
// Arguments after the receiver: an integer slot index, an integer level, a float spread fraction, an integer
// "randomize" flag, and two int output pointers (low and high). The receiver holds the address of a table of
// 8-byte slots at +96 (a signed halfword adjustment at +0, a function word at +4).
//  1. The slot at table+88 (evaluateRampAtRank__5Skillii) is called with the slot index and level; its integer
//     result v is converted to float.
//  2. low is written from  v - v * spread  and high from  v + v * spread, each rounded half away from zero
//     (+0.5 then truncation when the value is >= 0.0f, -0.5 otherwise), and a stored result of 0 or less is
//     replaced by 1. With the randomize flag zero the spread is used as given.
//  3. With the randomize flag non-zero the global word syncRandState steps once per draw
//     (state * 0x19660d + 0x3c6ef35f) and the draw  (state >> 16) * 2^-16  scales the spread:
//     the value becomes  v - v * draw * spread  (low) or  v + v * draw * spread  (high). The rounding
//     expression is evaluated twice, the sign test and the rounded branch each making their own draw, so low
//     takes two draws and high takes two draws: four steps of syncRandState in all, low first.
// low is written before the high expression is computed. Nothing else is written.
// Inferred: a damage range around the ramp value, narrowed by the fraction; the rounding expression is
// evaluated twice because the image shows two draws per output. Unknown: the member and argument names, and
// behaviour when low and high point at the same word or at syncRandState.
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

static unsigned read_word(const void* address, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(address) + offset);
}

struct Slot {
    short adjust;
    unsigned short pad;
    unsigned function;
};

__attribute__((always_inline)) static inline int call_slot(unsigned char* self, unsigned offset, int first, int second) {
    const Slot* slot = reinterpret_cast<const Slot*>(
        object_at(read_word(self, 96)) + offset);
    return call_word(slot->function, self + slot->adjust, first, second);
}

extern "C" unsigned syncRandState;

static float draw_unit() {
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    return static_cast<float>(static_cast<int>(syncRandState >> 16)) * 1.52587890625e-05f;
}

static int round_away(float value) {
    return value >= 0.0f ? static_cast<int>(value + 0.5f) : static_cast<int>(value - 0.5f);
}

extern "C" void get_ramp_damage(void*, int, int, float, int, int*, int*) asm("getRampDamage__5SkilliifiPiT5");
extern "C" void get_ramp_damage(void* self, int slot, int level, float spread, int randomize, int* low, int* high) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const float base = static_cast<float>(call_slot(bytes, 88, slot, level));
    if (randomize != 0) {
        *low = (base - base * draw_unit() * spread >= 0.0f) ? static_cast<int>(base - base * draw_unit() * spread + 0.5f)
                                                            : static_cast<int>(base - base * draw_unit() * spread - 0.5f);
    } else {
        *low = round_away(base - base * spread);
    }
    if (*low <= 0) *low = 1;
    if (randomize != 0) {
        *high = (base + base * draw_unit() * spread >= 0.0f) ? static_cast<int>(base + base * draw_unit() * spread + 0.5f)
                                                             : static_cast<int>(base + base * draw_unit() * spread - 0.5f);
    } else {
        *high = round_away(base + base * spread);
    }
    if (*high <= 0) *high = 1;
}
