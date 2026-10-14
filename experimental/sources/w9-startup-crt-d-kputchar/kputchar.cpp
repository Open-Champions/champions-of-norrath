// Observed kputchar at 0x002d6778, 60 bytes (a local symbol in the image).
// It polls the word at physical address 0x1000f130 until bit 0x8000 reads clear,
// then stores the low byte of its argument at 0x1000f180 and returns the
// argument unchanged. Both addresses are memory-mapped I/O; the loop reads the
// word afresh on every pass.
// Inferred, not shown by the code: the pair is a serial transmit status/data
// port, because serialPutchar (0x002d6868) feeds characters to it.
// The two macros let a host check substitute ordinary memory for the registers.
#ifndef W9_KPUTCHAR_STATUS
#define W9_KPUTCHAR_STATUS (*reinterpret_cast<volatile unsigned*>(0x1000f130u))
#endif
#ifndef W9_KPUTCHAR_DATA
#define W9_KPUTCHAR_DATA (*reinterpret_cast<volatile unsigned char*>(0x1000f180u))
#endif

extern "C" int kputchar(int character) {
    while ((W9_KPUTCHAR_STATUS & 0x8000u) != 0u) {
    }
    W9_KPUTCHAR_DATA = static_cast<unsigned char>(character);
    return character;
}
