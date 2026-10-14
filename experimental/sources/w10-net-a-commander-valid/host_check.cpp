// Host check for commanderValid__14WoodElfSoldier. Not an EE plan input.
#include <initializer_list>
#include "commander_valid.cpp"
int main() {
    alignas(8) static unsigned char soldier[1024], commander[64], record[64];
    *reinterpret_cast<unsigned char**>(soldier + 960) = 0;
    if (commanderValid__14WoodElfSoldier(soldier) != 0) return 1;
    *reinterpret_cast<unsigned char**>(soldier + 960) = commander;
    *reinterpret_cast<unsigned char**>(commander + 12) = record;
    for (unsigned flags : {0u, 8u, 0xffffffffu, 0xfffffff7u, 7u, 0x10u, 0x18u}) {
        *reinterpret_cast<unsigned*>(record + 12) = flags;
        if (commanderValid__14WoodElfSoldier(soldier) != ((flags & 8) == 0)) return 2;
    }
    return 0;
}
