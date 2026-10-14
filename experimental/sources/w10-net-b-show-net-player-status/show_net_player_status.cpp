// Observed showNetPlayerStatus__Fv at 0x001343b0, 2016 bytes in the pinned
// SLUS_205.65 image; no arguments, no result. It draws the launch lobby status
// panel. Everything below is what the image does, in call order.
// Addresses used: launchPeers (4 records of 252 bytes at 0x333278),
// netGameLaunchInfo (0x333130; signed bytes at +4 = entry count, +5 and +6 are
// compared with each other and with the loop index), hudFooterUni (0x004283e0,
// 256 bytes), smallFont, padAcceptButtonFontChar, padBackButtonFontChar,
// MC_Debug, hostStartedWithoutMe, netLaunchExpirationCount, netLaunchTimer.
//  1. If hostStartedWithoutMe is nonzero, or netLaunchExpirationCount is above
//     2700: fontStart(smallFont, 6, 0x80425d5f, -1), print one wrapped,
//     centered wide string (x 320, y 200, 400, -100, 580, 21) taken from the text
//     key "netLaunchStartedWithoutMe" (first case) or "netLaunchNoHost"
//     (second), fontEnd, and go to step 3.
//  2. Otherwise for each entry i below the entry count n (n read once; the
//     loop test rereads it), with x = 339 - 74 * n + 148 * i:
//       - title: key "netLaunchHost" for i = 0, else the key "netLaunchOther"
//         formatted with (float)(i + 1); drawn centered at (x + 54, 70);
//       - a bordered frame textureDrawBorder(x, 100, 110, 108, 6, 0x80808080,
//         0, 1, 0);
//       - when the record byte +92 is nonzero or i equals the local index
//         netGameLaunchInfo[6]: the portrait from "hud.lmp" named by the
//         string table entry for the record byte +136, drawn at (x + 4, 104);
//         the record name at +104 shortened to 11 characters (with "..." when
//         shortened) at y 211; key "netLaunchLevel" at 245 and the record word
//         at +152 at 262; key "netLaunchGold" at 279 and the word at +148 at
//         296 (all centered at x + 54);
//       - the status line at y 330, centered at x + 54: for the local entry the
//         key "netLaunchReady" in color 0x80204020; for others, by record byte
//         +92: 0 "netLaunchContacting" (0x80202040); 1..6 "netLaunchTransfer"
//         formatted with 100 * word(+244) / word(+248) (0x80004040); 7 and 8
//         "netLaunchReady" (0x80204020) and, when netGameLaunchInfo[5] equals
//         [6], a counter is incremented; 9 "netLaunchError" (0x800000c4); other
//         values print the previous contents of the line buffer;
//       - with MC_Debug nonzero, for i other than the local index, when the
//         word at launchPeers + 224 + 252 * local + 4 * i or the one with the
//         indexes swapped is nonzero: color 0x80000060 and "dev fail=%d" with
//         the first word at y 347.
//  3. Footer: when netLaunchTimer is nonzero the key "netLaunchStarting"
//     formatted with padBackButtonFontChar; else when the counter is nonzero
//     "netLaunchHostPrompt" formatted with padAcceptButtonFontChar and
//     padBackButtonFontChar; else "MemcardBack" formatted with
//     padBackButtonFontChar. The result goes to hudFooterUni and hudShowMsg is
//     called with the key "netLaunchTitle", null, the footer, 128,
//     608 - 74 * (4 - n), 284.
// Inferred: this is the lobby panel of the launch flow; the counter counts
// ready peers seen by the host. Unknown: what the record fields (+92 state, +136
// class index, +148/+152 values) mean beyond the use here, and the line buffer
// content when the first entry prints a state outside 0..9 (read uninitialised
// in the image; this port reads uninitialised storage the same way).
struct Font;
struct Texture;
extern "C" {
extern unsigned char netGameLaunchInfo[];
extern unsigned char launchPeers[];
extern unsigned short hudFooterUni[];
extern Font* smallFont;
extern int padAcceptButtonFontChar;
extern int padBackButtonFontChar;
extern signed char MC_Debug;
extern signed char hostStartedWithoutMe;
extern int netLaunchExpirationCount;
extern int netLaunchTimer;
unsigned short* textGetString__FPc(char* key);
void fontStart__FP4FontiUii(Font* font, int size, unsigned color, int flags);
void fontEnd__Fv();
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit);
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit);
void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* text, int width, int a, int b, int c);
void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int e, unsigned color, char* f, int g, int h);
void textureDrawColor__FP8_textureiiiUi(Texture* texture, int x, int y, int z, unsigned color);
Texture* lumpFindResource__FPcT0(char* group, char* name);
void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h);
void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
void hudShowMsg__FPUsN20iii(unsigned short* title, unsigned short* second, unsigned short* footer, int a, int x, int y);
int sprintf(char* destination, const char* format, ...);
unsigned long strlen(const char* text);
char* strcat(char* destination, const char* source);

