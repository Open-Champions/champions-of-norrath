// Host comparison only. Not an EE build input.
#include "anim_lists.cpp"

#include <cstdio>

int main() {
    if (sizeof fsList != sizeof(void*) || sizeof arList != sizeof(void*) ||
        sizeof arListBack != sizeof(void*)) {
        return 1;
    }
    // All three start null, as the nobits image starts cleared.
    if (fsList != nullptr || arList != nullptr || arListBack != nullptr) return 2;
    // Writable, and three distinct words: the initializer stores a head, a
    // pool, and a tail record through them.
    static unsigned char pool[8];
    fsList = pool;
    arList = pool;
    arListBack = pool + 4;
    if (fsList != pool || arList != pool || arListBack != pool + 4) return 3;
    if (&fsList == reinterpret_cast<unsigned char**>(&arList)) return 4;
    fsList = nullptr;
    arList = nullptr;
    arListBack = nullptr;
    return 0;
}
