#include <cstdio>
#include <cstring>
#include "strticmp.cpp"
// 257-byte table with the upper-case class bit (bit 0) on A-Z only, built like the newlib table.
extern "C" const unsigned char _ctype_[257] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int main() {
    if (norrath_strticmp("Mummy", "mummy") != 0) return 1;
    if (norrath_strticmp("MUMMY", "mummy") != 0) return 2;
    if (norrath_strticmp("mummy lord", "MUMMY") != 0) return 3;      // prefix matches
    if (norrath_strticmp("mum", "mummy") != 1) return 4;             // first string shorter
    if (norrath_strticmp("mummy", "mummies") != 1) return 5;
    if (norrath_strticmp("", "") != 0) return 6;
    if (norrath_strticmp("anything", "") != 0) return 7;
    if (norrath_strticmp("", "x") != 1) return 8;
    if (norrath_strticmp("a", "b") != 1 || norrath_strticmp("b", "a") != 1) return 9;   // never -1
    if (norrath_strticmp("[", "{") != 1) return 10;                  // only A-Z fold
    const char text[] = "WoodElf";
    if (norrath_strticmp(text, "woodelf") != 0 || norrath_strticmp(text, "WOODELFS") != 1) return 12;
    std::puts("strticmp contract passed");
    return 0;
}
