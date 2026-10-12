// Host check for copyScreen. Not an EE plan input.
#include "copy_screen.cpp"

int main() {
    if (sizeof(copyScreen) != 10240) return 1;
    if (sizeof(copyScreen) < 1154 * 8) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(copyScreen) % 16 != 0) return 3;
    for (unsigned char b : copyScreen) if (b) return 4;
    return 0;
}
