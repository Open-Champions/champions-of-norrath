// Observed fontStringSizeu__FP4FontPUsi at 0x001744a0, 20 bytes.
// The last argument is a character count (inferred: fontPrintWrapu__FiiPUsiiii passes
// a count in that position); it is not a colour.
// It forwards its three arguments unchanged to _fontStringSizeu__FP4FontPUsi
// at 0x00173140 and returns that result. The measuring routine is not defined
// here, and the meaning of the returned value is not established by this unit.
// The Font argument is kept opaque.
extern "C" int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count);

extern "C" int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count) {
    return _fontStringSizeu__FP4FontPUsi(font, text, count);
}
