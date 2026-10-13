// Host contract for gameButtonToIcon__Fi. Not an EE plan input.
#include "game_button_to_icon.cpp"
int main() {
    const int mask[] = {0x80, 0x20, 0x10, 0x40, 0x8000, 0x2000, 0x1000, 0x4000, 4, 1, 8, 2};
    const int icon[] = {125, 123, 124, 126, 127, 128, 129, 130, 131, 132, 133, 134};
    for (int i = 0; i < 12; ++i)
        if (gameButtonToIcon__Fi(mask[i]) != icon[i]) return 1 + i;
    const int other[] = {0, 3, 0x100, 0x200, 0x400, 0x800, 0x10000, -1, 0x82, 0x180};
    for (int v : other)
        if (gameButtonToIcon__Fi(v) != -1) return 20;
    return 0;
}
