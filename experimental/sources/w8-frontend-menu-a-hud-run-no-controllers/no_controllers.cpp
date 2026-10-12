// Observed hudRunNoControllers__Fi at 0x001c3838, 456 bytes, returns nothing.
// Reads: powerOffMode (signed byte), playerSelectCount (signed byte),
// netNetworkMode (signed byte), frontEndRunning (word), mediumFont (pointer),
// the string at levelName+81, and the word at offset 4 of each 88-byte
// element of the array `pad` (0x0034beb8, 352 bytes, so four elements).
// Writes hudNoController (byte).
//
// Observed flow: nothing happens while powerOffMode is non-zero. The count is
// the argument, replaced by playerSelectCount when the string at levelName+81
// equals "kelsel2". The chosen index is -1 unless an element's word at
// offset 4 is zero: in a network game only element 0 is looked at; otherwise
// elements below the count are scanned and the last zero one is chosen.
// With no chosen index hudNoController is cleared. Otherwise a message is
// built with usprintf from the text for "controllerOutMsg" and the address of
// the chosen element converted to float (as the image does), drawn in a font
// (the "gamefontsmall.fnt" resource of "frontend.lmp" while frontEndRunning,
// else mediumFont) at 320,200 wrapped to 400 wide with the 7 and 0x801e6464
// style arguments, and a border is drawn around it, 21 below the text
// height returned by the wrapped print. Finally hudNoController is set.
// Inferred: the buffer holds 400 UTF-16 units, from the 816-byte frame.
// Unknown: what offset 4 of a pad element means; the border parameters.
extern "C" {
struct Font;
struct norrath_pad_record {
    int word0;
    int word4;
    unsigned char rest[80];
};
extern signed char powerOffMode;
extern signed char playerSelectCount;
extern signed char netNetworkMode;
extern int frontEndRunning;
extern Font* mediumFont;
extern char levelName[];
extern norrath_pad_record pad[];
extern unsigned char hudNoController;

int strcmp(const char* a, const char* b);
unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
Font* lumpFindResource__FPcT0(const char* lump, const char* name);
void fontStart__FP4FontiUii(Font* font, int mode, unsigned color, int last);
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void fontEnd__Fv();
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, const char* lump,
                                     int b, int c);

void hudRunNoControllers__Fi(int count) {
    if (powerOffMode) return;
    if (strcmp(levelName + 81, "kelsel2") == 0) count = playerSelectCount;
    int chosen = -1;
    if (netNetworkMode) {
        if (pad[0].word4 == 0) chosen = 0;
    } else {
        for (int i = 0; i < count; ++i) {
            if (pad[i].word4 == 0) chosen = i;
        }
    }
    if (chosen < 0) {
        hudNoController = 0;
        return;
    }
    unsigned short text[400];
    usprintf__FPUsT0ffffffff(text, textGetString__FPc("controllerOutMsg"),
                             static_cast<float>(reinterpret_cast<__INTPTR_TYPE__>(&pad[chosen])), 0.0f,
                             0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Font* font = frontEndRunning ? lumpFindResource__FPcT0("frontend.lmp", "gamefontsmall.fnt")
                                 : mediumFont;
    fontStart__FP4FontiUii(font, 7, 0x801e6464u, -1);
    int height = fontPrintWrapCenteredu__FiiPUsiiii(320, 200, text, 400, -100, 580, 21);
    fontEnd__Fv();
    textureDrawBorder__FiiiiiUiPcii(112, 192, 416, height + 21, 7, 0x80808080u,
                                    frontEndRunning ? "frontend.lmp" : 0, 0, 0);
    hudNoController = 1;
}
}
