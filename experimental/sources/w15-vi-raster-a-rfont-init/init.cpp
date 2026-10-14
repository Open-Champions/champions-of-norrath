// Observed: Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi at 0x01135e40 is 304 bytes in the pinned SLUS-20565 image.
// Arguments (EABI, eight registers): this, raster, dictionary, an int (named `size` here, nothing more is known), a byte pointer
// `data`, an int `extra`, a bit-depth enum value, a final int. It stores `extra` at this+20 and `size` at this+40, then runs a fixed
// sequence of helpers, returning -1 as soon as one result is negative:
//   CalcMaxWidth__7VIRFontiPUc(this, size, data); CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(this, raster,
//   dictionary, size, this[20], bitDepth, last); CalcDataSize__7VIRFontPUc(this, data); Allocate__7VIRFont(this).
// After Allocate it adds the word at this+84 to each of the words at +64, +68, +76 and +72 (turning four offsets into addresses inside
// that buffer), then InitSlots__7VIRFontR8VIRaster(this, raster) and InitChars__7VIRFontPUc(this, data). The result is -1 if the last
// call's result is negative, otherwise 0.
// Inferred: this is the font loader; the four relocated words are tables inside the buffer Allocate produced, and +84 is its base.
// Unknown: the roles of the unnamed ints, what each helper does (none is authored here), and what the data bytes are.
// Nothing here ran on the EE.
extern "C" {
int CalcMaxWidth__7VIRFontiPUc(unsigned char* self, int size, unsigned char* data);
int CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(unsigned char* self, unsigned char* raster, unsigned char* dictionary,
                                                                     int size, int extra, int bitDepth, int last);
int CalcDataSize__7VIRFontPUc(unsigned char* self, unsigned char* data);
int Allocate__7VIRFont(unsigned char* self);
int InitSlots__7VIRFontR8VIRaster(unsigned char* self, unsigned char* raster);
int InitChars__7VIRFontPUc(unsigned char* self, unsigned char* data);

int Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi(unsigned char* self, unsigned char* raster, unsigned char* dictionary, int size,
                                                             unsigned char* data, int extra, int bitDepth, int last) {
    *reinterpret_cast<int*>(self + 20) = extra;
    *reinterpret_cast<int*>(self + 40) = size;
    if (CalcMaxWidth__7VIRFontiPUc(self, size, data) < 0) return -1;
    if (CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi(self, raster, dictionary, size, *reinterpret_cast<int*>(self + 20), bitDepth, last) < 0) return -1;
    if (CalcDataSize__7VIRFontPUc(self, data) < 0) return -1;
    if (Allocate__7VIRFont(self) < 0) return -1;
    unsigned base = *reinterpret_cast<unsigned*>(self + 84);
    *reinterpret_cast<unsigned*>(self + 64) += base;
    *reinterpret_cast<unsigned*>(self + 68) += base;
    *reinterpret_cast<unsigned*>(self + 76) += base;
    *reinterpret_cast<unsigned*>(self + 72) += base;
    if (InitSlots__7VIRFontR8VIRaster(self, raster) < 0) return -1;
    return InitChars__7VIRFontPUc(self, data) < 0 ? -1 : 0;
}
}
