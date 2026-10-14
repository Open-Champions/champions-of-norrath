// Observed: Update__12VIAtmosphereR7VISceneR8VICameralff at 0x01067d28 is 524
// bytes. Arguments: object, scene, camera, a 64-bit timestamp (a3), and two
// floats f12 and f13. Nothing happens unless the word at offset 1228 is nonzero.
// Then, with prev the 64-bit value at 1480: the 64-bit value at 1488 gets prev
// and the one at 1480 gets the timestamp; the float at 1192 gets (float)(int)(timestamp - prev)
// times the float bits 0x3a83126e. An angle a = f12 * (bits 0x3e860a8e) is made.
// A local vector (0, 0, a) is passed to RotateHPR__10VIMatrix44RC7VIVect3 on
// this+1280. The floats at 1328 and 1332 get sinf(a) * 35.0f and cosf(a) * -35.0f.
// A local zeroed matrix is scaled by the float at 1472 with Scale__10VIMatrix44f
// and Mul__10VIMatrix44RC10VIMatrix44T1(this+1344, local, this+1280) is called.
// A second vector (0, 0, f12 * 0x3e860a8e + bits 0x40490fda) goes to
// RotateHPR on this+1408. The floats at 1456 and 1460 get the negatives of those
// at 1328 and 1332. Then ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(this,
// scene, camera) and ProcessTransitions__12VIAtmosphereR7VISceneff(this, scene,
// f12, f13) are called, and the local matrix is destroyed with
// _$_10VIMatrix44 flags 2. No value is returned.
// Inferred: bits 0x3a83126e is 0.001, 0x3e860a8e about pi/12, 0x40490fda is pi, so
// the timestamp is in milliseconds, f12 is a time/phase value, and the object
// at 1280 is a rotation matrix. Unknown: the roles of 1344, 1408, 1472.
extern "C" {
void RotateHPR__10VIMatrix44RC7VIVect3(void* matrix, const void* angles);
float sinf(float value);
float cosf(float value);
void Scale__10VIMatrix44f(void* matrix, float scale);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(void* object, void* scene, void* camera);
void ProcessTransitions__12VIAtmosphereR7VISceneff(void* object, void* scene, float a, float b);
void _$_10VIMatrix44(void* matrix, int flags);

void Update__12VIAtmosphereR7VISceneR8VICameralff(void* object, void* scene, void* camera, long long time, float a, float b) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (*reinterpret_cast<int*>(bytes + 1228) == 0) return;
    const float k_millis = __builtin_bit_cast(float, 0x3a83126eu);
    const float k_angle = __builtin_bit_cast(float, 0x3e860a8eu);
    const float k_pi = __builtin_bit_cast(float, 0x40490fdau);
    long long previous = *reinterpret_cast<long long*>(bytes + 1480);
    float elapsed = static_cast<float>(static_cast<int>(time - previous));
    *reinterpret_cast<long long*>(bytes + 1488) = previous;
    float angle = a * k_angle;
    float rotate[3] = {0.0f, 0.0f, angle};
    *reinterpret_cast<long long*>(bytes + 1480) = time;
    *reinterpret_cast<float*>(bytes + 1192) = elapsed * k_millis;
    RotateHPR__10VIMatrix44RC7VIVect3(bytes + 1280, rotate);
    float* sine = reinterpret_cast<float*>(bytes + 1328);
    float* cosine = reinterpret_cast<float*>(bytes + 1332);
    *sine = sinf(angle) * 35.0f;
    *cosine = cosf(angle) * -35.0f;
    alignas(16) float local[16];
    for (int i = 0; i < 16; ++i) local[i] = 0.0f;
    Scale__10VIMatrix44f(local, *reinterpret_cast<float*>(bytes + 1472));
    Mul__10VIMatrix44RC10VIMatrix44T1(bytes + 1344, local, bytes + 1280);
    float rotate2[3] = {0.0f, 0.0f, a * k_angle + k_pi};
    RotateHPR__10VIMatrix44RC7VIVect3(bytes + 1408, rotate2);
    *reinterpret_cast<float*>(bytes + 1456) = -*sine;
    *reinterpret_cast<float*>(bytes + 1460) = -*cosine;
    ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(object, scene, camera);
    ProcessTransitions__12VIAtmosphereR7VISceneff(object, scene, a, b);
    _$_10VIMatrix44(local, 2);
}
}
