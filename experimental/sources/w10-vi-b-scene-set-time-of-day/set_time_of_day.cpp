// Observed SetTimeOfDay__7VIScenef at 0x01145388, 36 bytes. The argument is
// raised to 0.0 if lower, then lowered to the single-precision constant
// 0x41bffffa (23.999989) if higher, and stored as a float at offset 8160 of
// the scene. It returns zero. The unit of the value is not established here.
extern "C" {
struct VIScene;
int SetTimeOfDay__7VIScenef(VIScene* scene, float time) {
    if (time < 0.0f) time = 0.0f;
    if (time > 23.999989f) time = 23.999989f;
    *reinterpret_cast<float*>(reinterpret_cast<char*>(scene) + 8160) = time;
    return 0;
}
}
