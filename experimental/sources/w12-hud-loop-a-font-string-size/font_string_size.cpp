// Observed fontStringSize__FP4FontPci at 0x00174390, 20 bytes. Observed: the entry is a
// forwarding stub. It passes its three arguments unchanged to
// _fontStringSize__FP4FontPci (0x00172ab8) as a jump and returns whatever that call
// leaves in v0. Inferred: the result is an int width, since callers use it in
// arithmetic (see the front-end weight display). Unknown: the unit of the width and
// the Font layout; the font is passed through as an opaque pointer.
extern "C" int _fontStringSize__FP4FontPci(void* font, const char* text, int count);

extern "C" int fontStringSize__FP4FontPci(void* font, const char* text, int count) {
    return _fontStringSize__FP4FontPci(font, text, count);
}
