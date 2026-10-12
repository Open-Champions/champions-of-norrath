// Texture-cache, lump and megatile storage observed in the executable
// (SLUS_205.65). All initial values read from the file image are zero.
//   texmap           0x00430750, 0x1900 bytes, .data
//   texdecompress    0x00355cd0, 0x30 bytes,   .data
//   nmLumps          0x00483f8c, 4 bytes,      .sdata
//   uncompSpin       0x004840ac, 4 bytes,      .sdata
//   texmapstart      0x00485c4e, 2 bytes,      .sdata
//   texmapend        0x00485c50, 2 bytes,      .sdata
//   megatile_ramkmax 0x00485c60, 4 bytes,      .sdata
//   uncompMap        0x006e5e80, .bss; the symbol has no size, the next symbol
//                    starts 0x2800 bytes later and a texture initialiser
//                    writes up to byte 10235, so 0x2800 bytes are reserved
//   segmentUsed      0x006e8f00, .bss; the next symbol starts 0x100 bytes later
//                    and an initialiser clears 256 bytes
// Names come from the executable's symbol table. Element types of the tables
// are not established; they are byte arrays. The integer widths follow the
// symbol sizes.
#if defined(NORRATH_HOST_TEXTURE_DATA)
#define NORRATH_LOADED(name)
#else
#define NORRATH_LOADED(name) __attribute__((section(name)))
#endif

extern "C" {
unsigned char texmap[0x1900] NORRATH_LOADED(".data") = {0};
unsigned char texdecompress[0x30] NORRATH_LOADED(".data") = {0};
unsigned nmLumps NORRATH_LOADED(".sdata") = 0;
int uncompSpin NORRATH_LOADED(".sdata") = 0;
short texmapstart NORRATH_LOADED(".sdata") = 0;
short texmapend NORRATH_LOADED(".sdata") = 0;
int megatile_ramkmax NORRATH_LOADED(".sdata") = 0;
unsigned char uncompMap[0x2800];
unsigned char segmentUsed[0x100];
}
