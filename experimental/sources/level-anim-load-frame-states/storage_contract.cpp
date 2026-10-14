#include "storage.hpp"
int main() {
    static const unsigned char expect[580224] = {};
    if (sizeof(frameStates) != 580224) return 1;
    for (unsigned i = 0; i != 580224; ++i) {
        if (frameStates[i] != expect[i]) return 2;
    }
    return 0;
}
