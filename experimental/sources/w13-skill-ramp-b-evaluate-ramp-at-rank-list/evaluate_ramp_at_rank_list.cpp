// Observed evaluateRampAtRank__5SkilliiP8ListHead at 0x00313700, 112 bytes (extent to the next symbol 136).
// Arguments: the receiver, a slot index, a level and a list-head pointer. The receiver holds the address of a
// table of 8-byte slots at +96 (a signed halfword adjustment at +0 added to the receiver, a function word at +4).
// The first word of the list head is a node pointer, which may be null. When it is not null the word at +12 of
// the node is an object pointer (not tested for null) and the word at +12 of that object is tested against 0x40:
//  - bit set: call the slot at table+88 (evaluateRampAtRank__5Skillii) with the slot index and level and return its result;
//  - empty list or bit clear: call the slot at table+96 (evaluateCreatureRampAtRank__5Skillii) instead.
// Nothing is written. Inferred: the first element of the list picks the ramp set. Unknown: the node and object
// types beyond the two words read.
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

extern "C" int evaluate_ramp_at_rank_list(void*, int, int, void*) asm("evaluateRampAtRank__5SkilliiP8ListHead");
extern "C" int evaluate_ramp_at_rank_list(void* self, int slot, int level, void* list) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const unsigned node = read_word(list, 0);
    if (node != 0u) {
        const unsigned char* item = object_at(read_word(object_at(node), 12));
        if ((read_word(item, 12) & 0x40u) != 0u) return call_slot(bytes, 88, slot, level);
    }
    return call_slot(bytes, 96, slot, level);
}
