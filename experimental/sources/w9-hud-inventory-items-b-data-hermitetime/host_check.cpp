#include <cstddef>
extern "C" unsigned char hermiteTime[4];
int main() {
    if (sizeof(hermiteTime) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(hermiteTime); ++i) if (hermiteTime[i] != 0) return 2;
    return 0;
}
