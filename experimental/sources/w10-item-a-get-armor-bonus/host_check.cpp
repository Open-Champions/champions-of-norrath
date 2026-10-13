// Host check for getArmorBonus__4itemPQ24item11description. The template word at
// item+0 is a token resolved through host_resolve. Not an EE plan input.
#define NORRATH_INVENTORY_HOST 1
#include "get_armor_bonus.cpp"
#include <cstring>
extern "C" unsigned char player[1];
unsigned char player[1];
static unsigned char tpl[64];
namespace inventory_items { const void* host_resolve(unsigned) { return tpl; } }

static unsigned char item[72];
static void set(int slot, short tag, short v) {
    short* e = reinterpret_cast<short*>(item + 36 + 8 * slot);
    e[0] = tag; e[1] = v; e[2] = 0; e[3] = 0;
}
static int run(short base) {
    std::memcpy(tpl + 24, &base, 2);
    unsigned one = 1;
    std::memcpy(item, &one, 4);
    return getArmorBonus__4itemPQ24item11description(item);
}
int main() {
    for (int i = 0; i < 4; ++i) set(i, 0, 55);
    if (run(100) != 100) return 1;                          // no tag 13
    set(2, 13, 25);
    if (run(100) != 125) return 2;                          // 25 percent
    if (run(7) != 8) return 3;                              // 7 * 0.25 truncates to 1
    if (run(-40) != -50) return 4;                          // negative base scales and truncates toward zero
    set(0, 13, 50);
    if (run(100) != 150) return 5;                          // first tag 13 wins
    set(0, 13, 0);
    if (run(100) != 100) return 6;                          // a zero first match gives no bonus (second is not consulted)
    set(0, 0, 0); set(1, 0, 0); set(2, 0, 0); set(3, 13, -50);
    if (run(80) != 40) return 7;                            // fourth slot, negative percent
    item[36 + 8 * 4 - 8 + 0] = 0;                           // slot 3 tag byte cleared -> no match
    if (run(80) != 80) return 8;
    return 0;
}
