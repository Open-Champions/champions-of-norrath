// Observed the assert format at 0x4635e8, 49 bytes in .rodata: the
// NUL-terminated string
//   Assertion "%s" failed in %s:%d  context:%s (%x)\n
// __assert__FPCcUiT0i passes it to scePrintf three times with the
// condition, file, line, context word, and return address, then stores
// zero at address zero. Bytes below are the image at that address.
// Observed the assert context word, original name context at 0x4840f0, 4
// bytes in .sdata. Its image value is 0x004840e8, the address of the
// string "none" just before it, which the %s in the format above prints
// until set otherwise.
// Inferred: they are norrath_assert_format_view and
// norrath_assert_context_word_view, read by the sound assert helper.
// Unknown: who stores a new context, and placement/section identity of
// these relocatable objects with .rodata/.sdata.
extern "C" const unsigned char norrath_assert_format_view[49] = {
    0x41, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x22, 0x25,
    0x73, 0x22, 0x20, 0x66, 0x61, 0x69, 0x6c, 0x65, 0x64, 0x20, 0x69, 0x6e,
    0x20, 0x25, 0x73, 0x3a, 0x25, 0x64, 0x20, 0x20, 0x63, 0x6f, 0x6e, 0x74,
    0x65, 0x78, 0x74, 0x3a, 0x25, 0x73, 0x20, 0x28, 0x25, 0x78, 0x29, 0x0a,
    0x00,
};
extern "C" unsigned char norrath_assert_context_word_view[4] = {
    0xe8, 0x40, 0x48, 0x00,
};
