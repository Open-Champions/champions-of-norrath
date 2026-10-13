// Observed SetZoomDistance__6Cameraf at 0x0026fb08, 112 bytes.
// With lo = float at camera+176, hi = float at camera+184, blend = the float
// at camera+144 limited to at most 1.0, and a = word at camera+84, b = word at
// camera+88 (used as indices): stores the argument at camera+140, stores
// 1.0 - (argument - lo) / (hi - lo) at camera+216, then stores
// f[b] + (f[a] - f[b]) * blend at camera+252, where f[i] is the float at
// camera+188+4*i. Nothing is returned. The meaning of the fields is not
// established here. Nothing here was executed on the EE.
typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void SetZoomDistance__6Cameraf(unsigned char* camera, float distance) {
    const float lo = *reinterpret_cast<AliasedFloat*>(camera + 176);
    const float hi = *reinterpret_cast<AliasedFloat*>(camera + 184);
    const float raw = *reinterpret_cast<AliasedFloat*>(camera + 144);
    const float blend = raw < 1.0f ? raw : 1.0f;
    const int a = *reinterpret_cast<AliasedInt*>(camera + 84);
    const int b = *reinterpret_cast<AliasedInt*>(camera + 88);
    *reinterpret_cast<AliasedFloat*>(camera + 140) = distance;
    *reinterpret_cast<AliasedFloat*>(camera + 216) = 1.0f - (distance - lo) / (hi - lo);
    const float fa = *reinterpret_cast<AliasedFloat*>(camera + 188 + 4 * a);
    const float fb = *reinterpret_cast<AliasedFloat*>(camera + 188 + 4 * b);
    *reinterpret_cast<AliasedFloat*>(camera + 252) = fb + (fa - fb) * blend;
}
