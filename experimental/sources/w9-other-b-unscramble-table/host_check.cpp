// Host check for unscramble_table.cpp; not an EE plan input.
#include "unscramble_table.cpp"

int main() {
    if (unscrambleTable[0] != 0 || unscrambleTable[255] != 0) return 1;
    unscrambleTable[255] = 9;
    if (unscrambleTable[255] != 9) return 2;
    return 0;
}
