// Observed: SetEffectMix__12VIAtmospheref at 0x0106b5e8 is 8 bytes. It stores its float argument
// at byte offset 128 of the atmosphere object and returns nothing. Unknown: the meaning of the
// value beyond the method name.
extern "C" {
void SetEffectMix__12VIAtmospheref(unsigned char* self, float mix) {
    *reinterpret_cast<float*>(self + 128) = mix;
}
}
