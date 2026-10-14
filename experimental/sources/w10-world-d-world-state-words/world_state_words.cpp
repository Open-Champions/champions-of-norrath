// World words and tables.
//   worldLastDrawSucceded (0x0048412d, 1 byte, .sdata, file-backed initial value 1): the byte
//     after worldDrawEnable; written by the world draw and precache code.
//   worldWaterRainTimer (0x004841c0, 4 bytes, .sdata, file-backed zero).
//   worldActivityLevel (0x00429440, 32 bytes, .data, file-backed zeros): recorded size 32;
//     the element type is not established, so it is eight words.
//   worldAnimation (0x006ebb40, .bss): ELF size is 0; the next object starts 0xc00 bytes later,
//     which is 64 records of 48 bytes (the table walked by the authored world draw). Inferred.
//   worldBoundingBox (0x007d0b08, .bss): ELF size is 0; the next object starts 0x18 bytes later,
//     which is two three-float points. Inferred.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char worldLastDrawSucceded NORRATH_LOADED(".sdata") = 1;
int worldWaterRainTimer NORRATH_LOADED(".sdata") = 0;
int worldActivityLevel[8] NORRATH_LOADED(".data") = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char worldAnimation[64 * 48] NORRATH_LOADED(".bss") = {0};
float worldBoundingBox[6] NORRATH_LOADED(".bss") = {0};
}