namespace {
char* const kPortraitNames[6] = {const_cast<char*>("b_bm.tex"), const_cast<char*>("b_bf.tex"), const_cast<char*>("b_wem.tex"),
                                 const_cast<char*>("b_wef.tex"), const_cast<char*>("b_hem.tex"), const_cast<char*>("b_hef.tex")};
inline int local_index() { return static_cast<signed char>(netGameLaunchInfo[6]); }
inline unsigned short* key(const char* name) { return textGetString__FPc(const_cast<char*>(name)); }
inline void zero_format(unsigned short* out, unsigned short* format, float first, float second) {
    usprintf__FPUsT0ffffffff(out, format, first, second, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
}

void showNetPlayerStatus__Fv() {
    int ready_count = 0;
    char line[128];
    unsigned short wide[128];
    if (hostStartedWithoutMe != 0 || netLaunchExpirationCount > 2700) {
        const char* name = hostStartedWithoutMe != 0 ? "netLaunchStartedWithoutMe" : "netLaunchNoHost";
        fontStart__FP4FontiUii(smallFont, 6, 0x80425d5f, -1);
        fontPrintWrapCenteredu__FiiPUsiiii(320, 200, key(name), 400, -100, 580, 21);
        fontEnd__Fv();
    } else {
        int count = static_cast<signed char>(netGameLaunchInfo[4]);
        int base = 339 - 74 * count;
        int i = 0;
        if (count > 0) {
            do {
                unsigned char* peer = launchPeers + 252 * i;
                int x = base + 148 * i;
                int centre = x + 54;
                fontStart__FP4FontiUii(smallFont, 6, 0x80425d5f, -1);
                if (i == 0) {
                    fontPrintCenteredu__FiiPUsi(centre, 70, key("netLaunchHost"), 1000000);
                } else {
                    unsigned short* format = key("netLaunchOther");
                    zero_format(wide, format, static_cast<float>(i + 1), 0.0f);
                    fontPrintCenteredu__FiiPUsi(centre, 70, wide, 1000000);
                }
                fontEnd__Fv();
                textureDrawBorder__FiiiiiUiPcii(x, 100, 110, 108, 6, 0x80808080, 0, 1, 0);
                if (static_cast<signed char>(peer[92]) != 0 || local_index() == i) {
                    Texture* portrait = lumpFindResource__FPcT0(const_cast<char*>("hud.lmp"),
                                                                kPortraitNames[static_cast<signed char>(peer[136])]);
                    textureDrawColor__FP8_textureiiiUi(portrait, x + 4, 104, 6, 0x80808080);
                    fontStart__FP4FontiUii(smallFont, 6, 0x80808080, -1);
                    sprintf(line, reinterpret_cast<const char*>(peer + 104));
                    while (strlen(line) >= 12) line[strlen(line) - 1] = 0;
                    unsigned long shortened = strlen(line);
                    if (shortened != strlen(reinterpret_cast<const char*>(peer + 104))) strcat(line, "...");
                    fontColor__FUiii(0x80425d5f, -1, 0);
                    fontPrintCentered__FiiPci(centre, 211, line, 10000);
                    fontPrintCenteredu__FiiPUsi(centre, 245, key("netLaunchLevel"), 1000000);
                    fontColor__FUiii(0x80404040, -1, 0);
                    sprintf(line, "%d", *reinterpret_cast<int*>(peer + 152));
                    fontPrintCentered__FiiPci(centre, 262, line, 10000);
                    fontColor__FUiii(0x80425d5f, -1, 0);
                    fontPrintCenteredu__FiiPUsi(centre, 279, key("netLaunchGold"), 1000000);
                    fontColor__FUiii(0x80404040, -1, 0);
                    sprintf(line, "%d", *reinterpret_cast<int*>(peer + 148));
                    fontPrintCentered__FiiPci(centre, 296, line, 10000);
                    fontEnd__Fv();
                }
                fontStart__FP4FontiUii(smallFont, 6, 0x80808080, -1);
                if (local_index() == i) {
                    ustrcpy__FPUsT0(wide, key("netLaunchReady"));
                    fontColor__FUiii(0x80204020, -1, 0);
                } else {
                    fontColor__FUiii(0x800000ff, -1, 0);
                    int state = static_cast<signed char>(peer[92]);
                    switch (state) {
                    case 0:
                        ustrcpy__FPUsT0(wide, key("netLaunchContacting"));
                        fontColor__FUiii(0x80202040, -1, 0);
                        break;
                    case 1: case 2: case 3: case 4: case 5: case 6: {
                        unsigned short* format = key("netLaunchTransfer");
                        float done = static_cast<float>(*reinterpret_cast<int*>(peer + 244));
                        float total = static_cast<float>(*reinterpret_cast<int*>(peer + 248));
                        zero_format(wide, format, done / total * 100.0f, 0.0f);
                        fontColor__FUiii(0x80004040, -1, 0);
                        break;
                    }
                    case 7: case 8:
                        ustrcpy__FPUsT0(wide, key("netLaunchReady"));
                        fontColor__FUiii(0x80204020, -1, 0);
                        if (static_cast<signed char>(netGameLaunchInfo[5]) == local_index()) ++ready_count;
                        break;
                    case 9:
                        ustrcpy__FPUsT0(wide, key("netLaunchError"));
                        fontColor__FUiii(0x800000c4, -1, 0);
                        break;
                    default:
                        break;
                    }
                }
                fontPrintCenteredu__FiiPUsi(centre, 330, wide, 1000000);
                if (MC_Debug != 0 && local_index() != i) {
                    int local = local_index();
                    int* table = reinterpret_cast<int*>(launchPeers + 224);
                    if (table[63 * local + i] != 0 || table[63 * i + local] != 0) {
                        fontColor__FUiii(0x80000060, -1, 0);
                        sprintf(line, "dev fail=%d\n", table[63 * local_index() + i]);
                        fontPrintCentered__FiiPci(centre, 347, line, 10000);
                    }
                }
                fontEnd__Fv();
                ++i;
            } while (i < static_cast<signed char>(netGameLaunchInfo[4]));
        }
    }
    unsigned short* format;
    if (netLaunchTimer != 0) {
        format = key("netLaunchStarting");
        zero_format(hudFooterUni, format, static_cast<float>(padBackButtonFontChar), 0.0f);
    } else if (ready_count != 0) {
        format = key("netLaunchHostPrompt");
        zero_format(hudFooterUni, format, static_cast<float>(padAcceptButtonFontChar),
                    static_cast<float>(padBackButtonFontChar));
    } else {
        format = key("MemcardBack");
        zero_format(hudFooterUni, format, static_cast<float>(padBackButtonFontChar), 0.0f);
    }
    hudShowMsg__FPUsN20iii(key("netLaunchTitle"), 0, hudFooterUni, 128,
                           608 - 74 * (4 - static_cast<signed char>(netGameLaunchInfo[4])), 284);
}
}
