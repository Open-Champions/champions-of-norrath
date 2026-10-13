// Host check only; not an EE build input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include <cstdio>
#include <cstring>
extern "C" { unsigned char dwarf_reg_size_table[160]; int dwarf_reg_size_table_initialized; }
#include "init_reg_size_table.cpp"
int main() {
    int bad = 0;
    std::memset(dwarf_reg_size_table, 0xaa, sizeof dwarf_reg_size_table);
    init_reg_size_table();
    for (int i = 0; i < 160; ++i) {
        int want = 0xaa;
        if (i <= 30) want = 8;
        else if (i >= 32 && i <= 63) want = 4;
        else if (i >= 64 && i <= 66) want = 16;
        else if (i >= 67 && i <= 75) want = 4;
        else if (i >= 76 && i <= 78) want = 8;
        else if (i >= 79 && i <= 89) want = 0;
        else if (i >= 90 && i <= 121) want = 8;
        if (dwarf_reg_size_table[i] != want) { std::printf("index %d: %d want %d\n", i, dwarf_reg_size_table[i], want); ++bad; }
    }
    if (dwarf_reg_size_table_initialized != 1) { std::puts("flag"); ++bad; }
    return bad;
}
