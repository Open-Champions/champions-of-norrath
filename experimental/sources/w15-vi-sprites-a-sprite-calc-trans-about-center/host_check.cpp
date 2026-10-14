// Host check only; not a build input.
#include "calc_trans_about_center.cpp"
#include <cstring>
static long long now; static char order[8]; static int n;
static void *updObj, *updScene, *mulOut; static const void *mulA, *mulB;
extern "C" long long GetTime__7VIScene(void*) { order[n++] = 'T'; return now; }
extern "C" void UpdateCenterTransform__9VIHSpriteR7VIScene(void* o, void* s) { order[n++] = 'U'; updObj = o; updScene = s; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* o, const void* a, const void* b) { order[n++] = 'M'; mulOut = o; mulA = a; mulB = b; }
int main() {
    alignas(8) unsigned char sprite[512] = {};
    char scene[4], matrix[4], out[4];
    long long stored = 7;
    std::memcpy(sprite + 192, &stored, 8);
    now = 7; n = 0; std::memset(order, 0, sizeof order);
    if (CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, matrix, scene, out) != 0 || std::strcmp(order, "TM")) return 1;
    if (mulOut != out || mulA != sprite + 320 || mulB != matrix) return 2;
    now = 8; n = 0; std::memset(order, 0, sizeof order);
    if (CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, matrix, scene, out) != 0 || std::strcmp(order, "TUM")) return 3;
    if (updObj != sprite || updScene != scene) return 4;
    now = 7 + (1ll << 32); n = 0; std::memset(order, 0, sizeof order);
    CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(sprite, matrix, scene, out);
    return std::strcmp(order, "TUM") ? 5 : 0;
}
