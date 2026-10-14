// Observed zero-initialised storage in the pinned SLUS-20565 image.
//   body          0x00486ad8, word; stored once and loaded once (the load
//                 is tested against zero and passed on as an argument).
//   cursorTween   0x00486b74, float; six loads and five stores.
//   fullLaunchOK  0x00486cbb, byte; read as signed, stored once.
//   kickMenuList  0x004869e8, bytes; the next symbol is 8 bytes later.
//                 pause-menu code indexes it from 1 for the candidate list.
//   letterAtATime 0x00486b2c, word; three stores, four loads.
// Inferred: the element widths, from the load/store opcodes; kickMenuList
// is bounded by the 8-byte distance to its neighbour, not by an ELF size.
// Unknown: what body points at or holds, and the units of cursorTween.
extern "C" {
unsigned int body;
float cursorTween;
signed char fullLaunchOK;
unsigned char kickMenuList[8];
int letterAtATime;
}
