#include "font_string_size.cpp"

signed char supersampleMode = 1;

int main() {
    static FontGlyph glyphs[4];
    static FontKern kerns[4];
    static Font font;
    // Glyph 0 (also used for unmapped characters): advance 5 px. Glyph 1 'A': 10 px.
    // Glyph 2 'V': 8 px, kerned against 'A' by -2 (in 1/16 px units after the shift).
    // Glyph 3 'W': first kern entry exists but never matches the previous glyph.
    glyphs[0].advance = 5;  glyphs[0].first_kern = -1;
    glyphs[1].advance = 10; glyphs[1].first_kern = -1;
    glyphs[2].advance = 8;  glyphs[2].first_kern = 0;
    glyphs[3].advance = 12; glyphs[3].first_kern = 2;
    kerns[0] = {1, 2, -2};   // 'A' then 'V'
    kerns[1] = {3, 2, -4};   // 'W' then 'V' (second entry of the same glyph)
    kerns[2] = {9, 3, 7};
    kerns[3] = {1, 1, 100};  // a following entry for a different glyph ends the scan
    font.glyphs = glyphs;
    font.kerns = kerns;
    font.glyph_of_char['A'] = 1;
    font.glyph_of_char['V'] = 2;
    font.glyph_of_char['W'] = 3;
    font.glyph_of_char['n'] = -4;   // a negative glyph number behaves like glyph 0
    // 'A' alone: 10. A zero count and an empty string measure nothing.
    if (_fontStringSize__FP4FontPci(&font, "A", 1) != 10) return 1;
    if (_fontStringSize__FP4FontPci(&font, "A", 0) != 0) return 2;
    if (_fontStringSize__FP4FontPci(&font, "", 5) != 0) return 3;
    // Unmapped and negative-numbered characters use glyph 0's advance.
    if (_fontStringSize__FP4FontPci(&font, "?", 1) != 5) return 4;
    if (_fontStringSize__FP4FontPci(&font, "n", 1) != 5) return 5;
    // 'AV' with supersampling: 10 + 8 + (-2 shifted by 4 -> -32 sixteenths = -2 px) = 16.
    if (_fontStringSize__FP4FontPci(&font, "AV", 2) != 16) return 6;
    // Without supersampling the kerning amount is shifted by 3: -16 sixteenths = -1 px.
    supersampleMode = 0;
    if (_fontStringSize__FP4FontPci(&font, "AV", 2) != 17) return 7;
    supersampleMode = 1;
    // 'WV' matches the second entry of the scan: 12 + 8 - 4 = 16.
    if (_fontStringSize__FP4FontPci(&font, "WV", 2) != 16) return 8;
    // 'VV' has no matching previous glyph: 16. A first-character glyph never kerns.
    if (_fontStringSize__FP4FontPci(&font, "VV", 2) != 16) return 9;
    // The count limits the measurement, and the string ends at the first zero byte.
    if (_fontStringSize__FP4FontPci(&font, "AAA", 2) != 20) return 10;
    if (_fontStringSize__FP4FontPci(&font, "AAA", 99) != 30) return 11;
    // A kern entry whose own glyph matches but whose previous glyph is unrelated is skipped.
    if (_fontStringSize__FP4FontPci(&font, "AW", 2) != 22) return 12;
    // The sum is shifted arithmetically: a negative total of -1/16 px rounds toward -infinity.
    glyphs[0].advance = 0;
    glyphs[1].advance = 0;
    glyphs[2].advance = -1;
    glyphs[2].first_kern = -1;
    glyphs[2].advance = 0;
    kerns[0].amount = -1;
    glyphs[2].first_kern = 0;
    if (_fontStringSize__FP4FontPci(&font, "AV", 2) != -1) return 13;
    return 0;
}
