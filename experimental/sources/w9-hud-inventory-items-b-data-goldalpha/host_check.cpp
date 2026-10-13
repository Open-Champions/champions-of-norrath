#include <cstddef>
extern "C" unsigned char goldAlpha[16];
int main() {
    if (sizeof(goldAlpha) != 16) return 1;
    for (std::size_t i = 0; i < sizeof(goldAlpha); ++i) if (goldAlpha[i] != 0) return 2;
    return 0;
}
