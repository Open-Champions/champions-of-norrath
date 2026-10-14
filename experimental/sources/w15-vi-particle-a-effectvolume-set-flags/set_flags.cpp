// Observed: SetFlags__14VIEffectVolumeUi at 0x0114a2c0 is 8 bytes. It stores its unsigned argument in the word at offset 60 of
// the object and returns nothing.
// Inferred: the word is a flag mask (the constructor zeroes it).
// Unknown: the meaning of individual bits.
extern "C" {
void SetFlags__14VIEffectVolumeUi(unsigned char* self, unsigned flags) {
    *reinterpret_cast<unsigned*>(self + 60) = flags;
}
}
