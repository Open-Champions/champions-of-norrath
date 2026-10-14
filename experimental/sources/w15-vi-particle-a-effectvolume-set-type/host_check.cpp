// Host check only; not a build input.
#include "set_type.cpp"
#include <cstring>
#include <string>
static std::string tr;
extern "C" void DetachAllResources__14VIEffectVolume(void*) { tr += "D;"; }
extern "C" void FreeParticles__14VIEffectVolume(void*) { tr += "F;"; }
int main() {
    alignas(4) unsigned char o[128];
    auto w = [&](int off) { return *reinterpret_cast<unsigned*>(o + off); };
    std::memset(o, 0x5a, sizeof o);
    *reinterpret_cast<float*>(o + 44) = 7.0f;
    SetType__14VIEffectVolume18VIEffectVolumeTypef(o, 3, 2.5f);
    if (tr != "D;F;" || w(36) != 3 || *reinterpret_cast<float*>(o + 44) != 2.5f || w(72) != 1) return 1;
    SetType__14VIEffectVolume18VIEffectVolumeTypef(o, 9, -1.0f);
    if (w(36) != 9 || *reinterpret_cast<float*>(o + 44) != 2.5f) return 2;
    SetType__14VIEffectVolume18VIEffectVolumeTypef(o, 4, 0.0f);
    if (w(36) != 4 || *reinterpret_cast<float*>(o + 44) != 0.0f) return 3;
    float nan = __builtin_nanf("");
    SetType__14VIEffectVolume18VIEffectVolumeTypef(o, 5, nan);
    if (w(36) != 5 || *reinterpret_cast<float*>(o + 44) != 0.0f) return 4;
    for (int i = 0; i < 128; i++) if (i != 44 && !(i >= 36 && i < 40) && !(i >= 72 && i < 76) && i != 45 && i != 46 && i != 47 && o[i] != 0x5a) return 5;
    return 0;
}
