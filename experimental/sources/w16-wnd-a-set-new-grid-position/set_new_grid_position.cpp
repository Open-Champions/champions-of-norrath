// Observed: setNewGridPosition__Fii at 0x0027f9d8 is 412 bytes. With (cell, snap) it does nothing when cell equals the
// current word nameGridPos. Otherwise it first copies the three destination floats (nameGridDstX, nameGridDstY,
// nameGridDstW) into the three source floats (nameGridSrcX, nameGridSrcY, nameGridSrcW), then sets the destination:
//  - cell -1: X = 233 (262 when the byte `language` is nonzero); Y = (nameGridCount / nameGridW) * 20 + 122;
//    W = fontStringSizeu(mediumFont, textGetString("selectSetNameDone"), 1000000) + 10;
//  - cell -2: the same with X = 163 (192 when language is nonzero) and the string key "selectSetNameSpace";
//  - any other cell: W = 20.0; with q = cell / nameGridW and r = cell % nameGridW, Y = q * 20 + 122 and
//    X = r * 20 + 61 (+ 29 when language is nonzero).
// All results are converted from integers to float. Then, when the (re-read) nameGridPos is -3 or snap is nonzero, the
// source floats are set equal to the destination floats again. It stores cell in nameGridPos and zero in the float
// word otherHermiteTime. Nothing is stored for the early return.
// Inferred: the on-screen keyboard (name pad) cursor slides from the source rectangle to the destination one; -1 and -2
// are the two wide buttons below the letter grid, -3 the "no cell yet" state; otherHermiteTime is the slide's progress.
// Unknown: the word names beyond the image's own local symbols, and what the string keys name beyond their use here.
extern "C" {
extern int nameGridPos;
extern int nameGridW;
extern int nameGridCount;
extern float nameGridDstX;
extern float nameGridDstY;
extern float nameGridDstW;
extern float nameGridSrcX;
extern float nameGridSrcY;
extern float nameGridSrcW;
extern float otherHermiteTime;
extern char language;
extern void* mediumFont;
const unsigned short* textGetString__FPc(const char* key);
int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int limit);

void setNewGridPosition__Fii(int cell, int snap) {
    if (cell == nameGridPos) return;
    nameGridSrcX = nameGridDstX;
    nameGridSrcY = nameGridDstY;
    nameGridSrcW = nameGridDstW;
    if (cell == -1) {
        nameGridDstX = static_cast<float>((language ? 29 : 0) + 233);
        nameGridDstY = static_cast<float>(nameGridCount / nameGridW * 20 + 122);
        nameGridDstW = static_cast<float>(fontStringSizeu__FP4FontPUsi(mediumFont, textGetString__FPc("selectSetNameDone"), 1000000) + 10);
    } else if (cell == -2) {
        nameGridDstX = static_cast<float>((language ? 29 : 0) + 163);
        nameGridDstY = static_cast<float>(nameGridCount / nameGridW * 20 + 122);
        nameGridDstW = static_cast<float>(fontStringSizeu__FP4FontPUsi(mediumFont, textGetString__FPc("selectSetNameSpace"), 1000000) + 10);
    } else {
        nameGridDstW = 20.0f;
        nameGridDstX = static_cast<float>(cell % nameGridW * 20 + (language ? 29 : 0) + 61);
        nameGridDstY = static_cast<float>(cell / nameGridW * 20 + 122);
    }
    if (nameGridPos == -3 || snap != 0) {
        nameGridSrcX = nameGridDstX;
        nameGridSrcY = nameGridDstY;
        nameGridSrcW = nameGridDstW;
    }
    nameGridPos = cell;
    otherHermiteTime = 0.0f;
}
}
