// Observed subfontPrintu__FiiPUsi at 0x00173408, 1352 bytes: draws a zero-terminated 16-bit string with the
// current font (curFont), at most `limit` units, at screen position (x, y). The coordinates are fixed-point
// in the output units. Nothing is returned. Offsets in the font are decimal bytes; the font has a
// signed halfword count at +0, a glyph array address at +8 (16-byte entries), a kerning table address at
// +12 (6-byte entries), a texture record address at +16 (its halfword at +2 is a texture id), and from +20
// 256 signed halfwords mapping a code below 256 to a glyph index (negative means unmapped).
// Entry check: y + 100 as unsigned must be at most 900 and limit must be nonzero, otherwise nothing is done.
// Setup: supersampleMode (a signed byte) picks the unit scale. Off: x' = (x << 4) + 27648, y' = (y << 4) +
//   29184, extra offset 0, advance shift 4, glyph shift 4. On: x' = (x << 5) + 22528, y' = (y << 4) + 29184,
//   advance shift 5, glyph shift 3; the offset and, for odd y, y' depend on engineGetField__Fv: with odd y,
//   y' becomes (y << 4) + 29200 when the first call's low bit is set and (y << 4) + 29168 otherwise, and
//   the offset is ((signed halfword at texture + 2) >> 1) << 4 when a second call's low bit is set, else
//   -4; with even y, a set low bit gives offset 0, otherwise that same expression minus 4.
// For each unit: 10 is a newline-like code that draws nothing. A code below 256 maps through the font's
//   table (an unmapped entry takes the table entry at +110, the one for code 45); a larger code is found by
//   searching the first-halfword codes of the glyph entries (index 0 when absent). Index 2047 draws and
//   advances nothing; index 0 only advances x' by the halfword at glyph0 + 12 shifted by the advance
//   shift. Any other index draws a glyph:
//   - kerning: the halfword at glyph + 14, if not negative, indexes 6-byte kerning entries; while an entry's
//     second halfword equals this index, one whose first halfword equals the previous glyph index adds its
//     signed third halfword (shifted left by 4 with supersampling, else 3) to x' and ends the search;
//   - indices 91 to 104 are drawn in grey: fontColor__FUiii is called with ((fontCharAlpha, or the top
//     byte of lastFontColor when that is -1) << 24 | 0x606060), -1, 1; any other glyph first restores
//     fontColor(lastFontColor, lastFontCharAlpha, 0) if a grey glyph was drawn earlier;
//   - the glyph's halfwords at +2, +4, +6, +8, +10 give u0 = h2 << 4, u1 = h4 << 4, v0 = h6 << glyph shift,
//     v1 = h8 << glyph shift, and a height h10 << glyph shift; engineFullFrameMode nonzero draws through
//     drawTexture__FR6Packetiiiiiiiii (packet p, texture id, x', y', x' + 2 * (u1 - u0), y' + 2 * (h +
//     v1 - v0), u0, v0, u1, v1) and advances x' by (halfword at glyph + 12) << 5; otherwise four 64-bit
//     words are written at p's cursor (which moves on by 32) in the supersample or the plain layout below,
//     advancing by that halfword << 5 (supersample) or << 4. nmChars is incremented for each drawn glyph.
//   - the previous glyph index (initially -1) is replaced after every unit, including newlines.
// At the end a pending grey colour is undone with fontColor(lastFontColor, lastFontCharAlpha, 0).
// The packed words are 64-bit sums of sign-extended 32-bit values as the original forms them.
// Unknown: the meaning of the extra offset and of the grey glyph range beyond what is stated.
#if defined(NORRATH_HOST_SUBFONT)
extern unsigned char* norrath_subfont_arena;
#endif

struct Packet {
    unsigned cursor;
};

extern "C" {
extern signed char supersampleMode;
extern int engineFullFrameMode;
extern unsigned char* curFont;
extern int fontCharAlpha;
extern int lastFontCharAlpha;
extern unsigned char lastFontColor[4];
extern int nmChars;
extern Packet packet __asm__("p");
int engineGetField__Fv();
void fontColor__FUiii(unsigned color, int alpha, int flag);
void drawTexture__FR6Packetiiiiiiiii(Packet& packet, int a, int b, int c, int d, int e, int f, int g, int h, int i);
}

namespace {
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_SUBFONT)
    return norrath_subfont_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
inline int sh(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const short*>(base + offset); }
inline unsigned uh(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const unsigned short*>(base + offset); }
inline long long pack(int high, int low) {
    return static_cast<long long>(static_cast<int>(static_cast<unsigned>(high) << 16)) | static_cast<long long>(low);
}
inline void put(unsigned base, unsigned offset, long long value) {
    *reinterpret_cast<long long*>(address(base) + offset) = value;
}
}

