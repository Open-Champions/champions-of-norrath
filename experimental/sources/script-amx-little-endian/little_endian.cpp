// Observed amx_LittleEndian, four file-backed zero bytes at 0x004839f8.
// amx_Init stores 1 here after the little-endian probe. The following
// "%s.dbin" bytes belong to a different object.
extern "C" {
unsigned char norrath_amx_little_endian_word_view[4] = {0, 0, 0, 0};
}
