// Observed shopRun__Fi at 0x001b3500, 1056 bytes (int screen). `shop` (0x007d01f8)
// holds 116-byte screen records in the same layout as `inventory`: +0, +8 and +12
// are layout words, +52 is a player index (-1 when none) and +76 a mode value.
// S = &shop[screen]; the owner is P = S+52; the global frameNm is first copied to
// shopActiveFrame (0x00484910).
//   1. Mode other than 2: displayGold__FP12playerStructP12screenStruct(player + P *
//      23984, S) then displayWeight__FP12playerStructP12screenStruct(same, S).
//   2. By the mode value (read again after those calls):
//      * 0: displayItems__FiiP12screenStructi(screen, P, S, 0), then
//        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen, P, S, 0,
//        0, 0);
//      * 1: the same two calls with the inventory record I = &inventory[screen] in
//        place of S and 1 in place of 0 for the fourth argument of both;
//      * 3: doHelpfulMessage(screen, P, S, -2, 0, 0), then mainMenu__Fii(screen, P);
//      * 2: b = inventoryPadFindButton__Fi(padAcceptButton). When the mode words of
//        both shop[0] and shop[1] are 2, or nmPlayers is 1: msgHudSetPlayerBusy__Fcc(
//        netGetLocalPlayerNm__Fi(0), 0) and shopKick__Fv(), and nothing else. When
//        b is -1, or b equals the owner of shop[screen ^ 1]: the close step below.
//        Otherwise nothing happens if b equals the owner of shop[0], shop[1] or S, or
//        b is negative; else S gets mode 3, S+64 and S+60 = 0.0f, words S+36, S+40,
//        S+24, S+28, S+20 = 0, S+52 = b, and shopUnpackToGrid__Fiii(b, screen, 1) is
//        called (the owner store comes first).
//      * any other mode: nothing.
//   Close step: S+52 = -1. Then a flag is computed over i = 0 .. nmPlayers - 1:
//   with o the owner of shop[screen ^ 1], when o is not negative and the float at
//   player + o * 23984 + 172 is above 0.0f, and i != o, the flag is set to 1 unless
//   localNmPlayersAlive is below 2 (in which case it stays 0). If the flag ends
//   nonzero, a framed message is drawn: y = (int)((float)S.word12 * 0.75f);
//   textureDrawBorder__FiiiiiUiPcii(S.word0 + 30, y + 25, 270, 51, 4, 0x80616369, 0,
//   0, 0); fontStart__FP4FontiUii(defaultFont, 5, 0x80323c3c, -1); the text of key
//   "PressEnterShop" (textGetString__FPc) through usprintf__FPUsT0ffffffff with eight
//   0.0f; fontPrintCenteredu__FiiPUsi(S.word0 + S.word8 / 2, y + 35, text, 250);
//   fontEnd__Fv().
// Unknown: what the modes 0 to 3 are called, what the float at player + 172 is, and
// what shopActiveFrame is used for.
extern "C" {
extern unsigned char player[];
extern unsigned char shop[] __attribute__((aligned(4)));
extern unsigned char inventory[] __attribute__((aligned(4)));
extern int frameNm;
extern int shopActiveFrame;
extern int nmPlayers;
extern int localNmPlayersAlive;
extern int padAcceptButton;
extern void* defaultFont;
void displayGold__FP12playerStructP12screenStruct(unsigned char*, unsigned char*);
void displayWeight__FP12playerStructP12screenStruct(unsigned char*, unsigned char*);
void displayItems__FiiP12screenStructi(int, int, unsigned char*, int);
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int, int, unsigned char*, int, int, void*);
void mainMenu__Fii(int, int);
int inventoryPadFindButton__Fi(int);
signed char netGetLocalPlayerNm__Fi(int);
void msgHudSetPlayerBusy__Fcc(signed char, signed char);
void shopKick__Fv();
void shopUnpackToGrid__Fiii(int, int, int);
void textureDrawBorder__FiiiiiUiPcii(int, int, int, int, int, unsigned, char*, int, int);
void fontStart__FP4FontiUii(void*, int, unsigned, int);
void fontEnd__Fv();
unsigned short* textGetString__FPc(char*);
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float);
void fontPrintCenteredu__FiiPUsi(int, int, unsigned short*, int);

static inline int& word(unsigned char* base, int off) { return *reinterpret_cast<int*>(base + off); }
static inline float& real(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }

void shopRun__Fi(int screen) {
    unsigned char* s = shop + screen * 116;
    shopActiveFrame = frameNm;
    int owner = word(s, 52);
    if (word(s, 76) != 2) {
        unsigned char* record = player + word(s, 52) * 23984;
        displayGold__FP12playerStructP12screenStruct(record, s);
        displayWeight__FP12playerStructP12screenStruct(record, s);
    }
    int mode = word(s, 76);
    if (mode == 1) {
        unsigned char* inv = inventory + screen * 116;
        displayItems__FiiP12screenStructi(screen, owner, inv, 1);
        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen, owner, inv, 1, 0, 0);
    } else if (mode == 0) {
        displayItems__FiiP12screenStructi(screen, owner, s, 0);
        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen, owner, s, 0, 0, 0);
    } else if (mode == 3) {
        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(screen, owner, s, -2, 0, 0);
        mainMenu__Fii(screen, owner);
    } else if (mode == 2) {
        int b = inventoryPadFindButton__Fi(padAcceptButton);
        int other = word(shop + (screen ^ 1) * 116, 52);
        if ((word(shop, 76) == 2 && word(shop + 116, 76) == 2) || nmPlayers == 1) {
            msgHudSetPlayerBusy__Fcc(netGetLocalPlayerNm__Fi(0), 0);
            shopKick__Fv();
            return;
        }
        if (b != -1 && b != other) {
            if (word(shop, 52) == b || word(shop + 116, 52) == b || b < 0 || word(s, 52) == b) return;
            word(s, 76) = 3;
            real(s, 64) = 0.0f;
            word(s, 36) = 0;
            word(s, 40) = 0;
            word(s, 24) = 0;
            word(s, 28) = 0;
            real(s, 60) = 0.0f;
            word(s, 20) = 0;
            word(s, 52) = b;
            shopUnpackToGrid__Fiii(b, screen, 1);
            return;
        }
        word(s, 52) = -1;
        int flag = 0;
        int count = nmPlayers;
        for (int i = 0; i < count; i++) {
            int o = word(shop + (screen ^ 1) * 116, 52);
            if (o >= 0 && real(player + o * 23984, 172) > 0.0f) {
                if (i != o && localNmPlayersAlive >= 2) flag = 1;
            }
        }
        if (flag == 0) return;
        int y = static_cast<int>(static_cast<float>(word(s, 12)) * 0.75f);
        int x = word(s, 0);
        textureDrawBorder__FiiiiiUiPcii(x + 30, y + 25, 270, 51, 4, 0x80616369u, 0, 0, 0);
        fontStart__FP4FontiUii(defaultFont, 5, 0x80323c3cu, -1);
        unsigned short text[128];
        usprintf__FPUsT0ffffffff(text, textGetString__FPc(const_cast<char*>("PressEnterShop")), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        fontPrintCenteredu__FiiPUsi(x + word(s, 8) / 2, y + 35, text, 250);
        fontEnd__Fv();
    }
}
}
