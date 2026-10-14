// Observed pitchTab at 0x006cb078: zero-initialised storage (.bss). Its only observed
// reader, animGetVolumeScale__Fii, clamps its index to 0..127, so 128 floats (512 bytes)
// are the smallest size consistent with the code; the distance to the next symbol
// (lumps at 0x006cb280) is 520 bytes, which leaves eight bytes that look like
// alignment padding. The size is therefore inferred, not observed. Its writers and its
// meaning are not established.
extern "C" {
float pitchTab[128];
}
