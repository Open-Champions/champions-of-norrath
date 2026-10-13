#include <cstddef>
extern "C" unsigned char invGrid[4];
int main() {
    if (sizeof(invGrid) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(invGrid); ++i) if (invGrid[i] != 0) return 2;
    return 0;
}
