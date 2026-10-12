#include "find_global_dialog.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
signed char language = 2;
extern "C" unsigned char norrath_sound_global_dialog_map_view[12] = {};
static unsigned char* dir = nullptr;
extern "C" unsigned char* get__11ResourceMap(void* m) { if (m != norrath_sound_global_dialog_map_view) std::exit(90); return dir; }
static unsigned char table[80 * 8];
static void rec(int i, const char* name, short tag) {
    std::strcpy(reinterpret_cast<char*>(table + i * 80), name);
    table[i * 80 + 74] = tag & 255;
    table[i * 80 + 75] = (tag >> 8) & 255;
}
int main() {
    char a[] = "alpha", b[] = "beta", c[] = "gamma";
    if (soundFindGlobalDialog__FPc(a) != nullptr) return 1;           // no table
    dir = table;
    if (soundFindGlobalDialog__FPc(a) != nullptr) return 2;           // empty table
    rec(0, "alpha", 0); rec(1, "beta", 0); rec(2, "alpha", 2); rec(3, "alpha", 3);
    rec(4, "alpha", 0); rec(5, "alpha", 2);
    if (soundFindGlobalDialog__FPc(a) != table + 5 * 80) return 3;    // last language match
    if (soundFindGlobalDialog__FPc(b) != table + 1 * 80) return 4;    // only the zero tag
    if (soundFindGlobalDialog__FPc(c) != nullptr) return 5;           // no name
    language = 3;
    if (soundFindGlobalDialog__FPc(a) != table + 3 * 80) return 6;
    language = 7;
    if (soundFindGlobalDialog__FPc(a) != table + 4 * 80) return 7;    // last zero tag
    rec(6, "delta", 3);
    char d[] = "delta";
    if (soundFindGlobalDialog__FPc(d) != nullptr) return 8;           // tag 3 vs language 7, no zero
    language = -1;
    rec(6, "eps", -1);
    char e[] = "eps";
    if (soundFindGlobalDialog__FPc(e) != table + 6 * 80) return 9;    // negative halfword compares signed
    std::puts("find_global_dialog passed");
    return 0;
}
