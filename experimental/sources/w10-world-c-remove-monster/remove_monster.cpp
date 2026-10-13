// Observed removeMonster__FP10GameObject at 0x0018e388, 80 bytes (a local symbol in the
// image; the importers name it, so it is defined here with external linkage).
// Reads the flag word at object+12. Only when bit 0x20 is set, and the signed byte at
// object+570 is zero, it sets bit 4 in that byte, stores the flag word with 0x88000 added
// by OR (0x80000 | 0x8000), and then calls objectRemoveFromGrid(object). Anything else
// returns without a store. The flag word is read once, before the byte test.
// Inferred: the 0x20 test selects monsters, from the function name only; no flag or
// field names are claimed.
// Unknown: what bit 0x8000 and bit 0x80000 of the flag word, and bit 4 of the byte at
// +570, stand for.
struct MonsterObject {
    unsigned char before_flags[12];
    unsigned flags;
    unsigned char gap[570 - 16];
    signed char state_byte;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(MonsterObject, flags) == 12 && __builtin_offsetof(MonsterObject, state_byte) == 570,
              "observed offsets");
#endif

extern "C" void objectRemoveFromGrid__FP10GameObject(MonsterObject* object);

extern "C" void removeMonster__FP10GameObject(MonsterObject* object) {
    const unsigned flags = object->flags;
    if ((flags & 0x20u) != 0u && object->state_byte == 0) {
        object->state_byte = static_cast<signed char>(static_cast<unsigned char>(object->state_byte) | 4u);
        object->flags = flags | 0x88000u;
        objectRemoveFromGrid__FP10GameObject(object);
    }
}
