#include <cstddef>
extern "C" unsigned char hermDeltaScale[4];
int main() {
    if (sizeof(hermDeltaScale) != 4) return 1;
    for (std::size_t i = 0; i < sizeof(hermDeltaScale); ++i) if (hermDeltaScale[i] != 0) return 2;
    return 0;
}
