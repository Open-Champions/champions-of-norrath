// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "get_model.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
int main() {
    int bad = 0;
    unsigned char object[1100];
    unsigned char table[64];
    unsigned char first[2], second[2];
    rtti_c::host_note(table);
    rtti_c::host_note(first);
    rtti_c::host_note(second);
    for (unsigned selector : {0u, 1u, 2u, 0x80000000u}) {
        std::memset(object, 0, sizeof object);
        std::memset(table, 0, sizeof table);
        rtti_c::write_word(rtti_c::address(object) + 36u, rtti_c::address_bits(table));
        rtti_c::write_word(rtti_c::address(object) + 1048u, selector);
        rtti_c::write_word(rtti_c::address(table) + 12u, rtti_c::address_bits(first));
        rtti_c::write_word(rtti_c::address(table) + 28u, rtti_c::address_bits(second));
        void* got = norrath_lava_monster_get_model(object);
        if (got != (selector == 0u ? static_cast<void*>(first) : static_cast<void*>(second))) { std::printf("selector %x\n", selector); ++bad; }
    }
    std::printf(bad ? "failed\n" : "norrath_lava_monster_get_model passed\n");
    return bad != 0;
}
