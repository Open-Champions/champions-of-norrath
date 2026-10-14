#include <cstddef>
extern "C" unsigned char lastFontCharAlpha[4];
int main() {
    if (sizeof(lastFontCharAlpha) != 4) return 1;
    static const unsigned char want[4] = { 0xff, 0xff, 0xff, 0xff };
    for (std::size_t i = 0; i < sizeof(want); ++i) if (lastFontCharAlpha[i] != want[i]) return 2;
    return 0;
}
