// Observed: dwarf_reg_size_table is the 124-byte bss object at 0x008eaa98 (the
// next symbol starts 0x7c bytes later). The authored table-fill routine writes a
// byte per register index. It is zero at load.
extern "C" unsigned char dwarf_reg_size_table[124] = {};
static_assert(sizeof(dwarf_reg_size_table) == 124, "dwarf_reg_size_table");
