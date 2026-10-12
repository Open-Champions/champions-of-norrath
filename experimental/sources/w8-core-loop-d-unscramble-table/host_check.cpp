// Host contract for makeUnscrambleTable__Fv. Not an EE plan input.
extern "C" unsigned char unscrambleTable[256] = {};
#include "unscramble_table.cpp"
int main() {
    for (int i = 0; i < 256; ++i) unscrambleTable[i] = 0x5a;
    makeUnscrambleTable__Fv();
    bool seen[256] = {};
    for (int i = 0; i < 256; ++i) {
        if (seen[unscrambleTable[i]]) return 1;      // a permutation of all byte values
        seen[unscrambleTable[i]] = true;
    }
    // first entries and a position-weighted sum, taken from the instruction-level comparison
    static const unsigned char head[8] = {0, 36, 8, 44, 1, 37, 9, 45};
    for (int i = 0; i < 8; ++i) if (unscrambleTable[i] != head[i]) return 2;
    unsigned sum = 0;
    for (int i = 0; i < 256; ++i) sum += static_cast<unsigned>(unscrambleTable[i]) * static_cast<unsigned>(i + 1);
    return sum == 5542080u ? 0 : 3;
}
