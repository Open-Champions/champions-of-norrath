#include "storage.hpp"
int main() {
    static const unsigned char expect[56] = {};
    if (sizeof(scene) != 56) return 1;
    for (unsigned i = 0; i != 56; ++i) {
        if (scene[i] != expect[i]) return 2;
    }
    return 0;
}
