// Three initialised data objects, with values read from the image.
//   effectModeSize  0x0042de10  40 bytes: ten words 128, 9920, 8000, 18496, 28640, 44512,
//                               63168, 98368, 98368, 15360.
//   defaultHalos    0x0034c018  16 bytes: 0a 0a 44 40 | 00 80 72 44 | 28 28 44 48 | 00 00 f4 43.
//   fxResourceMap   0x0042ad00  112 bytes: 28 pointer words. The even words 0..24 are
//                               pointers to 13 resource names; the odd words and words 26 and
//                               27 are zero at load.
// Observed: the sizes, values, and the neighbouring symbols (every extent equals the distance
// to the next object). The authored GutChunk constructor reads words 0, 20, 22 and 24 as
// resource names; the authored static initialiser for these particles refers to the same table.
// Inferred: effectModeSize is a per-audio-effect-mode buffer size (the authored sound code
// indexes it by mode); the odd words of fxResourceMap are filled at run time (they are zero
// in the file).
// Unknown: the type and meaning of defaultHalos beyond its four little-endian words, and
// which code fills the zero words of fxResourceMap.
extern "C" {
int effectModeSize[10] = {128, 9920, 8000, 18496, 28640, 44512, 63168, 98368, 98368, 15360};
unsigned char defaultHalos[16] = {0x0a, 0x0a, 0x44, 0x40, 0x00, 0x80, 0x72, 0x44,
                                  0x28, 0x28, 0x44, 0x48, 0x00, 0x00, 0xf4, 0x43};
}
#define S(text) const_cast<char*>(text)
extern "C" {
char* fxResourceMap[28] = {S("fx.lmp"), nullptr, S("blood.vif"), nullptr, S("blood.tex"), nullptr,
                           S("blood_yellow.vif"), nullptr, S("blood_yellow.tex"), nullptr, S("bloodpool.anm"), nullptr,
                           S("bloodsplat1.vif"), nullptr, S("bloodsplat1.tex"), nullptr, S("bloodsplat2.vif"), nullptr,
                           S("bloodsplat2.tex"), nullptr, S("gutchunks.vif"), nullptr, S("gutchunks.tex"), nullptr,
                           S("gutchunks_yellow.tex"), nullptr, nullptr, nullptr};
}
#undef S
static_assert(sizeof effectModeSize == 40u && sizeof defaultHalos == 16u, "extents");
#if defined(__mips__)
static_assert(sizeof fxResourceMap == 112u, "extent");
#endif
