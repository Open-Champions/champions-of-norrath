#include "font_string_sizeu.cpp"

signed char supersampleMode = 1;

int main() {
    static FontGlyph glyphs[5];
    static FontKern kerns[3];
    static Font font;
    glyphs[0].code = 0;      glyphs[0].advance = 5;  glyphs[0].first_kern = -1;
    glyphs[1].code = 'A';    glyphs[1].advance = 10; glyphs[1].first_kern = -1;
    glyphs[2].code = 'V';    glyphs[2].advance = 8;  glyphs[2].first_kern = 0;
    glyphs[3].code = 0x3042; glyphs[3].advance = 16; glyphs[3].first_kern = 1;
    glyphs[4].code = 0x3044; glyphs[4].advance = 15; glyphs[4].first_kern = -1;
    kerns[0] = {1, 2, -2};        // 'A' then 'V'
    kerns[1] = {2, 3, -3};        // 'V' then U+3042
    kerns[2] = {9, 9, 99};
    font.glyph_count = 5;
    font.glyphs = glyphs;
    font.kerns = kerns;
    font.glyph_of_char['A'] = 1;
    font.glyph_of_char['V'] = 2;
    font.glyph_of_char['n'] = -1;           // negative: use the entry for character 45
    font.glyph_of_char[45] = 1;             // so 'n' is measured as glyph 1
    static const unsigned short A[] = {'A', 0}, AV[] = {'A', 'V', 0}, VA3[] = {'V', 0x3042, 0};
    static const unsigned short A_nl_A[] = {'A', 10, 'A', 0}, n[] = {'n', 0}, q[] = {'?', 0};
    static const unsigned short hira[] = {0x3044, 0x3042, 0x4e00, 0}, empty[] = {0};
    if (_fontStringSizeu__FP4FontPUsi(&font, A, 1) != 10) return 1;
    if (_fontStringSizeu__FP4FontPUsi(&font, A, 0) != 0) return 2;
    if (_fontStringSizeu__FP4FontPUsi(&font, empty, 3) != 0) return 3;
    if (_fontStringSizeu__FP4FontPUsi(&font, q, 1) != 5) return 4;       // unmapped: glyph 0
    if (_fontStringSizeu__FP4FontPUsi(&font, n, 1) != 10) return 5;
    // Supersampled kerning: 10 + 8 - 2 = 16.
    if (_fontStringSizeu__FP4FontPUsi(&font, AV, 2) != 16) return 6;
    supersampleMode = 0;
    if (_fontStringSizeu__FP4FontPUsi(&font, AV, 2) != 17) return 7;
    supersampleMode = 1;
    // 'V' then U+3042 searches the records: 8 + 16 - 3 = 21.
    if (_fontStringSizeu__FP4FontPUsi(&font, VA3, 2) != 21) return 8;
    // A line feed adds nothing and does not become the previous glyph, but uses a count.
    if (_fontStringSizeu__FP4FontPUsi(&font, A_nl_A, 3) != 20) return 9;
    if (_fontStringSizeu__FP4FontPUsi(&font, A_nl_A, 2) != 10) return 10;
    // Wide characters: U+3044 (15), U+3042 (16, no match for previous 4), U+4E00 not found -> glyph 0 (5).
    if (_fontStringSizeu__FP4FontPUsi(&font, hira, 3) != 36) return 11;
    // With no records, every wide character is glyph 0.
    font.glyph_count = 0;
    if (_fontStringSizeu__FP4FontPUsi(&font, hira, 3) != 15) return 12;
    return 0;
}
