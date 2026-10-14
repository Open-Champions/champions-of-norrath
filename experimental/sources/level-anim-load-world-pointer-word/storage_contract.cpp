#include "storage.hpp"
int main() {
    static const unsigned char expect[4] = {};
    if (sizeof(norrath_factory_world_pointer_word) != 4) return 1;
    for (unsigned i = 0; i != 4; ++i) {
        if (norrath_factory_world_pointer_word[i] != expect[i]) return 2;
    }
    return 0;
}
