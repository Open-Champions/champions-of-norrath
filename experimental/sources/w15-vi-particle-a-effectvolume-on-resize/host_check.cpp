// Host check only; not a build input.
#include "on_resize.cpp"
#include <cstring>
int main() {
    alignas(4) unsigned char o[128], e[128];
    std::memset(o, 0x5a, sizeof o); std::memset(e, 0x5a, sizeof e);
    float box[6] = {1, 2, 3, 4, 5, 6};
    OnResize__14VIEffectVolumeRC6VIBBox(o, box);
    float* ef = reinterpret_cast<float*>(e);
    ef[2] = 1; ef[3] = 2; ef[4] = 3; ef[5] = 4; ef[6] = 5; ef[7] = 6;
    reinterpret_cast<unsigned*>(e)[8] = 1; reinterpret_cast<unsigned*>(e)[18] = 1;
    return std::memcmp(o, e, sizeof o) != 0;
}
