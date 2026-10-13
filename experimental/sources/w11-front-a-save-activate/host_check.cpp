// Host check for saveActivate__9Savepoint (source: ../w8-actor-virtuals-c-save-activate).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin -DNORRATH_W8C_HOST host_check.cpp
// The observed body stores the word 1 at object+204 and touches nothing else.
#include "../w8-actor-virtuals-c-save-activate/save_activate.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

int main() {
    alignas(8) unsigned char object[512];
    for (unsigned initial : {0u, 1u, 7u, 0xffffffffu}) {
        for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 7u + 3u);
        unsigned char before[512];
        std::memcpy(before, object, sizeof object);
        std::memcpy(object + 204, &initial, 4);
        std::memcpy(before + 204, &initial, 4);
        saveActivate__9Savepoint(object);
        const unsigned char expect[4] = {1, 0, 0, 0};
        std::memcpy(before + 204, expect, 4);
        if (std::memcmp(object, before, sizeof object) != 0) return 1;
    }
    std::puts("save activate passed");
    return 0;
}
