// Observed textureSmoothBorder__FiiiiiUiiii at 0x0016efd8, 1552 bytes.
// Draws a bordered panel from pieces of the lump "interfac.lmp". Arguments:
// x, y, width, height, extra (passed on as the integer argument of the
// draws), color, style, skipFill, innerOnly. Nothing is drawn when fewer than
// 3000 draw-list units remain (GLISTSIZE - ((glistp - gstart) >> 3), see
// rectangleDraw). The style selects the piece-name prefix: 2 "tile", 1
// "btile", anything else "ctile". The nine names are prefix + topleft, topright,
// botleft, botright, left, right, bot, top, center, each with ".tex". Each
// piece is lumpFind(lump, name); the texture is word 1 of the result and its
// halfwords at 0 and 2 are the piece width and height. c is the width of the
// top-left piece.
//
// Unless innerOnly is nonzero, the four corners are drawn with
// _textureDrawColor(texture, px, py, extra, color) at
//   bottom-left  (x, y + height - c - 1)   bottom-right (x + width - c, same y)
//   top-left     (x, y - 1)                top-right    (x + width - c, y - 1)
// in that order. When width - 2c is positive the top and bottom pieces are
// then tiled across it, c units at a time starting at x + c, each step a
// top draw and then a bottom draw with textureDrawSuperColor(texture, px, py,
// extra, color, x, y, right, bottom, 0, 0): the top piece at py = y with
// right = x + width - c and bottom = y + height - c, the bottom piece at
// py = y + height - c with right = x + width - c (+ 4 when style is 2) and
// bottom = y + height. When height - 2c is positive the left and right pieces are
// tiled down the same way from y + c, a left draw then a right draw per step
// (left at px = x, right at px = x + width - c), with right = x + width and
// bottom = y + height - c.
//
// Then the centre piece is looked up (always; innerOnly does not skip it) and,
// unless skipFill is nonzero, tiled with textureDrawSuperColor in rows of its
// height and columns of its width. For style 2 the rows start at y + c - 26
// and cover (height - 2c + 50) offsets, the columns start at x + c - 25 and
// cover (width - 2c + 52), the clip right edge is x + width - (c - 25) + 1 and
// the clip bottom is y + height - (c - 26); a negative row range stops the
// function. For the other styles rows and columns start at y + c and x + c
// and cover height - 2c and width - 2c, with the clip right edge x + width - c
// and bottom y + height - c; a row range that is not positive stops the
// function. In both cases the clip left is x and the clip top is y.
struct NorrathBorderTexture {
    short width;
    short height;
};

struct NorrathLumpEntry {
    int unobserved;
    NorrathBorderTexture* texture;
};

extern "C" unsigned long glistp;
extern "C" unsigned long gstart;
extern "C" int GLISTSIZE;

extern "C" void* lumpLoad__FPc(char* name);
extern "C" NorrathLumpEntry* lumpFind__FPvPc(void* lump, char* name);
extern "C" void _textureDrawColor__FP8_textureiiiUi(NorrathBorderTexture* texture, int x, int y,
                                                    int extra, unsigned color);
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(NorrathBorderTexture* texture, int x,
                                                              int y, int extra, unsigned color,
                                                              int clipLeft, int clipTop,
                                                              int clipRight, int clipBottom,
                                                              char first, char second);
extern "C" int sprintf(char* destination, const char* format, ...);

