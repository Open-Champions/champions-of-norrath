// Host check only; not a build input.
#include "set_flags.cpp"
#include <cstring>
int main() {
    alignas(4) unsigned char o[128];
    std::memset(o, 0x5a, sizeof o);
    SetFlags__14VIEffectVolumeUi(o, 0xdeadbeefu);
    if (*reinterpret_cast<unsigned*>(o + 60) != 0xdeadbeefu) return 1;
    for (int i = 0; i < 128; i++) if ((i < 60 || i >= 64) && o[i] != 0x5a) return 2;
    SetFlags__14VIEffectVolumeUi(o, 0);
    return *reinterpret_cast<unsigned*>(o + 60) != 0;
}
