// Observed __ti6PhProp at 0x8eb248 in .bss, 12 bytes, zero at load. It is
// the type_info object of class PhProp: __tf6PhProp (the accessor that fills it
// on first use) and the pointer-type accessor __tfP6PhProp read and write it as
// three words. Exported here under the name its users import,
// norrath_ti_6PhProp.
// Inferred: three 32-bit words (name, table, base), filled at run time and
// left zero here. Unknown: any use outside those accessors.
extern "C" {
alignas(4) unsigned char norrath_ti_6PhProp[12] = {};
}
