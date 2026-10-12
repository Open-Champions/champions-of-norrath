// Observed startEntranceName at 0x0042a3f8, 32 bytes in .data. All 32 file
// bytes are zero. It is used as text by name, but its length bound is only the
// 32-byte symbol span (the next symbol, globalIcons, starts 32 bytes later).
extern "C" {
char startEntranceName[32] = {0};
}
