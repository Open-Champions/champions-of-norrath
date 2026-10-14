#include <cstddef>
extern "C" unsigned char inventoryLump[4];
int main() {
    if (sizeof(inventoryLump) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(inventoryLump); ++i) if (inventoryLump[i] != 0) return 2;
    return 0;
}
