// Observed _fontStringSize__FP4FontPci at 0x00172ab8, 268 bytes. It measures a
// narrow (8-bit) string with a font and returns the width in whole pixels. The
// public wrappers pass the count of characters to measure; the string also ends at
// the first zero byte, and a zero count measures nothing.
//
// Font layout read here (offsets observed; the field names are inferred from use):
//   +8   glyph records, 16 bytes each; record n has a signed 16-bit advance at
//        +12 and a signed 16-bit first-kerning-entry index at +14 (negative: none)
//   +12  kerning entries, 6 bytes each: previous glyph (16 bits, unsigned), glyph
//        (16 bits, unsigned), amount (signed 16 bits)
//   +20  signed 16-bit glyph numbers indexed by the character byte
// Per character: glyph = number for the byte. A glyph of 0 or less adds the
// advance of record 0, shifted left by 4. Otherwise, if its first-kerning index is
// not negative, the entries from that index on are scanned for one whose previous
// glyph equals the glyph of the character before (initially -1, never matching),
// stopping when an entry's glyph differs from the current one. A match adds its
// amount shifted left by 4 when supersampleMode is nonzero and by 3 otherwise. The
// record's advance, shifted left by 4, is added after any match. The sum is
// returned arithmetically shifted right by 4.
// Unknown: what the unmapped parts of the font header (offsets 0 and 4) hold.
extern "C" {
extern signed char supersampleMode;

struct FontGlyph {
    unsigned char before_advance[12];
    short advance;
    short first_kern;
};

struct FontKern {
    unsigned short previous;
    unsigned short glyph;
    short amount;
};

struct Font {
    unsigned char header[8];
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

int _fontStringSize__FP4FontPci(Font* font, const char* text, int count) {
    int previous = -1;
    int width = 0;
    if (count != 0 && *text != 0) {
        for (;;) {
            int glyph = font->glyph_of_char[static_cast<unsigned char>(*text)];
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
