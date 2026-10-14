#include "setup_time_of_day.cpp"

static int trace[16];
static int count;
static void* args[16][3];

extern "C" int SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend(VIScene* s, VICamera* c,
                                                                              VITerrainProfileBlend* b) {
    args[count][0] = s; args[count][1] = c; args[count][2] = b;
    trace[count++] = 1;
    return -5;
}
extern "C" int SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(VIScene* s, VITerrainProfileBlend* b,
                                                                    unsigned flags) {
    args[count][0] = s; args[count][1] = b; args[count][2] = reinterpret_cast<void*>(static_cast<unsigned long>(flags));
    trace[count++] = 2;
    return -6;
}
extern "C" int SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(VIScene* s, VITerrainProfileBlend* b) {
    args[count][0] = s; args[count][1] = b;
    trace[count++] = 3;
    return -7;
}

int main() {
    VIScene* s = reinterpret_cast<VIScene*>(0x1000);
    VICamera* c = reinterpret_cast<VICamera*>(0x2000);
    VITerrainProfileBlend* b = reinterpret_cast<VITerrainProfileBlend*>(0x3000);
    if (SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi(s, c, b, 0xfeedu) != 0) return 1;
    if (count != 3 || trace[0] != 1 || trace[1] != 2 || trace[2] != 3) return 2;
    if (args[0][0] != s || args[0][1] != c || args[0][2] != b) return 3;
    if (args[1][0] != s || args[1][1] != b || args[1][2] != reinterpret_cast<void*>(0xfeedul)) return 4;
    if (args[2][0] != s || args[2][1] != b) return 5;
    return 0;
}
