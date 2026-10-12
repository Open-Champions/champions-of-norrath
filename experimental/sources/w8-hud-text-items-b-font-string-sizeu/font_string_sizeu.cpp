// Observed fontStringSizeu__FP4FontPUsi at 0x001744a0, 20 bytes.
// It forwards its three arguments unchanged to _fontStringSizeu__FP4FontPUsi
// at 0x00173140 and returns that result. The measuring routine is not defined
// here, and the meaning of the returned value is not established by this unit.
// The Font argument is kept opaque.
extern "C" int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int color);

extern "C" int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int color) {
    return _fontStringSizeu__FP4FontPUsi(font, text, color);
}
