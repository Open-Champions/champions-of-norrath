// Observed: drawProgress__Fv at 0x00255ed0 is 436 bytes with a 1056-byte frame
// (a 1024-byte local buffer). In order it:
//  1. draws a filled rectangle with rectangleDraw__FiiiiiUi(119, 261,
//     (int)progress + 119, 271, 6, color) where color = (a << 24) | 0x276b7f
//     and a = (int)((float)flareAlpha * 0.0078125f * 128.0f);
//  2. draws rectangleDraw__FiiiiiUi(119, 259, 521, 273, 6, color) with
//     color = (a << 24) | 0x141414 and a = (int)((float)flareAlpha * 0.0078125f
//     * 64.0f);
//  3. picks a text key: "memcardLoadGame" if asyncMode is 3, otherwise
//     "Memcard2OverwriteMsg" if browserOverwrite is nonzero, otherwise
//     "Memcard3SaveMsg";
//  4. formats it into the local buffer with usprintf__FPUsT0ffffffff(buffer,
//     text, 137.0f, (float)slot, 137.0f, 0, 0, 0, 0, 0), where slot is the
//     result of getSlotName__FiiPc(-1, -1, 0) (called after the text lookup);
//  5. calls hudShowMsg__FPUsN20iii(0, buffer, 0, flareAlpha, 0, 0).
// progress is read as a float and flareAlpha as an int, from the load and
// convert instructions; the three branches contain repeated copies of steps 4
// and 5's setup in the image, which the source merges.
// Inferred: this is a card-progress overlay (bar plus backdrop plus message);
// the pixel positions are coordinates of a 640-wide screen by inspection only.
// Unknown: the unit of progress (the bar spans 0 to at most 402 pixels for a
// 119..521 frame), the meaning of 137.0f, the meaning of the rectangle's fifth
// argument 6, and the owner of progress, flareAlpha, asyncMode and
// browserOverwrite (the image holds several objects named flareAlpha; this
// routine reads the one at 0x00486cb0).

extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int mode,
                                        unsigned color);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" int getSlotName__FiiPc(int a, int b, char* c);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* format,
                                         float a, float b, float c, float d, float e,
                                         float f, float g, float h);
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* body,
                                       unsigned short* footer, int a, int b, int c);
extern "C" float progress;
extern "C" int flareAlpha;
extern "C" int asyncMode;
extern "C" int browserOverwrite;

extern "C" void drawProgress__Fv() {
    unsigned short buffer[512];
    int a = static_cast<int>(static_cast<float>(flareAlpha) * 0.0078125f * 128.0f);
    rectangleDraw__FiiiiiUi(119, 261, static_cast<int>(progress) + 119, 271, 6,
                            (static_cast<unsigned>(a) << 24) | 0x276b7fu);
    a = static_cast<int>(static_cast<float>(flareAlpha) * 0.0078125f * 64.0f);
    rectangleDraw__FiiiiiUi(119, 259, 521, 273, 6, (static_cast<unsigned>(a) << 24) | 0x141414u);
    const char* key = "Memcard3SaveMsg";
    if (asyncMode == 3) key = "memcardLoadGame";
    else if (browserOverwrite != 0) key = "Memcard2OverwriteMsg";
    unsigned short* text = textGetString__FPc(const_cast<char*>(key));
    int slot = getSlotName__FiiPc(-1, -1, 0);
    usprintf__FPUsT0ffffffff(buffer, text, 137.0f, static_cast<float>(slot), 137.0f, 0.0f, 0.0f,
                             0.0f, 0.0f, 0.0f);
    hudShowMsg__FPUsN20iii(0, buffer, 0, flareAlpha, 0, 0);
}
