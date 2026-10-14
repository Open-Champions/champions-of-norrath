// Observed checkForMonsters__FP10GameObject at 0x0013b760, 32 bytes with its
// padding. This is the grid callback of amxCheckIfAnyMonstersRemain. The word
// at object+12 is tested: if bit 6 is clear and the masked bits 0x28 equal
// exactly 0x20, noMonsters is cleared. Anything else leaves it unchanged.
extern "C" signed char noMonsters;

extern "C" void checkForMonsters__FP10GameObject(const unsigned char* object) {
    const unsigned flags = *reinterpret_cast<const unsigned*>(object + 12);
    if ((flags & 0x40u) == 0 && (flags & 0x28u) == 0x20u) noMonsters = 0;
}