extern "C" void textureSmoothBorder__FiiiiiUiiii(int x, int y, int width, int height, int extra,
                                                 unsigned color, int style, int skipFill,
                                                 int innerOnly) {
    int used = static_cast<int>(glistp - gstart) >> 3;
    if (GLISTSIZE - used < 3000) return;
    void* lump = lumpLoad__FPc(const_cast<char*>("interfac.lmp"));
    const char* prefix;
    if (style == 2) {
        prefix = "tile";
    } else if (style == 1) {
        prefix = "btile";
    } else {
        prefix = "ctile";
    }
    char topLeft[64], topRight[64], botLeft[64], botRight[64], bot[64], top[64], left[64],
        right[64], center[64];
    sprintf(topLeft, "%stopleft.tex", prefix);
    sprintf(topRight, "%stopright.tex", prefix);
    sprintf(botLeft, "%sbotleft.tex", prefix);
    sprintf(botRight, "%sbotright.tex", prefix);
    sprintf(left, "%sleft.tex", prefix);
    sprintf(right, "%sright.tex", prefix);
    sprintf(bot, "%sbot.tex", prefix);
    sprintf(top, "%stop.tex", prefix);
    sprintf(center, "%scenter.tex", prefix);
    int c = lumpFind__FPvPc(lump, topLeft)->texture->width;
    int doubleC = c * 2;
    if (innerOnly == 0) {
        int bottom = y + height;
        int cornerY = (bottom - c) - 1;
        _textureDrawColor__FP8_textureiiiUi(lumpFind__FPvPc(lump, botLeft)->texture, x, cornerY,
                                            extra, color);
        int rightX = (x + width) - c;
        _textureDrawColor__FP8_textureiiiUi(lumpFind__FPvPc(lump, botRight)->texture, rightX,
                                            cornerY, extra, color);
        int topY = y - 1;
        _textureDrawColor__FP8_textureiiiUi(lumpFind__FPvPc(lump, topLeft)->texture, x, topY,
                                            extra, color);
        _textureDrawColor__FP8_textureiiiUi(lumpFind__FPvPc(lump, topRight)->texture, rightX, topY,
                                            extra, color);
        int innerWidth = width - doubleC;
        if (innerWidth > 0) {
            int bottomRightClip = rightX;
            if (style == 2) bottomRightClip = rightX + 4;
            int stepX = x + c;
            int done = 0;
            do {
                textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                    lumpFind__FPvPc(lump, top)->texture, stepX, y, extra, color, x, y, rightX,
                    bottom - c, 0, 0);
                done += c;
                textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                    lumpFind__FPvPc(lump, bot)->texture, stepX, bottom - c, extra, color, x, y,
                    bottomRightClip, bottom, 0, 0);
                stepX += c;
            } while (done < innerWidth);
        }
        int innerHeight = height - doubleC;
        if (innerHeight > 0) {
            int done = 0;
            int stepY = y + c;
            do {
                done += c;
                textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                    lumpFind__FPvPc(lump, left)->texture, x, stepY, extra, color, x, y, x + width,
                    bottom - c, 0, 0);
                textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                    lumpFind__FPvPc(lump, right)->texture, (x + width) - c, stepY, extra, color, x,
                    y, x + width, bottom - c, 0, 0);
                stepY += c;
            } while (done < innerHeight);
        }
    }
    NorrathBorderTexture* centerTexture = lumpFind__FPvPc(lump, center)->texture;
    int centerWidth = centerTexture->width;
    int centerHeight = centerTexture->height;
    if (skipFill != 0) return;
    if (style == 2) {
        int rowLimit = (height - doubleC) + 50;
        if (rowLimit < 0) return;
        int innerWidth = width - doubleC;
        int columnLimit = innerWidth + 52;
        int rowOffset = 0;
        do {
            if (columnLimit > 0) {
                int columnOffset = 0;
                do {
                    int px = x + (c - 25) + columnOffset;
                    columnOffset += centerWidth;
                    textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                        centerTexture, px, y + (c - 26) + rowOffset, extra, color, x, y,
                        (x + width) - (c - 25) + 1, (y + height) - (c - 26), 0, 0);
                } while (columnOffset < innerWidth + 52);
            }
            rowOffset += centerHeight;
        } while (!(rowLimit < rowOffset));
    } else {
        int innerHeight = height - doubleC;
        if (innerHeight <= 0) return;
        int innerWidth = width - doubleC;
        int rowOffset = 0;
        do {
            if (innerWidth > 0) {
                int columnOffset = 0;
                do {
                    int px = x + c + columnOffset;
                    columnOffset += centerWidth;
                    textureDrawSuperColor__FP8_textureiiiUiiiiicc(
                        centerTexture, px, y + c + rowOffset, extra, color, x, y,
                        (x + width) - c, (y + height) - c, 0, 0);
                } while (columnOffset < innerWidth);
            }
            rowOffset += centerHeight;
        } while (rowOffset < innerHeight);
    }
}
