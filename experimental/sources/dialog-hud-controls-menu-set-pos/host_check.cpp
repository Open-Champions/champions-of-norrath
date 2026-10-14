extern "C" void menuSetPos__FP4Menui(unsigned* menu, int position);

static int expect(unsigned* menu, int slot, unsigned packed, unsigned sentinel) {
    if (menu[slot] != packed) return 1;
    if (menu[slot + 1] != sentinel) return 1;
    return 0;
}

int main() {
    unsigned empty[4] = {0u, 0x11111111u, 0x22222222u, 0x33333333u};
    menuSetPos__FP4Menui(empty, 4);
    if (empty[0] != 0u || empty[1] != 0xffffffffu || empty[2] != 0x22222222u) return 1;
    empty[1] = 0x11111111u;
    menuSetPos__FP4Menui(empty, -3);
    if (empty[1] != 0xffffffffu) return 2;

    unsigned one[8] = {0x10u, 0x111u, 0x222u, 0x333u, 0u, 0x444u, 0x555u, 0x666u};
    menuSetPos__FP4Menui(one, 9);
    if (expect(one, 5, 0u, 0x555u) != 0) return 3;
    if (one[0] != 0x10u || one[1] != 0x111u) return 4;

    unsigned four[24] = {};
    four[0] = 1u;
    four[4] = 2u;
    four[8] = 3u;
    four[12] = 4u;
    four[17] = 0x777u;
    four[18] = 0x888u;
    menuSetPos__FP4Menui(four, 5);
    if (four[17] != 0x303u || four[18] != 0x888u) return 5;
    menuSetPos__FP4Menui(four, 2);
    if (four[17] != 0x202u) return 6;
    menuSetPos__FP4Menui(four, -1);
    if (four[17] != 0u) return 7;
    if (four[0] != 1u || four[12] != 4u || four[16] != 0u) return 8;
    return 0;
}
