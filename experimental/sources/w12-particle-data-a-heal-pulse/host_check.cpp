#include <cstddef>

extern "C" unsigned short healColor[4];
extern "C" unsigned short healPulse[4];

int main() {
    if (sizeof healColor != 8 || sizeof healPulse != 8) return 1;
    for (std::size_t i = 0; i < 4; ++i) {
        if (healColor[i] != 0 || healPulse[i] != 0) return 2;
    }
    return 0;
}
