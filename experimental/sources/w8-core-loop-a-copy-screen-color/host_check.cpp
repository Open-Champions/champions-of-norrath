// Host check for copyScreenColor. Not an EE plan input.
#include "copy_screen_color.cpp"

int main() {
    if (sizeof(copyScreenColor) != sizeof(void*) || copyScreenColor != 0) return 1;
    return 0;
}
