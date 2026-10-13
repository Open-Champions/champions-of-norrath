// Observed doNetCamToggle__Fv at 0x001c7330, 472 bytes, 304-byte frame with a 256-byte text buffer.
// Nothing happens when the byte netNetworkMode is zero. Otherwise the first index below nmPlayers
// for which netPlayerIsLocal is nonzero is found (nmPlayers when none is). The rest runs only when
// the float at player-record byte 172 equals 0.0 and, when the word at record byte 21872 is nonzero,
// the 16-bit gameDeathTimerSingle[index] is zero; and nmPlayersAlive is nonzero. Then, in order:
// ChangeNetCam(gameCam, 0); return if DLG_Active() or the byte cutMode is nonzero; when
// netLocalPad[index] (88-byte records, word at +12) has a bit of padAcceptButton set,
// ChangeNetCam(gameCam, 1). Then the current camera target is read with GetTargetNum, its marker
// colour is playerGetMarkerColor(target, 128), fontStart(defaultFont, 6, colour, -1), the record
// name at the start of the target's player record (23984-byte stride) is printed centred at (320,
// 352) with limit 10000, and when nmPlayersAlive is at least 2 the text for the key "netCamToggle"
// is formatted with usprintf using (float)padAcceptButtonFontChar and seven 0.0 values, after
// fontColor(0x804d4d4d, -1, 0), and printed centred at (320, 372) with limit 1000000. fontEnd closes.
// Inferred: the float at byte 172 is the health test the other player code uses; the first line is
// a player name. Unknown: field names, what the spectated-camera feature is called in the original.
extern "C" {
extern signed char netNetworkMode;
extern int nmPlayers;
extern int nmPlayersAlive;
extern unsigned char player[];
extern short gameDeathTimerSingle[];
extern signed char cutMode;
extern unsigned padAcceptButton;
extern int padAcceptButtonFontChar;
extern unsigned char netLocalPad[];
extern void* gameCam;
extern void* defaultFont;
int netPlayerIsLocal__Fi(int player);
void ChangeNetCam__6Camerai(void* camera, int mode);
int DLG_Active__Fv();
int GetTargetNum__6Camera(void* camera);
unsigned playerGetMarkerColor__Fii(int player, int alpha);
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
void fontPrintCentered__FiiPci(int x, int y, const char* text, int limit);
void fontColor__FUiii(unsigned color, int a, int b);
unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit);
void fontEnd__Fv();
}

namespace {
unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
}

extern "C" void doNetCamToggle__Fv() {
    if (netNetworkMode == 0) return;
    int index = 0;
    while (index < nmPlayers && netPlayerIsLocal__Fi(index) == 0) ++index;
    const unsigned char* record = player + static_cast<unsigned>(index) * 23984u;
    union {
        unsigned bits;
        float value;
    } health;
    health.bits = load_u32(record + 172);
    if (health.value != 0.0f) return;
    if (load_u32(record + 21872) != 0 && gameDeathTimerSingle[index] != 0) return;
    if (nmPlayersAlive == 0) return;
    ChangeNetCam__6Camerai(gameCam, 0);
    if (DLG_Active__Fv() != 0) return;
    if (cutMode != 0) return;
    if ((load_u32(netLocalPad + static_cast<unsigned>(index) * 88u + 12) & padAcceptButton) != 0)
        ChangeNetCam__6Camerai(gameCam, 1);
    unsigned color = playerGetMarkerColor__Fii(GetTargetNum__6Camera(gameCam), 128);
    fontStart__FP4FontiUii(defaultFont, 6, color, -1);
    fontPrintCentered__FiiPci(320, 352,
        reinterpret_cast<const char*>(player + static_cast<unsigned>(GetTargetNum__6Camera(gameCam)) * 23984u), 10000);
    if (nmPlayersAlive >= 2) {
        unsigned short buffer[128];
        fontColor__FUiii(0x804d4d4du, -1, 0);
        unsigned short* format = textGetString__FPc("netCamToggle");
        usprintf__FPUsT0ffffffff(buffer, format, static_cast<float>(padAcceptButtonFontChar), 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintCenteredu__FiiPUsi(320, 372, buffer, 1000000);
    }
    fontEnd__Fv();
}