extern "C" void subfontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int limit) {
    if (static_cast<unsigned>(y + 100) > 900u) return;
    if (limit == 0) return;
    int lastGlyph = -1;
    int greyPending = 0;
    int screenX, screenY, offset, advanceShift, glyphShift;
    if (supersampleMode != 0) {
        int base = y << 4;
        screenY = base + 29184;
        screenX = (x << 5) + 22528;
        if (screenY & 0x10) {
            screenY = (engineGetField__Fv() & 1) ? base + 29200 : base + 29168;
            if (engineGetField__Fv() & 1) {
                offset = (sh(address(word(curFont, 16)), 2) >> 1) << 4;
            } else {
                offset = -4;
            }
        } else if (engineGetField__Fv() & 1) {
            offset = 0;
        } else {
            offset = ((sh(address(word(curFont, 16)), 2) >> 1) << 4) - 4;
        }
        advanceShift = 5;
        glyphShift = 3;
    } else {
        screenX = (x << 4) + 27648;
        screenY = (y << 4) + 29184;
        offset = 0;
        advanceShift = 4;
        glyphShift = 4;
    }
    while (*text != 0) {
        const unsigned char* font = curFont;
        unsigned code = *text;
        unsigned glyph;
        if (code == 10) {
            glyph = 2047;
        } else if (code < 256) {
            int mapped = sh(font, 20 + 2 * code);
            glyph = mapped >= 0 ? static_cast<unsigned>(mapped) : uh(font, 110);
        } else {
            glyph = 0;
            int count = sh(font, 0);
            if (count > 0) {
                const unsigned char* entry = address(word(font, 8));
                for (int i = 0; i < count; ++i, entry += 16) {
                    if (uh(entry, 0) == code) {
                        glyph = static_cast<unsigned>(i) & 0xffffu;
                        break;
                    }
                }
            }
        }
        if (glyph != 2047) {
            if (glyph == 0) {
                screenX += sh(address(word(font, 8)), 12) << advanceShift;
            } else {
                const unsigned char* g = address(word(font, 8)) + (glyph << 4);
                int u0 = sh(g, 2) << 4;
                int v0 = sh(g, 6) << glyphShift;
                int u1 = sh(g, 4) << 4;
                int v1 = sh(g, 8) << glyphShift;
                int height = sh(g, 10) << glyphShift;
                int kern = sh(g, 14);
                if (kern >= 0) {
                    const unsigned char* entry = address(word(font, 12)) + kern * 6;
                    if (uh(entry, 2) == glyph) {
                        for (;;) {
                            if (static_cast<int>(uh(entry, 0)) == lastGlyph) {
                                int adjust = sh(entry, 4);
                                screenX += supersampleMode != 0 ? adjust << 4 : adjust << 3;
                                break;
                            }
                            entry += 6;
                            if (uh(entry, 2) != glyph) break;
                        }
                    }
                }
                if (glyph - 91u < 14u) {
                    unsigned alpha = fontCharAlpha == -1 ? lastFontColor[3] : static_cast<unsigned>(fontCharAlpha);
                    greyPending = 1;
                    fontColor__FUiii((alpha << 24) | 0x606060u, -1, 1);
                } else if (greyPending) {
                    greyPending = 0;
                    fontColor__FUiii(*reinterpret_cast<unsigned*>(lastFontColor), lastFontCharAlpha, 0);
                }
                int advance;
                if (engineFullFrameMode != 0) {
                    drawTexture__FR6Packetiiiiiiiii(packet, sh(address(word(curFont, 16)), 2), screenX,
                                                    screenY + (height << 1), screenX + ((u1 - u0) << 1),
                                                    screenY + ((height + (v1 - v0)) << 1), u0, v0, u1, v1);
                    advance = sh(address(word(curFont, 8)) + (glyph << 4), 12) << 5;
                } else if (supersampleMode != 0) {
                    unsigned cursor = packet.cursor;
                    put(cursor, 0, pack(offset + v0 + 4, u0 + 4));
                    put(cursor, 8, pack(screenY + (height << 1), screenX));
                    put(cursor, 16, pack(offset + v1 + 4, u1 + 4));
                    put(cursor, 24, pack(screenY + ((height + (v1 - v0)) << 1), screenX + ((u1 - u0) << 1)));
                    packet.cursor = cursor + 32;
                    advance = sh(address(word(curFont, 8)) + (glyph << 4), 12) << 5;
                } else {
                    unsigned cursor = packet.cursor;
                    put(cursor, 0, pack(v0 + 8, u0 + 8));
                    put(cursor, 24, pack(screenY + height + (v1 - v0), screenX + u1 - u0));
                    put(cursor, 8, pack(screenY + height, screenX));
                    put(cursor, 16, pack(v1 + 8, u1 + 8));
                    packet.cursor = cursor + 32;
                    advance = sh(address(word(curFont, 8)) + (glyph << 4), 12) << 4;
                }
                screenX += advance;
                nmChars = nmChars + 1;
            }
        }
        lastGlyph = static_cast<int>(glyph);
        ++text;
        if (--limit == 0) break;
    }
    if (greyPending) fontColor__FUiii(*reinterpret_cast<unsigned*>(lastFontColor), lastFontCharAlpha, 0);
}
