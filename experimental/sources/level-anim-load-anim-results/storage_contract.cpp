#include "storage.hpp"
int main() {
    static const unsigned char expect[156800] = {};
    if (sizeof(animResults) != 156800) return 1;
    for (unsigned i = 0; i != 156800; ++i) {
        if (animResults[i] != expect[i]) return 2;
    }
    return 0;
}
