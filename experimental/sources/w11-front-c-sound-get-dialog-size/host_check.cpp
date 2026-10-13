// Host check for soundGetDialogSize__Fi. Not an EE plan input.
#include <cstring>
alignas(8) static unsigned char table[80 * 8];
static const signed char* dir;
extern "C" const signed char* soundGetDialogDir__Fv() { return dir; }
extern "C" int soundGetDialogSize__Fi(int);
static int word(int off) { int v; std::memcpy(&v, table + off, 4); return v; }
static void put(int off, int v) { std::memcpy(table + off, &v, 4); }
int main() {
    dir = 0;
    if (soundGetDialogSize__Fi(0) != 0 || soundGetDialogSize__Fi(-1) != 0) return 1;
    // Record k starts at 80 * (k + 1); record -1 is the first 80 bytes.
    dir = reinterpret_cast<const signed char*>(table + 80);
    for (int w = 0; w < 80 * 8; w += 4) put(w, 1000 + w * 3);  // distinct words
    table[80] = 7;      // record 0
    table[160] = 0x80;  // record 1: negative byte is nonzero
    table[240] = 1;     // record 2
    table[320] = 0;     // record 3 is empty
    table[400] = 5;     // record 4 is past the empty one
    for (int i = 0; i <= 4; ++i) {
        int base = 80 * (i + 1);
        // An empty record anywhere in 0..i makes the answer 0.
        int expect = i >= 3 ? 0 : word(base + 144) - word(base + 64);
        if (soundGetDialogSize__Fi(i) != expect) return 10 + i;
    }
    // Negative index is untested: record -1 is table + 0.
    table[0] = 0;
    if (soundGetDialogSize__Fi(-1) != word(144) - word(64)) return 20;
    if (soundGetDialogSize__Fi(0) == 0) return 21;
    return 0;
}
