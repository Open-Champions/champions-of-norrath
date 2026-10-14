// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "soundsprite-attenuation-mode.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char object[140];
    for (unsigned seed = 1; seed <= 4; ++seed) {
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 37 + seed * 91);
        int want;
        std::memcpy(&want, object + 76, 4);
        unsigned char before[sizeof object];
        std::memcpy(before, object, sizeof object);
        if (AttenuationMode__C13VISoundSprite(object) != want) return 1;
        if (std::memcmp(before, object, sizeof object) != 0) return 2;
        int other = want ^ 0x5a5a5a5a;
        std::memcpy(object + 76, &other, 4);
        if (AttenuationMode__C13VISoundSprite(object) != other) return 3;
    }
    return 0;
}
