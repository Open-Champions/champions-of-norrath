// Observed: SpellEffect__19VISpellEffectSystemi at 0x01002d10 is 28 bytes in the pinned SLUS-20565 image. It returns the word at
// offset index * 12 of the block whose address is the word at +1120 of the system.
// Inferred: the pool of spell effects at +1112 keeps its element block at +1120 (hence the elements are 12 bytes apart) and this
// accessor returns the pointer held by one element. Unknown: element layout beyond the first word, and the index bounds, which
// are not checked. Nothing here ran on the EE.
extern "C" void* SpellEffect__19VISpellEffectSystemi(void* self, int index) {
    unsigned char* const block = *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(self) + 1120);
    return *reinterpret_cast<void**>(block + index * 12);
}
