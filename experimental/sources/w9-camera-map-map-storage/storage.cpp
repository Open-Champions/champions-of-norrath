// Observed storage for five map symbols. Sizes and initial contents come from
// the private SLUS_205.65 symbol table and section bytes read locally.
//  - mapButtonWasDown (0x0042a4b0, 16 bytes, .data): four words, all zero in
//    the file. The only indexing seen in the authored automap code is by player.
//  - mapObjList (0x00484f9c, 4 bytes, .sdata): one pointer-sized word, zero.
//  - mapScale (0x00486b04), mapX (0x00486b08), mapY (0x00486b0c): three floats
//    in .sbss, zero-initialized storage.
// The neighbouring floats mapScissorX (0x00486b10) and mapScissorY (0x00486b14)
// are not defined here. The layout of the map-icon and tile storage is not
// part of this unit. Nothing here was executed on the EE.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif

extern "C" {
int mapButtonWasDown[4] NORRATH_LOADED(".data") = {0, 0, 0, 0};
void* mapObjList NORRATH_LOADED(".sdata") = 0;
float mapScale NORRATH_LOADED(".sbss");
float mapX NORRATH_LOADED(".sbss");
float mapY NORRATH_LOADED(".sbss");
}
