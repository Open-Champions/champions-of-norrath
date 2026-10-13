// Observed data objects of the render package, all taken from the image's symbol table and the initial
// bytes the image holds for them. No code is involved.
//  spellAlpha     0x00408560, 16 bytes: four words, all zero in the image. The sibling clear function
//                 (authored elsewhere) treats it as four ints.
//  spellAlphaInc  0x004085d0, 16 bytes: four words, all zero in the image, treated the same way.
//  texLastDecompressionTime 0x004840d0, one word, zero in the image; the decompress driver stores a
//                 timer difference there as an int.
//  worldTileEntityLightScale 0x00484128, one float; the image holds 1.0f (bytes 00 00 80 3f).
//  waterPatch     0x00484138, one word, zero in the image; waterInit and the water draw treat it as the
//                 base address of an array of 4784-byte patch records.
// Whether the zero ones share a section with other initialised data in the image is a linker matter and
// is not asserted here. The BSS-only objects with the same-named duplicates (trailParticle,
// trailParticleSpark, soulParticle, slideDustParticleDark) are not defined here: the symbol table lists
// several unrelated objects under the first two names and their element types are not established.
extern "C" {
int spellAlpha[4];
int spellAlphaInc[4];
int texLastDecompressionTime;
float worldTileEntityLightScale = 1.0f;
unsigned char* waterPatch;
}
