// Observed: Surface__C14VIEffectVolumei at 0x0114a318, 16 bytes. It returns the word at byte 64 + 4 * index of the object, for the signed index in the second argument; the index is not bounds-checked.
// Inferred: an array of per-surface words starting at byte 64; its length is unknown here.
// Nothing here ran on the EE; the paired comparison against the original used a private interpreter.
extern "C" {
int Surface__C14VIEffectVolumei(const void* object, int index) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 64 + 4 * index);
}
}
