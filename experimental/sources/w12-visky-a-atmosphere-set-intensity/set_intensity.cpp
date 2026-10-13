// Observed: SetIntensity__12VIAtmospheref at 0x0106b5f8 is 84 bytes. The float at byte offset 72
// of the atmosphere object is replaced by the argument. When the argument is greater than 0.0 and
// the old value compares equal to 0.0, ClearAllSplashes__12VIAtmosphere(this) runs first. A
// negative old value or a non-positive argument skips that call. The store is
// unconditional. Nothing is returned.
// Inferred: the intensity going from off to on discards stale splash state (the callee is
// named for clearing splashes; its body is a separate package item). The R5900 has no NaN, so only
// ordinary finite values are claimed. Unknown: the unit of the value.
extern "C" {
void ClearAllSplashes__12VIAtmosphere(unsigned char* self);

void SetIntensity__12VIAtmospheref(unsigned char* self, float intensity) {
    float* slot = reinterpret_cast<float*>(self + 72);
    if (intensity > 0.0f && *slot == 0.0f) ClearAllSplashes__12VIAtmosphere(self);
    *slot = intensity;
}
}
