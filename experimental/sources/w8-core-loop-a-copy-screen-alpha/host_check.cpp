// Host check for copyScreenAlpha. Not an EE plan input.
#include "copy_screen_alpha.cpp"

int main() {
    if (sizeof(copyScreenAlpha) != sizeof(void*) || copyScreenAlpha != 0) return 1;
    return 0;
}
