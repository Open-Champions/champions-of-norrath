// Observed storage used by the texture page streaming state machine (texProcessLoad):
//   loadCompleteFlag        0x00486981  1 byte,  zero in the file (the done byte for the async stream)
//   loadPage                0x00486982  2 bytes, zero (block id being loaded)
//   loadKillListSize        0x004840f4  4 bytes, zero
//   loadSegmentsLoaded      0x004840f8  4 bytes, zero
//   needToClearCacheBuffer  0x00484108  1 byte,  file value 1
//   reallyLoad.873          0x00484107  1 byte,  file value 0 (a function-local static in the
//                                       original; the dotted name is the image's symbol)
//   loadKillList            0x006e9380  zero in the file; the next symbol, alwaysLoadedPage, starts
//                                       16 bytes later, so 16 bytes are reserved. The image's
//                                       symbol has size 0, so 16 is the gap, not a stated size.
// The names of the first five and loadKillList are the image's own local symbols.
// Inferred: loadKillList holds slot numbers as signed bytes (the loader stores
// static_cast<signed char>(slot)); no field meaning is claimed beyond that.
// Unknown: how many slots the loader can queue (it may exceed the 16-byte gap).
extern "C" {
signed char loadCompleteFlag = 0;
short loadPage = 0;
int loadKillListSize = 0;
int loadSegmentsLoaded = 0;
signed char needToClearCacheBuffer = 1;
signed char reallyLoad_873 __asm__("reallyLoad.873") = 0;
signed char loadKillList[16] = {};
}
static_assert(sizeof(loadCompleteFlag) == 1 && sizeof(loadPage) == 2 && sizeof(loadKillList) == 16, "observed sizes");
