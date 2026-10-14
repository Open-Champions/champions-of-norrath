// Host contract for firstTime. Not an EE plan input.
#include "first_time.cpp"

int main() {
    if (firstTime != 0) return 1;
    if (sizeof firstTime != 1u) return 2;
    firstTime = firstTime + 1;
    if (firstTime != 0 + 1) return 3;
    return 0;
}
