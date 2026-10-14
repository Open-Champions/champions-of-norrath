// Observed: Type__C14VIEffectVolume at 0x0114a238, 8 bytes. It returns the signed int word at byte 36 of its argument and writes nothing.
// Inferred: the volume type word. Unknown: the member name.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
int Type__C14VIEffectVolume(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 36);
}
}
