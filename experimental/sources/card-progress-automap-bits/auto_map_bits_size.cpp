// Observed: _3map$autoMapBitsSize is the 4-byte word 0x16c8. MC_Init adds
// it to each level size. The recorded symbol size is 4. Bytes after it belong
// to other pooled objects.
// Inferred: none.
// Unknown: why the map bit count is 5832.
extern "C" int _3map$autoMapBitsSize = 0x16c8;
