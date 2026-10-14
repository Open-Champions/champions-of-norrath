// Observed: CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44 at 0x010644a8, 120 bytes. Arguments (this, matrix,
// scene, out). It calls GetTime__7VIScene(scene) and compares the full 64-bit result with the 64-bit word at this+192. When they differ
// it calls UpdateCenterTransform__9VIHSpriteR7VIScene(this, scene). It then calls Mul__10VIMatrix44RC10VIMatrix44T1(out, this+320, matrix)
// and returns zero. It never writes this+192 itself.
// Inferred: the 4x4 matrix at this+320 is a transform about the sprite's centre that UpdateCenterTransform refreshes once per scene
// time; this routine multiplies it by the supplied matrix into the output. The argument order of Mul is as read from the registers.
// Unknown: the writer of this+192 (presumably UpdateCenterTransform) and the product's operand order convention.
extern "C" {
long long GetTime__7VIScene(void* scene);
void UpdateCenterTransform__9VIHSpriteR7VIScene(void* object, void* scene);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* out, const void* a, const void* b);

int CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(void* object, const void* matrix, void* scene, void* out) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (GetTime__7VIScene(scene) != *reinterpret_cast<long long*>(bytes + 192)) UpdateCenterTransform__9VIHSpriteR7VIScene(object, scene);
    Mul__10VIMatrix44RC10VIMatrix44T1(out, bytes + 320, matrix);
    return 0;
}
}
