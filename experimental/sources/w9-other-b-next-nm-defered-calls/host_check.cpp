// Host check for next_nm_defered_calls.cpp; not an EE plan input.
#include "next_nm_defered_calls.cpp"

int main() {
    if (nextNmDeferedCalls != 0) return 1;
    nextNmDeferedCalls = 2;
    if (nextNmDeferedCalls != 2) return 2;
    return 0;
}
