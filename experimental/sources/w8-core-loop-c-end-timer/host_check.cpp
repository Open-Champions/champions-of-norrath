// Host contract for endTimer. Not an EE plan input.
#include "end_timer.cpp"

int main() {
    if (endTimer != 1) return 1;
    if (sizeof endTimer != 4u) return 2;
    endTimer = endTimer + 1;
    if (endTimer != 1 + 1) return 3;
    return 0;
}
