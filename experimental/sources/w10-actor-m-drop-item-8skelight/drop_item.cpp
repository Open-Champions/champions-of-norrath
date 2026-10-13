// Observed dropItem__8Skelighti at 0x2473f0, 12 bytes. Both arguments are ignored. It stores a zero
// 64-bit value at the start of the file-local data object partlist.1103 (0x7d8010) and returns nothing.
// Unknown: the size and layout of partlist.1103 (the symbol table gives it no size) and why dropping an
// item resets it. The definition of partlist.1103 is not authored here, so it stays an import.
extern "C" {
extern unsigned char norrath_skelight_partlist_1103[] __asm__("partlist.1103");
}

typedef unsigned long long __attribute__((may_alias)) AliasedDouble;

extern "C" void dropItem__8Skelighti(void*, int) {
    *reinterpret_cast<AliasedDouble*>(norrath_skelight_partlist_1103) = 0ull;
}
