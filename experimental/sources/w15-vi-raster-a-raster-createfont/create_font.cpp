// Observed: CreateFont__8VIRasteri at 0x01113bd0 is 140 bytes in the pinned SLUS-20565 image. It allocates 88 bytes with __builtin_new
// and, when nonnull, runs the VIRFont constructor (__7VIRFont) on it. A null allocation returns -1. Otherwise it adds the pointer to
// the pool at +19508 of the raster with Add__t6VIPool1ZP7VIRFontRCP7VIRFont, which returns an index; a negative index returns -1. On
// success, if the argument is nonzero it adds one to the word at +4 of the new font. It returns the index.
// Inferred: the pool at +19508 is the font table that Font__8VIRasteri and ShareFont__8VIRasteri read (entry base pointer at +19516); the
// +4 word is a reference count.
// Unknown: the pool layout, the constructor's effects, and what happens to a font whose Add failed (it is not freed).
// Nothing here ran on the EE.
extern "C" {
unsigned char* __builtin_new(unsigned size);
void __7VIRFont(unsigned char* self);
int Add__t6VIPool1ZP7VIRFontRCP7VIRFont(unsigned char* pool, unsigned char** item);
int CreateFont__8VIRasteri(unsigned char* self, int share) {
    unsigned char* font = __builtin_new(88);
    if (font) __7VIRFont(font);
    unsigned char* slot = font;
    if (!slot) return -1;
    int index = Add__t6VIPool1ZP7VIRFontRCP7VIRFont(self + 19508, &slot);
    if (index < 0) return -1;
    if (share) *reinterpret_cast<int*>(slot + 4) += 1;
    return index;
}
}
