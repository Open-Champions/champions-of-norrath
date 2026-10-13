// Observed evaluateRampAtRank__5SkilliiP10GameObject at 0x00313688, 112 bytes (extent to the next symbol 120).
// Arguments: the receiver, a slot index, a level and a game-object pointer that may be null. The receiver holds
// the address of a table of 8-byte slots at +96 (a signed halfword adjustment at +0 added to the receiver,
// a function word at +4). The word at +12 of the game object is tested against the bit 0x40:
//  - object non-null and bit set: call the slot at table+88 (evaluateRampAtRank__5Skillii in the class tables)
//    with the slot index and level, and return its result;
//  - object null or bit clear: call the slot at table+96 (evaluateCreatureRampAtRank__5Skillii) instead.
// Nothing is written. Inferred: bit 0x40 selects between the two ramp sets; what the bit means in the object
// is not established. Unknown: the object's other fields.
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

extern "C" int evaluate_ramp_at_rank_object(void*, int, int, void*) asm("evaluateRampAtRank__5SkilliiP10GameObject");
extern "C" int evaluate_ramp_at_rank_object(void* self, int slot, int level, void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (object != nullptr && (read_word(object, 12) & 0x40u) != 0u)
        return call_slot(bytes, 88, slot, level);
    return call_slot(bytes, 96, slot, level);
}
