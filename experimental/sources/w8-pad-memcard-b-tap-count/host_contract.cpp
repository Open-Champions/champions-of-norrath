#include "tap_count.cpp"

int main() {
    if (sizeof tapCount != 2u) return 1;
    if (tapCount[0] != 1 || tapCount[1] != 1) return 2;
    return 0;
}
