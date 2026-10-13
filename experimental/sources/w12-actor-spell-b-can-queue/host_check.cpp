// Host contract for canQueue__5Spell; not an EE input. Includes the source.
#include "can_queue.cpp"
#include <cstdio>

int main() {
    int storage[4] = {1, 2, 3, 4};
    if (canQueue__5Spell(storage) != 0) { std::puts("fail: result"); return 1; }
    if (canQueue__5Spell(nullptr) != 0) { std::puts("fail: null this"); return 1; }
    if (storage[0] != 1 || storage[3] != 4) { std::puts("fail: wrote"); return 1; }
    std::puts("canQueue ok");
    return 0;
}
