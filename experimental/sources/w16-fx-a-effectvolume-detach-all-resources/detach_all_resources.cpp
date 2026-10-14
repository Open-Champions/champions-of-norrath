// Observed: DetachAllResources__14VIEffectVolume at 0x0114a328, 52 bytes. It calls DetachSurface__14VIEffectVolumei(this, 0) and then
// DetachSurface__14VIEffectVolumei(this, 1). Nothing is stored here and nothing is returned.
// Inferred: an effect volume has two surface slots and this releases both, slot 0 first.
// Unknown: what a slot holds.
extern "C" {
void DetachSurface__14VIEffectVolumei(void* self, int slot);

void DetachAllResources__14VIEffectVolume(void* self) {
    DetachSurface__14VIEffectVolumei(self, 0);
    DetachSurface__14VIEffectVolumei(self, 1);
}
}
