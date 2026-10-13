// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "simplesprite-get-material-pal.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char object[136];
    for (unsigned seed = 1; seed <= 4; ++seed) {
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 37 + seed * 91);
        int want;
        std::memcpy(&want, object + 72, 4);
        unsigned char before[sizeof object];
        std::memcpy(before, object, sizeof object);
        if (GetMaterialPal__C14VISimpleSprite(object) != want) return 1;
        if (std::memcmp(before, object, sizeof object) != 0) return 2;
        int other = want ^ 0x5a5a5a5a;
        std::memcpy(object + 72, &other, 4);
        if (GetMaterialPal__C14VISimpleSprite(object) != other) return 3;
    }
    return 0;
}
