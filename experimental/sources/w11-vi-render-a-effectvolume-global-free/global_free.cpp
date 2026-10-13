// Observed: GlobalFree__14VIEffectVolume at 0x0114a110 is 12 bytes. It stores
// zero in the 4-byte word at 0x011b1a98, which the image names
// _14VIEffectVolume$mScene, and returns nothing.
// Inferred: that word is the static scene pointer VIEffectVolume::GlobalInit sets.
// Unknown: the storage owner; the symbol stays an unresolved import here.
extern "C" {
extern void* _14VIEffectVolume$mScene;

void GlobalFree__14VIEffectVolume() {
    _14VIEffectVolume$mScene = 0;
}
}
