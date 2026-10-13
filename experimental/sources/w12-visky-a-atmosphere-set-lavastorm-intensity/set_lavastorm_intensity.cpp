// Observed: SetLavastormIntensity__12VIAtmospheref at 0x0106b680 is 112 bytes. With the
// atmosphere object as this: if the signed word at byte offset 4480 is -1, or else the signed word
// at 4484 is -1, nothing is stored and nothing is called. Otherwise the float at offset 76 becomes
// the argument; when the argument is greater than 0.0 and the old value compares equal to 0.0,
// ClearAllFireballs__12VIAtmosphere(this) runs before the store. Nothing is returned.
// Inferred: the two words are "not set up" markers (-1) for the lava storm, and the clear drops stale
// fireballs when the storm switches on. Unknown: what the two words hold when valid.
extern "C" {
void ClearAllFireballs__12VIAtmosphere(unsigned char* self);

void SetLavastormIntensity__12VIAtmospheref(unsigned char* self, float intensity) {
    if (*reinterpret_cast<const int*>(self + 4480) == -1) return;
    if (*reinterpret_cast<const int*>(self + 4484) == -1) return;
    float* slot = reinterpret_cast<float*>(self + 76);
    if (intensity > 0.0f && *slot == 0.0f) ClearAllFireballs__12VIAtmosphere(self);
    *slot = intensity;
}
}
