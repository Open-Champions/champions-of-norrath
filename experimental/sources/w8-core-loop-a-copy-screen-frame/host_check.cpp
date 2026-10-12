// Host check for copyScreenFrame. Not an EE plan input.
#include "copy_screen_frame.cpp"

int main() {
    if (sizeof(copyScreenFrame) != sizeof(void*) || copyScreenFrame != 0) return 1;
    return 0;
}
