#include "update.cpp"
#include <string.h>

static int order[32]; static int order_n;
static float rot_vec[4][3]; static void* rot_mat[4]; static int rot_n;
static float sin_arg, cos_arg;
static float scale_arg; static void* scale_mat; static float local_probe[16];
static void* mul_args[3]; static void* weather_args[3]; static void* trans_obj; static float trans_a, trans_b;
static void* dtor_mat; static int dtor_flags;
static unsigned char* g_object;
static float seen_1192_at_rot;
extern "C" void RotateHPR__10VIMatrix44RC7VIVect3(void* m, const void* v) {
    rot_mat[rot_n] = m; memcpy(rot_vec[rot_n++], v, 12); order[order_n++] = 1;
    if (rot_n == 1) memcpy(&seen_1192_at_rot, g_object + 1192, 4);
}
extern "C" float sinf(float v) { sin_arg = v; order[order_n++] = 2; return 0.5f; }
extern "C" float cosf(float v) { cos_arg = v; order[order_n++] = 3; return 0.25f; }
extern "C" void Scale__10VIMatrix44f(void* m, float s) { scale_mat = m; scale_arg = s; memcpy(local_probe, m, 64); order[order_n++] = 4; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* r, const void* a, const void* b) { mul_args[0] = r; mul_args[1] = const_cast<void*>(a); mul_args[2] = const_cast<void*>(b); order[order_n++] = 5; }
extern "C" void ProcessWeather__12VIAtmosphereR7VISceneR8VICamera(void* o, void* s, void* c) { weather_args[0] = o; weather_args[1] = s; weather_args[2] = c; order[order_n++] = 6; }
extern "C" void ProcessTransitions__12VIAtmosphereR7VISceneff(void* o, void*, float a, float b) { trans_obj = o; trans_a = a; trans_b = b; order[order_n++] = 7; }
extern "C" void _$_10VIMatrix44(void* m, int f) { dtor_mat = m; dtor_flags = f; order[order_n++] = 8; }

int main() {
    static unsigned char object[1600];
    int scene, camera;
    g_object = object;
    memset(object, 0, sizeof object);
    int active = 1; memcpy(object + 1228, &active, 4);
    long long prev = 5000000000LL; memcpy(object + 1480, &prev, 8);
    float scale = 2.0f; memcpy(object + 1472, &scale, 4);
    long long now = prev + 1234;
    Update__12VIAtmosphereR7VISceneR8VICameralff(object, &scene, &camera, now, 2.0f, 7.0f);
    const float ka = __builtin_bit_cast(float, 0x3e860a8eu);
    const float kp = __builtin_bit_cast(float, 0x40490fdau);
    long long v1480, v1488; memcpy(&v1480, object + 1480, 8); memcpy(&v1488, object + 1488, 8);
    if (v1480 != now || v1488 != prev) return 1;
    float f1192; memcpy(&f1192, object + 1192, 4);
    if (f1192 != 1234.0f * __builtin_bit_cast(float, 0x3a83126eu)) return 2;
    if (seen_1192_at_rot != f1192) return 3;
    float angle = 2.0f * ka;
    if (rot_n != 2 || rot_mat[0] != object + 1280 || rot_mat[1] != object + 1408) return 4;
    if (rot_vec[0][0] != 0 || rot_vec[0][1] != 0 || rot_vec[0][2] != angle) return 5;
    if (rot_vec[1][0] != 0 || rot_vec[1][1] != 0 || rot_vec[1][2] != 2.0f * ka + kp) return 6;
    if (sin_arg != angle || cos_arg != angle) return 7;
    float f1328, f1332, f1456, f1460;
    memcpy(&f1328, object + 1328, 4); memcpy(&f1332, object + 1332, 4);
    memcpy(&f1456, object + 1456, 4); memcpy(&f1460, object + 1460, 4);
    if (f1328 != 17.5f || f1332 != -8.75f || f1456 != -17.5f || f1460 != 8.75f) return 8;
    for (int i = 0; i < 16; ++i) if (local_probe[i] != 0.0f) return 9;
    if (scale_arg != 2.0f) return 10;
    if (mul_args[0] != object + 1344 || mul_args[1] != scale_mat || mul_args[2] != object + 1280) return 11;
    if (weather_args[0] != object || weather_args[1] != &scene || weather_args[2] != &camera) return 12;
    if (trans_obj != object || trans_a != 2.0f || trans_b != 7.0f) return 13;
    if (dtor_mat != scale_mat || dtor_flags != 2) return 14;
    int expect[] = {1, 2, 3, 4, 5, 1, 6, 7, 8};
    if (order_n != 9) return 15;
    for (int i = 0; i < 9; ++i) if (order[i] != expect[i]) return 20 + i;

    // inactive: nothing is touched
    order_n = 0; rot_n = 0;
    memset(object, 0, sizeof object);
    Update__12VIAtmosphereR7VISceneR8VICameralff(object, &scene, &camera, 99, 1.0f, 1.0f);
    if (order_n != 0) return 40;
    for (unsigned i = 0; i < sizeof object; ++i) if (object[i]) return 41;
    // negative elapsed time wraps through the 32-bit truncation
    memset(object, 0, sizeof object);
    memcpy(object + 1228, &active, 4);
    prev = 0x100000010LL; memcpy(object + 1480, &prev, 8);
    Update__12VIAtmosphereR7VISceneR8VICameralff(object, &scene, &camera, 0x100000000LL, 0.0f, 0.0f);
    memcpy(&f1192, object + 1192, 4);
    if (f1192 != -16.0f * __builtin_bit_cast(float, 0x3a83126eu)) return 42;
    return 0;
}
