// Observed lastFontCharAlpha at 0x0048411c, an initialised data object. It is nm-sized 4; the image word is 0xffffffff.
// The type and meaning are not established; the object is 4 bytes with the image contents.
extern "C" {
extern unsigned char lastFontCharAlpha[4];
alignas(4) unsigned char lastFontCharAlpha[4] = { 0xff, 0xff, 0xff, 0xff };
}
