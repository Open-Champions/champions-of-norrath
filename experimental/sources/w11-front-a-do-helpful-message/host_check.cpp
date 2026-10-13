// Host check for doHelpfulMessage__FiiP12screenStructiiPQ24item11description. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin -DW11_FRONT_A_HOST host_check.cpp
// Every callee is a stub that appends to a trace. usprintf writes a two-line text so the newline
// replacement is visible; the trace names the string and the float values. The 32-bit pointer
// words in the player record and in the item slot are handles that hostWordAddress maps.
#include "do_helpful_message.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char player[4 * 23984 + 40000];
int localNmPlayersAlive, nmPlayers, drawFrameNm, drewHelpfulMessageHelpBar;
unsigned char shopPromptOK[2];
unsigned char language;
void* mediumFont;
int padAcceptButtonFontChar, padBackButtonFontChar, infoButtonFontChar, equipButtonFontChar, dropButtonFontChar;
}
alignas(8) static unsigned char target[4][64];
alignas(8) static unsigned char qrec[2][64];
static int fontToken;
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word >= 0x1000u && word < 0x1004u) return reinterpret_cast<__UINTPTR_TYPE__>(target[word - 0x1000u]);
    if (word >= 0x2000u && word < 0x2002u) return reinterpret_cast<__UINTPTR_TYPE__>(qrec[word - 0x2000u]);
    return 0;
}

static std::string trace;
static unsigned short strings[4];
static int sizeAnswer, equippedAnswer;
static std::string lastName;
static std::string text(const unsigned short* t) {
    std::string out;
    for (; *t; ++t) out += *t == 10 ? "\\n" : *t == 32 ? "_" : std::string(1, static_cast<char>(*t));
    return out;
}
static std::string joined;
extern "C" {
unsigned short* textGetString__FPc(const char* name) { lastName = name; return strings; }
void usprintf__FPUsT0ffffffff(unsigned short* to, unsigned short*, float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7) {
    trace += "fmt " + lastName;
    for (float f : {a0, a1, a2, a3, a4, a5, a6, a7}) trace += " " + std::to_string(static_cast<int>(f));
    trace += ";";
    to[0] = 'a'; to[1] = 10; to[2] = 'b'; to[3] = 0;
}
void fontStart__FP4FontiUii(void* f, int size, unsigned color, int flags) {
    trace += std::string("start") + (f == &fontToken ? "" : "?") + std::to_string(size) + "," + std::to_string(color) + "," + std::to_string(flags) + ";";
}
void fontEnd__Fv() { trace += "end;"; }
void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int a, int b, int c, int d) {
    trace += "wrap" + std::to_string(x) + "," + std::to_string(y) + "," + text(t) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + "," + std::to_string(d) + ";";
}
int fontStringSizeu__FP4FontPUsi(void* f, unsigned short*, int limit) {
    trace += std::string("size") + (f == &fontToken ? "" : "?") + std::to_string(limit) + ";";
    return sizeAnswer;
}
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned color, int b, int c, int d) {
    trace += "border" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(w) + "," + std::to_string(h) + "," + std::to_string(a) +
             "," + std::to_string(color) + "," + std::to_string(b + c + d) + ";";
}
int itemEquipped__FiPQ24item11description(int i, void* item) {
    trace += "eq" + std::to_string(i) + (item == player + 23984 * i + 328 + 4616 * 2 + 72 * 3 ? "ok" : "?") + ";";
    return equippedAnswer;
}
}

