// table at 0x006d8fa0, 32768 bytes, to unknownBuffer.
// BSS is zero. animInit later stores powf results at table + 8408.
// This is not noiseTable.
extern "C" {
unsigned char table[32768];
}
static_assert(sizeof table == 32768u, "table extent");
