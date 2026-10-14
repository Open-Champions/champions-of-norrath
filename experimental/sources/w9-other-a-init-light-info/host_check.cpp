#include "init_light_info.cpp"
#include <cstdio>
#include <cstring>

float lightInfo[96];

int main() {
    for (int i = 0; i < 96; ++i) lightInfo[i] = 7.5f;
    initLightInfo__Fv();
    int bad = 0;
    for (int i = 80; i < 96; ++i) bad += lightInfo[i] != 7.5f;  // untouched tail
    bad += lightInfo[0] != 345.0f || lightInfo[8] != -508.0f || lightInfo[9] != 168.0f;
    bad += lightInfo[31] != 33.0f || lightInfo[72] != -27.0f || lightInfo[79] != 23.0f || lightInfo[35] != 66.0f;
    // running twice is idempotent
    float first[96];
    std::memcpy(first, lightInfo, sizeof first);
    initLightInfo__Fv();
    bad += std::memcmp(first, lightInfo, sizeof first) != 0;
    std::printf(bad ? "FAILED %d\n" : "ok\n", bad);
    return bad != 0;
}
