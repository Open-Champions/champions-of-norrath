// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "set_effect_mix.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char obj[256], before[256];
    std::memset(obj, 0x33, sizeof obj);
    std::memcpy(before, obj, sizeof obj);
    SetEffectMix__12VIAtmospheref(obj, 1.5f);
    float got;
    std::memcpy(&got, obj + 128, 4);
    if (got != 1.5f) return 1;
    before[128] = obj[128]; before[129] = obj[129]; before[130] = obj[130]; before[131] = obj[131];
    if (std::memcmp(obj, before, sizeof obj) != 0) return 2;
    SetEffectMix__12VIAtmospheref(obj, -0.25f);
    std::memcpy(&got, obj + 128, 4);
    return got == -0.25f ? 0 : 3;
}
