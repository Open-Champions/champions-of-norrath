// Observed: the bss block at 0x007d7e20 is a function-local static (symbol buffer.1071) of
// getMinibossName__5Mummy. The next image symbol starts 16 bytes later, so 16 bytes is the
// extent the image allows; the only observed writer formats "MBMummy%d" with a signed byte, at
// most 11 characters plus the terminator. The block is uninitialised in the image.
// The authored getMinibossName__5Mummy imports it under this name.
extern "C" {
char mummy_miniboss_name_buffer_7d7e20[16];
}