static int failures;
static void check(const char* what, const std::string& want) {
    if (trace != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, trace.c_str(), want.c_str()); ++failures; }
}
static void reset() {
    std::memset(player, 0, sizeof player);
    std::memset(target, 0, sizeof target);
    std::memset(qrec, 0, sizeof qrec);
    for (int i = 0; i < 4; ++i) { const unsigned h = 0x1000u + i; std::memcpy(player + 23984 * i + 21872, &h, 4); }
    mediumFont = &fontToken;
    localNmPlayersAlive = 1; nmPlayers = 2; drawFrameNm = 10; drewHelpfulMessageHelpBar = 99; language = 0;
    shopPromptOK[0] = shopPromptOK[1] = 1;
    padAcceptButtonFontChar = 1; padBackButtonFontChar = 2; infoButtonFontChar = 3; equipButtonFontChar = 4; dropButtonFontChar = 5;
    sizeAnswer = 100; equippedAnswer = 0;
    trace.clear();
}
static const char* const B = "border256,410,130,31,6,1082163328,0;";  // offset 0, width 100 -> 306 - 50
static const char* const S = "start7,2152089158,-1;";
int main() {
    int screen[32] = {};
    // flag only: the player slot is zero, so the function stops after step 3.
    reset(); const unsigned zero = 0; std::memcpy(player + 21872, &zero, 4);
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 1, nullptr);
    check("flag", std::string("fmt Player1Enter 1 0 0 0 0 0 0 0;") + S + "wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    if (shopPromptOK[0] != 1) { std::puts("FAIL prompt kept"); ++failures; }
    // Two local players: side 1 shifts right by 160; language set and mode 2 with tab < 3 uses the short wrap.
    reset(); std::memcpy(player + 21872, &zero, 4); localNmPlayersAlive = 2; language = 1; sizeAnswer = 40;
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(1, 0, screen, 2, 1, nullptr);
    check("flag side 1", std::string("fmt Player2Enter 1 0 0 0 0 0 0 0;") + S + "wrap480,382,a\\nb,320,-100,580,31;end;size1000000;border446,410,70,31,6,1082163328,0;");
    reset(); std::memcpy(player + 21872, &zero, 4); localNmPlayersAlive = 3; sizeAnswer = 41; 
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 1, nullptr);
    check("flag three", std::string("fmt OtherEnter 1 0 0 0 0 0 0 0;") + S + "wrap160,412,a_b,640,-100,580,21;end;size1000000;border126,410,71,31,6,1082163328,0;");
    // Prompt block, one text per row.
    struct Row { int mode, tab, screen88, screen48, rec242; const char* want; };
    const Row rows[] = {
        {-1, 0, 1, 0, 0, "fmt AcceptExit 1 2 0 0 0 0 0 0;"},
        {0, 3, 0, 0, 0, "fmt Back 2 0 0 0 0 0 0 0;"},
        {0, 4, 0, 0, 0, "fmt Back 2 0 0 0 0 0 0 0;"},
        {0, 5, 0, 0, 0, "fmt ChooseBack 1 2 0 0 0 0 0 0;"},
        {0, 5, 0, 1, 1, "fmt SelectChooseDone 127 128 129 130 1 2 0 0;"},
        {0, 5, 0, 0, 1, "fmt SpellBookChooseDone 3 1 2 0 0 0 0 0;"},
        {-2, 0, 0, 0, 0, "fmt ChooseDone 1 2 0 0 0 0 0 0;"},
        {0, 0, 0, 0, 0, "fmt BuyBack 3 2 0 0 0 0 0 0;"},
        {1, 0, 0, 0, 0, "fmt SellBack 3 2 0 0 0 0 0 0;"},
    };
    for (size_t k = 0; k < sizeof rows / sizeof rows[0]; ++k) {
        reset();
        screen[20] = rows[k].tab; screen[22] = rows[k].screen88; screen[12] = rows[k].screen48; player[242] = static_cast<unsigned char>(rows[k].rec242);
        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, rows[k].mode, 0, nullptr);
        // with screen88 zero a mode other than 0, 1, -2 would be forced to 2; the rows avoid that.
        const std::string want = std::string(S) + rows[k].want + "wrap320,412,a_b,640,-100,580,21;end;" + "size1000000;" + B;
        check(("row " + std::to_string(k)).c_str(), want);
    }
    // Mode 2: item rows. tab 2, desc halfword 3.
    unsigned char desc[64] = {};
    const short k3 = 3; std::memcpy(desc + 34, &k3, 2);
    screen[20] = 2; screen[22] = 0; screen[12] = 0;
    reset(); doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 2, 0, nullptr);
    check("mode 2 no desc", std::string(S) + "fmt Back 2 0 0 0 0 0 0 0;wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    reset(); { const unsigned h = 0x2000u; std::memcpy(player + 328 + 4616 * 2 + 72 * 3, &h, 4); equippedAnswer = 1; }
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 2, 0, desc);
    check("unequip", std::string(S) + "eq0ok;fmt UnequipDropBack 4 5 2 0 0 0 0 0;wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    for (int bit = 0; bit < 2; ++bit) {
        reset(); { const unsigned h = 0x2000u; std::memcpy(player + 328 + 4616 * 2 + 72 * 3, &h, 4);
            const unsigned long long v = bit ? (1ull << 41) : (1ull << 40); std::memcpy(qrec[0] + 40, &v, 8); }
        doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 2, 0, desc);
        check(bit ? "drop" : "equip", std::string(S) + "eq0ok;" + (bit ? "fmt DropBack 5 2 0 0 0 0 0 0;" : "fmt EquipDropBack 4 5 2 0 0 0 0 0;") +
              "wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    }
    // Mode 3: no desc hides; desc with screen[22] set combines (the unset-buffer cases are not run on the host).
    screen[20] = 0;
    reset(); screen[22] = 1; doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 3, 0, nullptr);
    check("mode 3 no desc", std::string(S) + "end;");
    reset(); screen[22] = 1; doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 3, 0, desc);
    check("combine", std::string(S) + "fmt CombineWith 4 2 0 0 0 0 0 0;wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    screen[22] = 0;
    // Guards: prompt flag off, player out of range, dead flag bit 8, null record.
    reset(); shopPromptOK[0] = 0; doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 0, nullptr);
    check("prompt off", ""); if (shopPromptOK[0] != 1) { std::puts("FAIL prompt set"); ++failures; }
    reset(); doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 2, screen, 0, 0, nullptr); check("out of range", std::string("size1000000;") + B);
    reset(); { const unsigned f = 8; std::memcpy(target[0] + 12, &f, 4); } doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 0, nullptr);
    check("bit 8", std::string("size1000000;") + B);
    reset(); { const unsigned f = 7; std::memcpy(target[0] + 12, &f, 4); } doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 0, nullptr);
    check("other flags", std::string(S) + "fmt BuyBack 3 2 0 0 0 0 0 0;wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    reset(); std::memcpy(player + 21872, &zero, 4); shopPromptOK[1] = 0; doHelpfulMessage__FiiP12screenStructiiPQ24item11description(1, 0, screen, 0, 0, nullptr);
    check("null record", ""); if (shopPromptOK[1] != 0) { std::puts("FAIL null record sets prompt"); ++failures; }
    // Mode forced to 2 when screen[22] is zero; kept when set.
    reset(); screen[20] = 2; doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 9, 0, nullptr);
    check("forced to 2", std::string(S) + "fmt Back 2 0 0 0 0 0 0 0;wrap320,412,a_b,640,-100,580,21;end;size1000000;" + B);
    // The help-bar timer.
    reset(); drewHelpfulMessageHelpBar = 9; shopPromptOK[0] = 0; drawFrameNm = 10;
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 0, nullptr);
    if (drewHelpfulMessageHelpBar != 0) { std::puts("FAIL timer clear"); ++failures; }
    reset(); drewHelpfulMessageHelpBar = 10; drawFrameNm = 10;
    doHelpfulMessage__FiiP12screenStructiiPQ24item11description(0, 0, screen, 0, 0, nullptr);
    if (drewHelpfulMessageHelpBar != 10) { std::puts("FAIL timer set"); ++failures; }
    if (failures == 0) std::puts("do_helpful_message host check passed");
    return failures != 0;
}
