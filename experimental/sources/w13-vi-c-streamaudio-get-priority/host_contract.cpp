// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "streamaudio-get-priority.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char object[132];
    for (unsigned seed = 1; seed <= 4; ++seed) {
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 37 + seed * 91);
        int want;
        std::memcpy(&want, object + 68, 4);
        unsigned char before[sizeof object];
        std::memcpy(before, object, sizeof object);
        if (GetPriority__C19VIStreamAudioSprite(object) != want) return 1;
        if (std::memcmp(before, object, sizeof object) != 0) return 2;
        int other = want ^ 0x5a5a5a5a;
        std::memcpy(object + 68, &other, 4);
        if (GetPriority__C19VIStreamAudioSprite(object) != other) return 3;
    }
    return 0;
}
