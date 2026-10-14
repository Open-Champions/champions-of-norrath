// Observed: SetType__14VIEffectVolume18VIEffectVolumeTypef at 0x0114a240 is 96 bytes. Arguments: the object, a type value and a
// float. It calls DetachAllResources__14VIEffectVolume(this) and then FreeParticles__14VIEffectVolume(this). The type value is stored
// in the word at +36 unconditionally (the store sits in a branch delay slot). The float is stored at +44 only when 0.0f <= value, so
// a negative or NaN value leaves +44 unchanged. Finally the word 1 is stored at +72. Nothing is returned.
// Inferred: +36 is the volume type and +44 a type-specific scalar; +72 is a changed flag (the constructor and OnResize set it as well).
// Unknown: the meaning of the individual type values; what the float means for each type.
extern "C" {
void DetachAllResources__14VIEffectVolume(void* self);
void FreeParticles__14VIEffectVolume(void* self);

void SetType__14VIEffectVolume18VIEffectVolumeTypef(unsigned char* self, unsigned type, float value) {
    DetachAllResources__14VIEffectVolume(self);
    FreeParticles__14VIEffectVolume(self);
    *reinterpret_cast<unsigned*>(self + 36) = type;
    if (0.0f <= value) *reinterpret_cast<float*>(self + 44) = value;
    *reinterpret_cast<unsigned*>(self + 72) = 1;
}
}
