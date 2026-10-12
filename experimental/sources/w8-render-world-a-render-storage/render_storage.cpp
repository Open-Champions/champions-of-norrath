// Small storage objects that authored sources already import by their original
// names. Addresses are from the pinned SLUS_205.65 symbol table. Extents are the
// span to the next symbol in the same section (st_size is zero for these local
// symbols). Image bytes are zero except lastNmBlocksFree.
//
//   lastNmBlocksFree   0x00483ad8  4 bytes, .data, file word 0x000186a0 (100000).
//   cacheHead          0x0048696c  4 bytes, .sbss, zero.
//   cacheTail          0x00486970  4 bytes, .sbss, zero.
//   lastNmQ.1088       0x00486b20  4 bytes, .sbss, zero. Compiler name of a
//                                  function-local static; spelled with an asm label.
//   checkBlock         0x00486b40  4 bytes, .sbss, zero.
//   loadSampleRate.784 0x00486bac  4 bytes, .sbss, zero. Same kind of name.
//   decompressStack    0x006e1d80  16384 bytes, .bss, to `cache`.
//   cache              0x006e5d80  192 bytes, .bss, to dummyTex. Sixteen 12-byte
//                                  records as textureInternalInit__Fv uses it.
//
// A second local symbol named `cache` exists at 0x007cb770 (3520 bytes to
// cacheLoadName). Only the first one is imported by authored code here; the
// second is not defined by this file.
// The reviewed -G0 profile does not recreate .sdata or .sbss, so the zero words
// are ordinary zero storage.
extern "C" {
int lastNmBlocksFree = 100000;
int cacheHead;
int cacheTail;
int lastNmQ __asm__("lastNmQ.1088");
int checkBlock;
int loadSampleRate __asm__("loadSampleRate.784");
alignas(16) unsigned char decompressStack[16384];
alignas(16) unsigned char cache[192];
}
