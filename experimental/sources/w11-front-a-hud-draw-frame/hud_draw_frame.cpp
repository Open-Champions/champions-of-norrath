// Observed: HUDDrawFrame__Fv at 0x001c3ec0 is 512 bytes, no arguments, no result. In order:
//  1. drawGold__Fv(), then runHUD__Fi(i) for i from 0 while i < nmPlayers (read again after each
//     call), then helpfulMessagesDraw__Fv(), drawBust__Fv(), drawCompass__Fv() and
//     doNetCamToggle__Fv().
//  2. When the word coreLoading and the signed byte cutMode are both zero:
//     hudRunNoControllers__Fi(nmPlayers).
//  3. Only when the signed byte netNetworkMode and the int netTrouble are both nonzero:
//     a. When bit 0x20 of drawFrameNm is set: t = lumpFindResource__FPcT0("hud.lmp",
//        "noconnect.tex"), then textureDrawColor__FP8_textureiiiUi(t, 320 - w / 2, 224 - h / 2, 7,
//        0x80808080), with w and h the signed halfwords at t + 0 and t + 2 (halved toward zero).
//     b. netTrouble (read again) above 1200: ustrcpy__FPUsT0(infoMessage,
//        textGetString__FPc("netMessageLostConnection")); infoMessageTimer = 300;
//        infoMessageAlways = 1.
//     c. A nonzero byte netKickedOut: the same with "netMessageKickedOut".
//     d. netPlayerIsConnected__Fi(0) zero: the same with "netMessageHostGone".
//     e. netTrouble (read again) above 1800, or above 600 with netKickedOut nonzero or
//        netPlayerIsConnected__Fi(0) zero (the second test is skipped when the byte is nonzero):
//        gameNetKick__Fi(0) and gameBailFromGameLoop = 1.
// Inferred: this is the draw task of the HUD; step 3 is a network-trouble overlay that shows a
// message after about 1200 ticks of trouble and leaves the game after about 1800 (or 600 when the
// connection is already known to be gone).
// Unknown: the unit of netTrouble, and the exact use of drawFrameNm bit 0x20.
extern "C" {
void drawGold__Fv();
void runHUD__Fi(int player_index);
void helpfulMessagesDraw__Fv();
void drawBust__Fv();
void drawCompass__Fv();
void doNetCamToggle__Fv();
void hudRunNoControllers__Fi(int count);
void* lumpFindResource__FPcT0(char* lump, const char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
unsigned short* textGetString__FPc(const char* name);
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from);
int netPlayerIsConnected__Fi(int index);
void gameNetKick__Fi(int index);
extern int nmPlayers;
extern int coreLoading;
extern signed char cutMode;
extern signed char netNetworkMode;
extern int netTrouble;
extern int drawFrameNm;
extern signed char netKickedOut;
extern unsigned short infoMessage[];
extern unsigned short infoMessageTimer;
extern unsigned char infoMessageAlways;
extern int gameBailFromGameLoop;
}

static void showMessage(const char* name) {
    ustrcpy__FPUsT0(infoMessage, textGetString__FPc(name));
    infoMessageTimer = 300;
    infoMessageAlways = 1;
}

extern "C" void HUDDrawFrame__Fv() {
    drawGold__Fv();
    for (int i = 0; i < nmPlayers;) runHUD__Fi(i++);
    helpfulMessagesDraw__Fv();
    drawBust__Fv();
    drawCompass__Fv();
    doNetCamToggle__Fv();
    if (coreLoading == 0 && cutMode == 0) hudRunNoControllers__Fi(nmPlayers);
    if (netNetworkMode == 0 || netTrouble == 0) return;
    if ((drawFrameNm & 0x20) != 0) {
        unsigned char* t = static_cast<unsigned char*>(lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"), "noconnect.tex"));
        const int w = *reinterpret_cast<short*>(t);
        const int h = *reinterpret_cast<short*>(t + 2);
        textureDrawColor__FP8_textureiiiUi(t, 320 - w / 2, 224 - h / 2, 7, 0x80808080u);
    }
    if (netTrouble > 1200) showMessage("netMessageLostConnection");
    if (netKickedOut != 0) showMessage("netMessageKickedOut");
    if (netPlayerIsConnected__Fi(0) == 0) showMessage("netMessageHostGone");
    if (netTrouble > 1800 || (netTrouble > 600 && (netKickedOut != 0 || netPlayerIsConnected__Fi(0) == 0))) {
        gameNetKick__Fi(0);
        gameBailFromGameLoop = 1;
    }
}
