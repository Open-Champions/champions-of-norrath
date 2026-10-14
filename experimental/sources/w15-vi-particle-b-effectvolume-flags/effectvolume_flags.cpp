// Observed: Flags__C14VIEffectVolume at 0x0114a2b8, 8 bytes. It returns the signed int word at byte 60 of its argument and writes nothing.
// Inferred: a flags word of the volume. Unknown: the member name.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
int Flags__C14VIEffectVolume(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 60);
}
}
