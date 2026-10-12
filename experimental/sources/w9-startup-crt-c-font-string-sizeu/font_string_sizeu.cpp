// Observed _fontStringSizeu__FP4FontPUsi at 0x00173140, 404 bytes. It measures a
// string of 16-bit characters with a font and returns the width in whole pixels.
// The count argument bounds the characters measured; the string also ends at the
// first zero element, and a zero count measures nothing. It is the 16-bit
// counterpart of _fontStringSize__FP4FontPci and shares its kerning scan and its
// fixed-point arithmetic (sums in sixteenths of a pixel, result shifted right by 4).
//
// Font fields read here (offsets observed; names inferred from use):
//   +0    signed 16 bits: number of glyph records searched for characters >= 256
//   +8    glyph records, 16 bytes each: unsigned 16-bit character code at +0, a
//         signed 16-bit advance at +12 and a signed 16-bit first-kerning index at +14
//   +12   kerning entries, 6 bytes each (previous glyph, glyph, signed amount)
//   +20   signed 16-bit glyph numbers for characters 0 through 255
// Glyph choice per character c:
//   c == 10        glyph 2047: contributes nothing and does not become the previous
//                  glyph, but still uses up one count
//   c < 256        the number at +20 + 2c; when it is negative, the unsigned 16-bit
//                  value at font +110 (the entry for character 45) is used instead
//   c >= 256       the index of the first glyph record whose code equals c, or 0
//                  when there is none (also 0 when the record count is not positive)
// Glyph 2047, whether it came from character 10 or from the number table, is skipped as
// described above. Glyph 0 (the only non-positive value that can occur) adds record 0's
// advance shifted left by 4. Any other glyph scans the kerning entries exactly as the
// narrow version does, then adds the glyph's advance shifted left by 4.
// supersampleMode nonzero shifts a matched kern amount left by 4, zero by 3.
// Unknown: the meaning of glyph 2047 beyond its use for character 10, and why the
// fallback is the entry for character 45.
extern "C" {
extern signed char supersampleMode;

struct FontGlyph {
    unsigned short code;
    unsigned char between[10];
    short advance;
    short first_kern;
};

struct FontKern {
    unsigned short previous;
    unsigned short glyph;
    short amount;
};

struct Font {
    short glyph_count;
    unsigned char header[6];
    FontGlyph* glyphs;
    FontKern* kerns;
    void* texture;
    short glyph_of_char[256];
};

#if defined(__mips__)
static_assert(sizeof(FontGlyph) == 16, "layout");
static_assert(sizeof(FontKern) == 6, "layout");
static_assert(__builtin_offsetof(Font, glyphs) == 8, "layout");
static_assert(__builtin_offsetof(Font, kerns) == 12, "layout");
static_assert(__builtin_offsetof(Font, glyph_of_char) == 20, "layout");
#endif

int _fontStringSizeu__FP4FontPUsi(Font* font, const unsigned short* text, int count) {
    int previous = -1;
    int width = 0;
    if (count != 0 && *text != 0) {
        for (;;) {
            unsigned code = *text;
            int glyph = 2047;
            if (code != 10) {
                if (code < 256) {
                    short mapped = font->glyph_of_char[code];
                    glyph = mapped >= 0 ? static_cast<unsigned short>(mapped)
                                        : static_cast<unsigned short>(font->glyph_of_char[45]);
                } else {
                    glyph = 0;
                    if (font->glyph_count > 0) {
                        const FontGlyph* record = font->glyphs;
                        for (int index = 0; index < font->glyph_count; ++index, ++record) {
                            if (record->code == code) {
                                glyph = static_cast<unsigned short>(index);
                                break;
                            }
                        }
                    }
                }
            }
            if (glyph != 2047) {
                const FontGlyph* glyphs = font->glyphs;
                if (glyph > 0) {
                    const FontGlyph* record = glyphs + glyph;
                    if (record->first_kern >= 0) {
                        const FontKern* entry = font->kerns + record->first_kern;
                        if (entry->glyph == glyph) {
                            for (;;) {
                                if (entry->previous == previous) {
                                    width += supersampleMode != 0 ? entry->amount * 16 : entry->amount * 8;
                                    break;
                                }
                                ++entry;
                                if (entry->glyph != glyph)
                                    break;
                            }
                        }
                    }
                    width += record->advance * 16;
                } else {
                    width += glyphs->advance * 16;
                }
                previous = glyph;
            }
            if (--count == 0)
                break;
            ++text;
            if (*text == 0)
                break;
        }
    }
    return width >> 4;
}
}
