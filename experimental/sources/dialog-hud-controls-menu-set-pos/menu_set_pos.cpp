// Observed menuSetPos__FP4Menui at 0x001940e0, 108 bytes.
// Records are 16 bytes. A zero first word is an empty menu: the packed
// position stored at byte 4 is -1. Otherwise the walk stops on the first
// zero word. A negative position becomes 0, and a position past the last
// live record becomes that last index. The stored word is
// position | (position << 8), written at byte 4 of the terminating record.
extern "C" void menuSetPos__FP4Menui(unsigned* menu, int position) {
    int index = 0;
    unsigned* entry = menu;
    if (menu[0] != 0u) {
        index = 1;
        for (;;) {
            entry = menu + static_cast<unsigned>(index) * 4u;
            if (entry[0] == 0u) break;
            ++index;
        }
    }
    index -= 1;
    if (position < 0) position = 0;
    if (index < position) position = index;
    unsigned packed = static_cast<unsigned>(position);
    packed |= packed << 8;
    entry[1] = packed;
}
