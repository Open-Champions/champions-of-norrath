// Observed textMessageDraw__Fv at 0x002caf30, 1112 bytes.
// When netNetworkMode (signed byte, 0x00483829) is zero, every pending raw key
// is discarded until keyboardGetNextKey returns -1 and the function returns.
// It also returns, doing nothing, while frontEndRunning (0x00483f30) is
// nonzero, DLG_Active is nonzero, or chatOn (0x004865dc) is zero. Otherwise:
//  - fontStart(smallFont, 6, 0x7f808080, -1);
//  - the edit line colour is playerGetMarkerColor(outMsg byte 2, 127). While
//    the word waiting (0x00486dac) is nonzero each of the three low bytes is
//    halved and the top byte is kept;
//  - when outMsg byte 1 (the cursor position) is not 0xff, the edit cell at
//    outMsg[byte 1 + 3] becomes -115 for the first half of each 60-frame cycle
//    of drawFrameNm (0x004844a8, signed remainder) and -116 for the second;
//  - the edit line is drawn with drawUpFrom(20, 390, text, 0, 600, colour),
//    where text is null when outMsg byte 0 is 1 and outMsg + 3 otherwise; the
//    returned value is the next y;
//  - for each of the eight 126-byte inMsg entries (0x008d8938) whose byte 2 is
//    not 0xfe, in order, unless its msgTimeout word (0x008d8d28) is above 600:
//    the alpha is 127 while the timeout is below 480, else
//    (600 - timeout) * 127 / 120; the colour is playerGetMarkerColor(byte 2,
//    alpha); the cell at entry[byte 1 + 3] blinks as for the edit line when
//    entry byte 1 is not 0xff; the entry is drawn with drawUpFrom(20, y,
//    entry + 3, player + byte2 * 23984, 600, colour) and y becomes the result.
//    An entry skipped for age leaves y unchanged. The player table is at
//    0x00408600. The original also has a path for a zero record pointer (draw
//    with null strings when entry byte 0 is 1); a table base plus an offset is
//    never zero there, so that path is kept below but cannot run;
//  - fontEnd().
extern "C" signed char netNetworkMode;
extern "C" int frontEndRunning;
extern "C" int chatOn;
extern "C" int waiting;
extern "C" int drawFrameNm;
extern "C" void* smallFont;
extern "C" unsigned char outMsg[];
extern "C" unsigned char inMsg[8][126];
extern "C" int msgTimeout[8];
extern "C" unsigned char player[];

extern "C" int keyboardGetNextKey__Fv();
extern "C" int DLG_Active__Fv();
extern "C" void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
extern "C" void fontEnd__Fv();
extern "C" unsigned playerGetMarkerColor__Fii(int player, int alpha);
extern "C" int drawUpFrom__FiiPCUcT2iUi(int x, int y, const unsigned char* text,
                                        const unsigned char* name, int width, unsigned color);

static signed char blink_cell() {
    return static_cast<signed char>((((drawFrameNm % 60) / 30) == 0 ? 1 : 0) - 116);
}

extern "C" void textMessageDraw__Fv() {
    if (netNetworkMode == 0) {
        while (keyboardGetNextKey__Fv() != -1) {
        }
        return;
    }
    if (frontEndRunning != 0) return;
    if (DLG_Active__Fv() != 0) return;
    if (chatOn == 0) return;
    fontStart__FP4FontiUii(smallFont, 6, 0x7f808080u, -1);
    int dim = waiting;
    unsigned color = playerGetMarkerColor__Fii(static_cast<signed char>(outMsg[2]), 127);
    if (dim != 0) {
        color = (((color & 0xff) >> 1) | ((color >> 24) << 24)) |
                ((((color >> 16) & 0xff) >> 1) << 16 | ((color >> 1) & 0x7f00));
    }
    if (outMsg[1] != 255) outMsg[outMsg[1] + 3] = static_cast<unsigned char>(blink_cell());
    int y;
    if (outMsg[0] == 1) {
        y = drawUpFrom__FiiPCUcT2iUi(20, 390, 0, 0, 600, color);
    } else {
        y = drawUpFrom__FiiPCUcT2iUi(20, 390, outMsg + 3, 0, 600, color);
    }
    for (int i = 0; i < 8; i++) {
        int marker = static_cast<signed char>(inMsg[i][2]);
        if (marker == -2) continue;
        unsigned char* who = player + marker * 23984;
        int age = msgTimeout[i];
        if (age >= 601) continue;
        int alpha = 127;
        if (age >= 480) alpha = ((600 - age) * 127) / 120;
        color = playerGetMarkerColor__Fii(static_cast<signed char>(inMsg[i][2]), alpha);
        if (inMsg[i][1] != 255) inMsg[i][inMsg[i][1] + 3] = static_cast<unsigned char>(blink_cell());
        if (who != 0) {
            y = drawUpFrom__FiiPCUcT2iUi(20, y, inMsg[i] + 3, who, 600, color);
        } else if (inMsg[i][0] == 1) {
            y = drawUpFrom__FiiPCUcT2iUi(20, y, 0, 0, 600, color);
        } else {
            y = drawUpFrom__FiiPCUcT2iUi(20, y, inMsg[i] + 3, who, 600, color);
        }
    }
    fontEnd__Fv();
}
