#include <cstddef>
extern "C" unsigned char msgTimeout[32];
int main() {
    if (sizeof(msgTimeout) != 32) return 1;
    for (std::size_t i = 0; i < sizeof(msgTimeout); ++i) if (msgTimeout[i] != 0) return 2;
    return 0;
}
