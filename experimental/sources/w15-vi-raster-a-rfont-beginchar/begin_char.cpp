// Observed: BeginChar__7VIRFontUsRiPPUc at 0x011360b8 is 60 bytes in the pinned SLUS-20565 image. With e the 12-byte record at index
// (word at offset 36) of the array whose pointer is at offset 64 (0x40) of the font object, it stores the 16-bit argument into the
// halfword at e+6, writes the halfword at e+4 (zero-extended) through the int reference, writes (word at offset 84 (0x54)) + (word at e+0)
// through the pointer-to-pointer, and returns 0.
// Inferred: the offset-36 word is the current character counter advanced by EndChar__7VIRFont, e+0 an offset into a glyph bitmap
// buffer based at offset 84, e+4 a glyph size or stride, e+6 a slot for the caller's code.
// Unknown: the remaining record bytes (e+2, e+8..11), the array length, and the glyph buffer format. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int BeginChar__7VIRFontUsRiPPUc(unsigned char* self, unsigned short code, int* size, unsigned* data) {
    int index = *reinterpret_cast<int*>(self + 36);
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(self + 64));
    unsigned base = *reinterpret_cast<unsigned*>(self + 84);
    unsigned char* e = records + index * 12;
    unsigned short extent = *reinterpret_cast<unsigned short*>(e + 4);
    *reinterpret_cast<unsigned short*>(e + 6) = code;
    *size = extent;
    *data = base + *reinterpret_cast<unsigned*>(e);
    return 0;
}
}
