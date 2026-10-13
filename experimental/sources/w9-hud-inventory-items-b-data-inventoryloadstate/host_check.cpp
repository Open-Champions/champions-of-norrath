#include <cstddef>
extern "C" unsigned char inventoryLoadState[4];
int main() {
    if (sizeof(inventoryLoadState) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(inventoryLoadState); ++i) if (inventoryLoadState[i] != 0) return 2;
    return 0;
}
