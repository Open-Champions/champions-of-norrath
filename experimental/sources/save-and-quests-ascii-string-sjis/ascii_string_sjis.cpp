// Observed AsciiString2Sjis__FPUcPUs at 0x0024fc10, 160 bytes. Each byte of
// the source string is converted by Ascii2Sjis__FUc. The low two bytes of the
// returned code are swapped and stored as one halfword, in order. A zero
// halfword follows the last one: one halfword per source byte plus a
// terminator. Ascii2Sjis__FUc is not defined in this package.
extern "C" {
__SIZE_TYPE__ strlen(const char* text);
unsigned Ascii2Sjis__FUc(unsigned char character);

void AsciiString2Sjis__FPUcPUs(unsigned char* source, unsigned short* destination) {
    int length = static_cast<int>(strlen(reinterpret_cast<char*>(source)));
    int index = 0;
    if (length > 0) {
        do {
            const unsigned code = Ascii2Sjis__FUc(source[index]);
            index += 1;
            *destination = static_cast<unsigned short>(((code >> 8) & 0xffu) | ((code & 0xffu) << 8));
            destination += 1;
        } while (index < length);
    }
    *destination = 0;
}
}
