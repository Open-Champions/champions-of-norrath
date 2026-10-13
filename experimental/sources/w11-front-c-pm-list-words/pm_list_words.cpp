// Observed pmlistchars at 0x00435f68 (4096 bytes in .data, zero in the file, symbol size
// 0x1000, after pmlistptrs) and pmlistcharsused at 0x00485e20 (4 bytes). addpmlist appends a
// string to the character pool and advances the used count; megatileEntitize stores zero in the
// count. Inferred: a string pool with a used-length cursor. Unknown: why the pool is 4096.
extern "C" {
char pmlistchars[0x1000];
int pmlistcharsused;
}
