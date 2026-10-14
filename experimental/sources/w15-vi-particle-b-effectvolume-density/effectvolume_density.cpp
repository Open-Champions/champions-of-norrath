// Observed: Density__C14VIEffectVolume at 0x0114a2a0, 8 bytes. It returns the float word at byte 44 of its argument and writes nothing.
// Inferred: a float field of the volume. Unknown: the member name.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
float Density__C14VIEffectVolume(const void* object) {
    return *reinterpret_cast<const float*>(static_cast<const unsigned char*>(object) + 44);
}
}
