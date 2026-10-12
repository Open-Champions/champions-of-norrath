// Host check for animStarted. Not an EE plan input.
#include "anim_started.cpp"

int main() {
    if (sizeof(animStarted) != 1 || animStarted != 0) return 1;
    return 0;
}
