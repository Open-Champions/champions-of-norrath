// Host check only; not an EE build input. Checks the observed extent (124 bytes) and the zero initial value.
#include <cstdio>
extern "C" signed char dwarf_reg_size_table[124];
int main() {
    if (sizeof(dwarf_reg_size_table) != 124) return 1;
    for (signed char v : dwarf_reg_size_table) if (v != 0) return 2;
    std::puts("dwarf_reg_size_table ok");
    return 0;
}
