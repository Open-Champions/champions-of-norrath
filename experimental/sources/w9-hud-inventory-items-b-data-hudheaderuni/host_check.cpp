#include <cstddef>
extern "C" unsigned char hudHeaderUni[256];
int main() {
    if (sizeof(hudHeaderUni) != 256) return 1;
    for (std::size_t i = 0; i < sizeof(hudHeaderUni); ++i) if (hudHeaderUni[i] != 0) return 2;
    return 0;
}
