#include "storage.hpp"
int main() {
    static const unsigned char expect[64] = {};
    if (sizeof(cd) != 64) return 1;
    for (unsigned i = 0; i != 64; ++i) {
        if (cd[i] != expect[i]) return 2;
    }
    return 0;
}
