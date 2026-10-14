// Host check for nm_defered_calls.cpp; not an EE plan input.
#include "nm_defered_calls.cpp"

int main() {
    if (nmDeferedCalls != 0) return 1;
    nmDeferedCalls = 3;
    if (nmDeferedCalls != 3) return 2;
    return 0;
}
