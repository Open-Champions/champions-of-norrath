// Observed: p starts at 0x006e93d0 in zero-filled storage; the next symbol, ParticleEmitterArray, starts 16
// bytes later and alwaysLoadedPage precedes it. _fontPrintu__FiiPUsi, fontColor__FUiii and fontEnd__Fv take
// its address; fontEnd__Fv also loads a word from it and the file's static initializer stores one word.
// Inferred: four 32-bit words, of which the authored font routines use three as cursor, start and tag
// pointers. The size is the gap to the next symbol.
// Unknown: the fourth word and the original type name.

extern "C" {
unsigned p[4];
}
static_assert(sizeof(p) == 16, "p");
