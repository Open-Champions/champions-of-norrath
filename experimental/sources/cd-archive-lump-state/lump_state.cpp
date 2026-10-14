// Loader words that this package owns. Extents are the bytes from each
// symbol to the next symbol. The file image of each one is zero.
// lumpLoadName at 0x006d86c0 ends at globDir, 192 bytes later.
// discardable at 0x006cc680 ends at layer0Dir, 32 bytes later.
// lumpLoadLump, lumpLoadSize, and lumpLoadCompletionFlag are the four-byte,
// four-byte, and one-byte objects at 0x00484014, 0x00484018, and 0x00484011.
extern "C" {
unsigned char lumpLoadLump[4] = {};
unsigned char lumpLoadSize[4] = {};
unsigned char lumpLoadCompletionFlag = 0;
unsigned char lumpLoadName[192] = {};
unsigned char discardable[32] = {};
}
