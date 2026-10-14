#include <cstdio>
#include "attack_bonus.cpp"

static int run(short base, short k0, short v0, short k1, short v1, short k2, short v2, short k3, short v3) {
    AttackBase b{}; b.value = base;
    AttackRecord r{&b, {}, {{k0, v0, {}}, {k1, v1, {}}, {k2, v2, {}}, {k3, v3, {}}}};
    return getAttackBonus__4itemPQ24item11description(&r);
}

int main() {
    int bad = 0;
    if (run(100, 0, 0, 0, 0, 0, 0, 0, 0) != 100) bad++;
    if (run(100, 35, 50, 0, 0, 0, 0, 0, 0) != 150) bad++;
    if (run(100, 1, 9, 2, 9, 3, 9, 35, 10) != 110) bad++;   // fourth entry is read
    if (run(100, 35, 0, 35, 40, 0, 0, 0, 0) != 100) bad++;   // first match wins, zero adds nothing
    if (run(100, 0, 0, 0, 0, 0, 0, 0, 35) != 100) bad++;     // kind in a value slot is ignored
    if (run(7, 35, 33, 0, 0, 0, 0, 0, 0) != 9) bad++;        // 9.31 truncated
    if (run(100, 35, -20, 0, 0, 0, 0, 0, 0) != 80) bad++;
    if (run(-10, 35, 50, 0, 0, 0, 0, 0, 0) != -15) bad++;
    if (!bad) std::puts("getAttackBonus passed");
    return bad;
}
