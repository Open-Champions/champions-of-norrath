// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_weapon.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    const unsigned want[] = {44, 48, 44, 48, 46, 50};
    const unsigned variants[] = {0, 1, 2, 3, 4, 5, 6, 7, 100, 0xffffffffu, 0x80000000u};
    for (unsigned v : variants) {
        unsigned char object[1000];
        std::memset(object, 0x5a, sizeof object);
        rtti_c::write_word(rtti_c::address(object) + 960u, v);
        rtti_c::write_word(rtti_c::address(object) + 964u, 0x11223344u);
        setWeapon__5Mummy(object);
        const unsigned expect = v < 6 ? want[v] : 0x11223344u;
        if (rtti_c::read_word(rtti_c::address(object) + 964u) != expect) { std::printf("variant %u\n", v); ++bad; }
        if (object[959] != 0x5a || object[968] != 0x5a) { std::printf("neighbours\n"); ++bad; }
    }
    std::printf(bad ? "failed\n" : "setWeapon__5Mummy passed\n");
    return bad != 0;
}
