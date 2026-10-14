#include <cstddef>
extern "C" unsigned char lastFontColor[4];
int main() {
    if (sizeof(lastFontColor) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(lastFontColor); ++i) if (lastFontColor[i] != 0) return 2;
    return 0;
}
