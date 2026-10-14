#include <cstddef>
extern "C" unsigned char goldNumber[288];
int main() {
    if (sizeof(goldNumber) != 288) return 1;
    for (std::size_t i = 0; i < sizeof(goldNumber); ++i) if (goldNumber[i] != 0) return 2;
    return 0;
}
