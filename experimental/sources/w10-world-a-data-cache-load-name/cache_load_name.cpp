// Observed cacheLoadName at 0x007cc530 (.bss), 64 bytes. The span to the next symbol (zeroPad) is 64 bytes. The authored armor-cache storage notes name this symbol as the end of the cache table.
// Inferred: A text buffer holding the name being loaded into the cache.
// Unknown: The longest text written; the 64-byte span may include padding.
extern "C" {
char cacheLoadName[64];
}
