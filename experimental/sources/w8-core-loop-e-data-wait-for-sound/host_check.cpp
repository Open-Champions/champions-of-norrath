#include "wait_for_sound.cpp"
int main() {
    static_assert(sizeof(waitForSound) == 1, "one byte");
    if (waitForSound != 0) return 1;
    waitForSound = 1;
    return waitForSound == 1 ? 0 : 2;
}
