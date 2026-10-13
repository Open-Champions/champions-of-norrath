// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "material-get-layer-color.cpp"
#include <cstring>
#include <initializer_list>

int main() {
    alignas(8) unsigned char material[48 + 144 * 4];
    for (unsigned i = 0; i < sizeof material; ++i) material[i] = static_cast<unsigned char>(i * 13 + 5);
    for (int layer = 0; layer < 4; ++layer) {
        unsigned char buffer[12];
        std::memset(buffer, 0xee, sizeof buffer);
        void* r = GetLayerColor__C10VIMateriali(buffer + 3, material, layer);
        if (r != buffer + 3) return 1;
        if (std::memcmp(buffer + 3, material + 56 + 144 * layer, 4) != 0) return 2;
        for (int i : {0, 1, 2, 7, 8, 9, 10, 11}) if (buffer[i] != 0xee) return 3;
    }
    return 0;
}
