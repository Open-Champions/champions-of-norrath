#include "combine_table.cpp"

int main() {
    if (sizeof(combineTable) != 1020) return 1;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(combineTable);
    // Whole-table FNV-1a and byte sum computed from the image bytes (little-endian words).
    unsigned hash = 0x811c9dc5u;
    unsigned sum = 0;
    for (unsigned index = 0; index < 1020; ++index) {
        hash = (hash ^ bytes[index]) * 0x01000193u;
        sum += bytes[index];
    }
    if (hash != 3055359525u || sum != 77030u) return 2;
    int zeros = 0;
    for (unsigned word : combineTable) zeros += word == 0;
    if (zeros != 17) return 3;
    if (combineTable[0] != 0x467e08u || combineTable[1] != 0x467e18u || combineTable[212] != 0 || combineTable[254] != 0) return 4;
    return 0;
}
