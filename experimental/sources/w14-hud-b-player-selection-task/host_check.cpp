// Host check for playerSelectionTask__Fv. Not an EE plan input.
// Build as x86_64 with statics below 4 GiB (the target and object tables hold 32-bit pointer words):
//   c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
// Recording stubs stand in for every callee and the real sprintf/sinf are used. Each scenario sets the unit state, runs one
// frame and checks the state changes and the call trace for the branch under test.
#include "player_selection_task.cpp"

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int curPlayer, charDelta, selectMode, newMode, lockAlpha, requestToExit, statAllocation, msgAlpha, msgAlphaDelta;
unsigned char curStat, statAllocWarn, startExitFade;
signed char curSlider, lastSlider, maxSlider;
float hermiteTime, delta, hermDeltaScale, headerAlpha, bodyAlpha, otherHermiteTime, throb;
float flareAlpha __asm__("norrath_player_select_view_486d3c");
float flareDelta __asm__("norrath_player_select_view_486d44");
signed char firstTime __asm__("norrath_player_select_view_486d88");
int throbR, throbG, throbB, throbA;
char* npcLumpName;
signed char playerSelectCount, playerSelectStartPlayer;
UString* playerSelectExitMessage;
int frameNm, coreLoading;
void* gameCam;
void* smallFont;
signed char language;
unsigned padAcceptButton, padBackButton;
int padAcceptButtonFontChar, padBackButtonFontChar;
alignas(8) unsigned char pad[4 * 88];
alignas(8) unsigned char targets[10 * 636];
alignas(8) unsigned char playerSelectInfo[4 * 41 + 16];
alignas(8) unsigned char targetObjs[40];
alignas(8) unsigned char abilityTable[80];
alignas(8) unsigned char hairStyleSetting[16], hairColorSetting[16], beardSetting[16], skinSetting[16];
alignas(8) unsigned char playerSelectSoundLump[16];
unsigned statTable[4];
UString hudFooterUni[128];
}
alignas(8) static unsigned char objects[10][512];
static std::string trace;
static int rGetMode, rTex, rYesNo, rName, rHair, rSlider, rChange, rFade;
static bool lumpFound;
static int sliderCalls;

static std::string num(long long v) { return std::to_string(v); }
static std::string fnum(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", static_cast<double>(v)); return b; }
static std::string text(const UString* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static void put(UString* d, const std::string& s) { size_t i = 0; for (; i < s.size(); ++i) d[i] = static_cast<UString>(s[i]); d[i] = 0; }
static UString strings[64][64];
static int stringCount;

extern "C" {
int formatText(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int n = std::vsnprintf(buffer, 128, format, args);
    va_end(args);
    return n;
}
int GetMode__6Camera(void*) { trace += "getmode;"; return rGetMode; }
void moveCamera__Fv() { trace += "moveCam;"; }
void npcLookAtCamera__Fv() { trace += "lookCam;"; }
int texIsLoading__Fv() { trace += "texLoading;"; return rTex; }
int hudFadeScreen__Fi(int amount) { trace += "fade(" + num(amount) + ");"; return rFade; }
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    trace += std::string("lump(") + lump + "," + name + ");";
    return lumpFound ? reinterpret_cast<void*>(&lumpFound) : nullptr;
}
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void*, float blend, int mode) {
    trace += "anim(" + num((static_cast<unsigned char*>(state) - 288 - objects[0]) / 512) + "," + fnum(blend) + "," + num(mode) + ");";
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float v) { trace += "seq(" + num(a != nullptr) + "," + num(o != nullptr) + "," + fnum(v) + ");"; }
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* s, void* p, float a, float b, float c) {
    trace += "sound(" + num(s != nullptr) + "," + num(p != nullptr) + "," + fnum(a) + "," + fnum(b) + "," + fnum(c) + ");";
}
UString* textGetString__FPc(const char* key) {
    UString* s = strings[stringCount++ % 64];
    put(s, std::string("<") + key + ">");
    return s;
}
void usprintf__FPUsT0ffffffff(UString* d, UString* f, float a, float b, float c, float e, float, float, float, float) {
    trace += "us(" + text(f) + "," + fnum(a) + "," + fnum(b) + "," + fnum(c) + "," + fnum(e) + ");";
    put(d, text(f) + "|");
}
void usprintf__FPUsPcffffffff(UString* d, const char* f, float a, float, float, float, float, float, float, float) {
    trace += std::string("usc(") + f + "," + fnum(a) + ");";
    put(d, std::string(f) + "|" + fnum(a));
}
void ustrcpy__FPUsT0(UString* d, UString* s) { trace += "ustrcpy;"; put(d, text(s)); }
void hudShowMsg__FPUsN20iii(UString* a, UString* b, UString* c, int alpha, int x, int y) {
    trace += "msg(" + text(a) + "," + text(b) + "," + (c == hudFooterUni ? "FOOT" : "?") + "," + num(alpha) + "," + num(x) + "," + num(y) + ");";
}
int hudRunYesNo__FPUsiiii(UString*, int a, int b, int c, int d) {
    trace += "yesno(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
    return rYesNo;
}
void allDone__Fi(int v) { trace += "allDone(" + num(v) + ");"; }
void initPosition__Fii(int a, int b) { trace += "initPos(" + num(a) + "," + num(b) + ");"; }
void engineSetBlur__Fi(int v) { trace += "blur(" + num(v) + ");"; }
void resetName__Fv() { trace += "resetName;"; }
void switchToNamePad__Fv() { trace += "toNamePad;"; }
int doNamePad__Fv() { trace += "doNamePad;"; return rName; }
void drawNamePad__Fv() { trace += "drawNamePad;"; }
void initPreSelect__Fv() { trace += "preSelect;"; }
void initPostSelect__Fv() { trace += "postSelect;"; }
void hudResetFadeScreen__Fi(int v) { trace += "resetFade(" + num(v) + ");"; }
float evalHermite__Ffffff(float a, float, float, float, float) { trace += "herm(" + fnum(a) + ");"; return 0.5f; }
void boxDraw__FiiiiiUii(int a, int b, int c, int d, int e, unsigned color, int f) {
    trace += "box(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(color) + "," + num(f) + ");";
}
int hudRunSlider__FiiiiiUifPii(int x, int y, int w, int a, int b, unsigned color, float fraction, int* value, int player) {
    trace += "slider(" + num(x) + "," + num(y) + "," + num(w) + "," + num(a) + "," + num(b) + "," + num(color) + "," + fnum(fraction) + "," + num(player) + ");";
    ++sliderCalls;
    if (rSlider) *value = 5;
    return rSlider;
}
int changeSkin__Fii(int c, int v) { trace += "skin(" + num(c) + "," + num(v) + ");"; return rChange; }
int changeHairStyle__Fii(int c, int v) { trace += "style(" + num(c) + "," + num(v) + ");"; return rChange; }
int changeHairColor__Fii(int c, int v) { trace += "color(" + num(c) + "," + num(v) + ");"; return rChange; }
int changeBeard__Fii(int c, int v) { trace += "beard(" + num(c) + "," + num(v) + ");"; return rChange; }
int getHairStyle__Fi(int c) { trace += "getStyle(" + num(c) + ");"; return rHair; }
int gameButtonToIcon__Fi(int b) { trace += "icon(" + num(b) + ");"; return 100 + (b == 0x8000); }
void fontStart__FP4FontiUii(void* f, int a, unsigned color, int b) { trace += "start(" + num(f == smallFont) + "," + num(a) + "," + num(color) + "," + num(b) + ");"; }
void fontEnd__Fv() { trace += "end;"; }
void fontColor__FUiii(unsigned color, int a, int b) { trace += "color(" + num(color) + "," + num(a) + "," + num(b) + ");"; }
void fontPrintu__FiiPUsi(int x, int y, UString* t, int w) { trace += "pu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(w) + ");"; }
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* t, int w) { trace += "pcu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(w) + ");"; }
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString* t, int a, int b, int c, int d) {
    trace += "pwu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
    return 10;
}
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, UString* t, int a, int b, int c, int d) {
    trace += "pwc(" + num(x) + "," + num(y) + "," + text(t) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
    return 10;
}
int fontStringSizeu__FP4FontPUsi(void*, UString* t, int) { return 4 * static_cast<int>(text(t).size()); }
void textureSmoothBorder__FiiiiiUiiii(int a, int b, int c, int d, int e, unsigned color, int f, int g, int h) {
    trace += "smooth(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(color) + "," + num(f) + "," + num(g) + "," + num(h) + ");";
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)
static bool has(const std::string& part) { return trace.find(part) != std::string::npos; }
static bool before(const std::string& a, const std::string& b) { size_t x = trace.find(a), y = trace.find(b); return x != std::string::npos && y != std::string::npos && x < y; }
static unsigned char* info(int p) { return playerSelectInfo + 41 * p; }
static const float K077 = __builtin_bit_cast(float, 0x3d9db22du);

static const char* const kNames[10] = {"nm0", "nm1", "nm2", "nm3", "nm4", "nm5", "nm6", "nm7", "nm8", "nm9"};
alignas(8) static char nameText[10][8];
alignas(8) static char keyText[10][16];
alignas(8) static char lumpText[8];

static void setWord(unsigned char* base, int off, const void* p) {
    unsigned v = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p));
    std::memcpy(base + off, &v, 4);
}

static void reset() {
    trace.clear(); stringCount = 0; sliderCalls = 0;
    rGetMode = 0; rTex = 0; rYesNo = 0; rName = 0; rHair = 3; rSlider = 0; rChange = 0; rFade = 0; lumpFound = true;
    curPlayer = 0; charDelta = 0; selectMode = 1; newMode = 0; lockAlpha = 0; requestToExit = 0; statAllocation = 0; msgAlpha = 0; msgAlphaDelta = 0;
    curStat = 0; statAllocWarn = 0; startExitFade = 0; curSlider = 0; lastSlider = 0; maxSlider = 4;
    hermiteTime = 0; delta = 0; hermDeltaScale = 0; headerAlpha = 1.0f; bodyAlpha = 0.5f; otherHermiteTime = 1.0f; throb = 0;
    flareAlpha = 0.5f; flareDelta = 0.0f; firstTime = 0; throbR = throbG = throbB = throbA = 0;
    std::strcpy(lumpText, "npc"); npcLumpName = lumpText;
    playerSelectCount = 4; playerSelectStartPlayer = 0; playerSelectExitMessage = nullptr;
    frameNm = 0; coreLoading = 0; gameCam = nullptr; smallFont = &lumpFound; language = 0;
    padAcceptButton = 0x40; padBackButton = 0x120; padAcceptButtonFontChar = 77; padBackButtonFontChar = 88;
    std::memset(pad, 0, sizeof pad); std::memset(targets, 0, sizeof targets); std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
    std::memset(objects, 0, sizeof objects); std::memset(abilityTable, 0, sizeof abilityTable);
    for (int i = 0; i < 10; ++i) {
        std::snprintf(nameText[i], 8, "%s", kNames[i]);
        std::snprintf(keyText[i], 16, "key%d", i);
        setWord(targets + 636 * i, 0, nameText[i]);
        setWord(targets + 636 * i, 4, keyText[i]);
        setWord(targets + 636 * i, 12, keyText[i]);
        targets[636 * i + 17] = static_cast<unsigned char>(i % 5);
        targets[636 * i + 19] = 2;
        for (int k = 24; k < 28; ++k) targets[636 * i + k] = 5;
        setWord(targetObjs, 4 * i, objects[i]);
        objects[i][292] = 0;
        *reinterpret_cast<unsigned short*>(objects[i] + 294) = 0;
    }
    std::memset(hairStyleSetting, 1, 16); std::memset(hairColorSetting, 2, 16); std::memset(beardSetting, 3, 16); std::memset(skinSetting, 4, 16);
    std::strcpy(reinterpret_cast<char*>(playerSelectSoundLump), "snd");
    for (int i = 0; i < 4; ++i) statTable[i] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(keyText[i]));
    std::memset(hudFooterUni, 0, sizeof hudFooterUni);
}

int main() {
    // an early return while loading and one for a camera mode
    reset(); coreLoading = 1;
    playerSelectionTask__Fv();
    CHECK(trace.empty() && throbR == 132 && throbG == 132 && throbB == 104 && throbA == 47);
    reset(); rGetMode = 1;
    playerSelectionTask__Fv();
    CHECK(trace == "getmode;");

    // mode 0 moves on to mode 1 and the frame ends with the backdrop and prompt
    reset(); selectMode = 0;
    playerSelectionTask__Fv();
    CHECK(selectMode == 1 && hermDeltaScale == __builtin_bit_cast(float, 0x3e15818cu) && flareDelta == 0.0f);
    CHECK(trace.rfind("getmode;moveCam;lookCam;", 0) == 0);
    CHECK(has("smooth(21,29,269,374,6,") && has("us(<SelectPromptNext>,77,88,128,0);"));

    // the idle animations of the targets
    reset(); selectMode = 9;
    *reinterpret_cast<unsigned short*>(objects[1] + 294) = 0x88;
    *reinterpret_cast<unsigned short*>(objects[2] + 294) = 0x80;
    *reinterpret_cast<unsigned short*>(objects[3] + 294) = 0x80; objects[3][292] = 2;  // not below 2
    *reinterpret_cast<unsigned short*>(objects[4] + 294) = 0x08;                        // no 0x80 flag
    setWord(targetObjs, 20, nullptr);
    playerSelectionTask__Fv();
    CHECK(has("lump(npc,nm1_intro3.anm);anim(1,0.2,3);lump(npc,nm2_intro1.anm);anim(2,0.2,3);moveCam;"));
    CHECK(!has("nm3_") && !has("nm4_") && !has("nm5_"));
    lumpFound = false;
    reset(); lumpFound = false; selectMode = 9;
    *reinterpret_cast<unsigned short*>(objects[1] + 294) = 0x80;
    playerSelectionTask__Fv();
    CHECK(has("lump(npc,nm1_intro1.anm);moveCam;") && !has("anim("));

    // the first frame: fade -8 unless textures are loading
    reset(); selectMode = 9; firstTime = 1;
    playerSelectionTask__Fv();
    CHECK(has("texLoading;fade(-8);") && firstTime == 0);
    reset(); selectMode = 9; firstTime = 1; rTex = 1;
    playerSelectionTask__Fv();
    CHECK(has("texLoading;fade(0);"));
    reset(); selectMode = 9; firstTime = 1; rFade = 3;
    playerSelectionTask__Fv();
    CHECK(firstTime == 1 && trace.find("smooth") == std::string::npos);

    // fades
    reset(); selectMode = 9; bodyAlpha = 0.5f; headerAlpha = 0.5f;
    playerSelectionTask__Fv();
    CHECK(std::fabs(bodyAlpha - 0.609f) < 1e-4f && flareAlpha == bodyAlpha && std::fabs(headerAlpha - 0.609f) < 1e-4f);
    reset(); selectMode = 9; bodyAlpha = 0.95f; headerAlpha = 0.95f;
    playerSelectionTask__Fv();
    CHECK(bodyAlpha == 1.0f && headerAlpha == 1.0f);
    reset(); selectMode = 9; bodyAlpha = 0.5f; hermiteTime = 0.3f;
    playerSelectionTask__Fv();
    CHECK(std::fabs(bodyAlpha - 0.369f) < 1e-4f);
    reset(); selectMode = 9; bodyAlpha = 0.1f; lockAlpha = 1;
    playerSelectionTask__Fv();
    CHECK(bodyAlpha == 0.0f);
    reset(); selectMode = 9; flareDelta = K077; flareAlpha = 0.5f;
    playerSelectionTask__Fv();
    CHECK(std::fabs(flareAlpha - (0.5f + K077 * 0.99999994f)) < 1e-6f);
    reset(); selectMode = 9; flareDelta = -K077; flareAlpha = 0.01f;
    playerSelectionTask__Fv();
    CHECK(flareAlpha == 0.0f);

    // mode 1: the leave question for a later player, accept goes back one player
    reset(); curPlayer = 2; playerSelectStartPlayer = 0; requestToExit = 1; msgAlpha = 126; msgAlphaDelta = 16;
    pad[88 * 2 + 12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(msgAlpha == 126 && msgAlphaDelta == -17 && has("msg(<selectPrevPlayerTitle>,<selectPrevPlayer>|,FOOT,126,0,0);"));
    CHECK(has("us(<selectPrevPlayer>,2,0,0,0);us(<MemcardYesNo>,77,88,0,0);"));
    reset(); curPlayer = 2; playerSelectStartPlayer = 0; requestToExit = 1; msgAlpha = 126; pad[88 * 2 + 12] = 0x20; pad[88 * 2 + 13] = 0x01;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == -16);
    reset(); curPlayer = 2; playerSelectStartPlayer = 0; requestToExit = 1; msgAlpha = 0; msgAlphaDelta = -17;
    info(2)[32] = 4; info(1)[32] = 6;  // info(2)[-9] is info(1)[32]
    playerSelectionTask__Fv();
    CHECK(has("initPos(4,6);") && curPlayer == 1 && selectMode == 8 && requestToExit == 0 && msgAlphaDelta == 0 && std::fabs(delta - 0.054f) < 1e-4f);
    CHECK(has("lump(npc,nm6_intro2.anm);anim(6,0.2,14);"));
    reset(); curPlayer = 2; playerSelectStartPlayer = 0; requestToExit = 1; msgAlpha = 0; msgAlphaDelta = -16;
    playerSelectionTask__Fv();
    CHECK(curPlayer == 2 && selectMode == 1 && requestToExit == 0 && msgAlphaDelta == 0);
    // the yes/no question for the first player
    reset(); curPlayer = 1; playerSelectStartPlayer = 1; requestToExit = 1; rYesNo = 2;
    playerSelectionTask__Fv();
    CHECK(has("yesno(0,1,0,0);allDone(1);"));
    reset(); curPlayer = 1; playerSelectStartPlayer = 1; requestToExit = 1; rYesNo = 1;
    playerSelectionTask__Fv();
    CHECK(requestToExit == 0 && !has("allDone"));

    // mode 1: browsing the characters and accepting or cancelling
    reset(); info(0)[32] = 3; pad[8] = 0x00; pad[9] = 0x20; pad[10] = 0x80;  // 0x802000
    playerSelectionTask__Fv();
    CHECK(charDelta == 1 && has("initPos(3,4);blur(73);") && hermDeltaScale == 1.0f);
    reset(); info(0)[32] = 9; pad[9] = 0x20; pad[10] = 0x80;
    playerSelectionTask__Fv();
    CHECK(charDelta == 0 && !has("initPos"));
    reset(); info(0)[32] = 3; pad[8] = 0x00; pad[9] = 0x80; pad[10] = 0x40;      // 0x408000
    playerSelectionTask__Fv();
    CHECK(charDelta == -1 && has("blur(73);initPos(3,2);"));
    reset(); info(0)[32] = 0; pad[9] = 0x80; pad[10] = 0x40;
    playerSelectionTask__Fv();
    CHECK(charDelta == 0 && !has("initPos"));
    reset(); info(0)[32] = 2; pad[12] = 0x40; bodyAlpha = 0.5f;
    playerSelectionTask__Fv();
    CHECK(flareDelta == -K077 && maxSlider == 5 && otherHermiteTime == 1.0f && curSlider == 0 && lastSlider == 0);
    CHECK(has("lump(fx.lmp,menu_main_accept.anm);seq(1,0,1);"));
    reset(); pad[12] = 0x20; pad[13] = 0x01;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == 16 && requestToExit == 1 && msgAlpha == 0 && has("lump(fx.lmp,menu_main_back.anm);seq(1,0,1);"));
    reset(); hermiteTime = 0.6f; charDelta = 1; info(0)[32] = 3;
    playerSelectionTask__Fv();
    CHECK(info(0)[32] == 4 && charDelta == 0);
    reset(); hermiteTime = 0.6f; charDelta = -1; info(0)[32] = 3;
    playerSelectionTask__Fv();
    CHECK(info(0)[32] == 2 && charDelta == 0);
    reset(); hermiteTime = 0.4f; charDelta = 1; info(0)[32] = 3;
    playerSelectionTask__Fv();
    CHECK(info(0)[32] == 3 && charDelta == 1);
    // the screen draws the class text, then the description measure passes with the clipped (-100,-100) form
    reset(); info(0)[32] = 2;
    playerSelectionTask__Fv();
    CHECK(has("pwu(0,0,<key2>,219,-100,-100,17);sprintf") == false);
    CHECK(has("pwu(0,0,<key2>,219,-100,-100,17);pwu(0,0,<nm2_description>,219,-100,-100,17);"));
    CHECK(has("pu(41,40,<selectClass>,1000000);pu(41,69,<selectSex>,1000000);pu(41,103,<selectDescription>,1000000);"));
    CHECK(has("pu(41,132,<selectStrengths>,1000000);pu(41,161,<selectWeaknesses>,1000000);"));
    CHECK(has("pwu(50,57,<key2>,219,-50,600,17);pu(50,86,<key2>,1000000);pwu(50,120,<nm2_description>,219,-100,580,17);"));
    CHECK(has("pwu(50,149,<nm2_strengths>,219,-100,580,17);pwu(50,178,<nm2_weaknesses>,219,-100,580,17);"));
    // flareDelta negative with the alpha at zero: the character is confirmed and the sound plays
    reset(); info(0)[32] = 6; flareDelta = -K077; flareAlpha = 0.0f;
    playerSelectionTask__Fv();
    CHECK(has("resetName;lump(npc,nm6_intro2.anm);anim(6,0.2,14);lump(snd,ewm114.vag);sound(1,0,1,1,0);") && selectMode == 2 && flareDelta == K077);
    reset(); info(0)[32] = 8; flareDelta = -K077; flareAlpha = 0.0f;
    playerSelectionTask__Fv();
    CHECK(has("lump(snd,esm017_alta.vag);"));
    reset(); info(0)[32] = 2; flareDelta = -K077; flareAlpha = 0.0f;
    playerSelectionTask__Fv();
    CHECK(has("lump(snd,erm017_altc.vag);"));
    reset(); flareDelta = -K077; flareAlpha = 0.5f;
    playerSelectionTask__Fv();
    CHECK(selectMode == 1 && !has("resetName"));
    reset(); flareDelta = K077; flareAlpha = 1.0f - K077 + 0.0f;  // reaches 1.0 this frame
    playerSelectionTask__Fv();
    CHECK(flareDelta == 0.0f || flareAlpha != 1.0f);

    // mode 2: sliders and navigation
    reset(); selectMode = 2; info(0)[32] = 4; maxSlider = 4; flareAlpha = 1.0f; bodyAlpha = 1.0f; curSlider = 1; lastSlider = 0; otherHermiteTime = 1.0f;
    playerSelectionTask__Fv();
    CHECK(before("pwc(154,104,<selectCustomLookSkin>", "pwc(154,260,<selectCustomLookBeard>"));
    CHECK(has("slider(") && sliderCalls == 4 && has("getStyle(4);"));
    CHECK(has("slider(60,130,188,10,6,2149589024,1,-1);"));  // skin 4 of 5 options: fraction 4/4
    CHECK(has("slider(60,182,188,10,6,2149589024,0.75,0);"));  // hair style 3 / 4 with the pad focus on slider 1
    reset(); selectMode = 2; maxSlider = 3; flareAlpha = 1.0f; bodyAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(sliderCalls == 3 && !has("selectCustomLookBeard"));
    reset(); selectMode = 2; maxSlider = 3; rSlider = 1; rChange = 1;
    playerSelectionTask__Fv();
    CHECK(has("skin(0,5);lump(fx.lmp,menu_main_toggle.anm);seq(1,0,1);") && has("style(0,5);lump(fx.lmp,menu_main_toggle.anm);") && has("color(0,5);"));
    // accept stores the look and the first row of the ability table, back returns to mode 1
    reset(); selectMode = 2; info(1)[32] = 0; curPlayer = 1; targets[17] = 3;
    for (int i = 0; i < 80; ++i) abilityTable[i] = static_cast<unsigned char>(i + 100);
    pad[88 + 12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(newMode == 3 && statAllocation == 20 && curStat == 0 && statAllocWarn == 0 && flareDelta == -K077);
    CHECK(info(1)[33] == 1 && info(1)[34] == 2 && info(1)[35] == 4 && info(1)[36] == 3);
    CHECK(info(1)[37] == 148 && info(1)[38] == 152 && info(1)[39] == 156 && info(1)[40] == 160);
    reset(); selectMode = 2; pad[12] = 0x20; pad[13] = 0x01;
    playerSelectionTask__Fv();
    CHECK(newMode == 1 && flareDelta == -K077 && has("lump(npc,nm0_intro4.anm);anim(0,0.3,6);lump(fx.lmp,menu_main_back.anm);"));
    reset(); selectMode = 2; curSlider = 2; pad[16] = 0x00; pad[17] = 0x10; pad[18] = 0x10;  // 0x101000
    playerSelectionTask__Fv();
    CHECK(curSlider == 1 && otherHermiteTime == 0.0f && lastSlider == 2);
    reset(); selectMode = 2; curSlider = 0; maxSlider = 4; pad[16] = 0x00; pad[17] = 0x10; pad[18] = 0x10;
    playerSelectionTask__Fv();
    CHECK(curSlider == 0 && otherHermiteTime == 1.0f);
    reset(); selectMode = 2; curSlider = 2; maxSlider = 4; pad[17] = 0x40; pad[18] = 0x20;  // 0x204000
    playerSelectionTask__Fv();
    CHECK(curSlider == 3 && otherHermiteTime == 0.0f);
    reset(); selectMode = 2; curSlider = 3; maxSlider = 4; pad[17] = 0x40; pad[18] = 0x20;
    playerSelectionTask__Fv();
    CHECK(curSlider == 3);
    reset(); selectMode = 2; otherHermiteTime = 0.5f;
    playerSelectionTask__Fv();
    CHECK(std::fabs(otherHermiteTime - 0.6f) < 1e-3f);
    reset(); selectMode = 2; flareDelta = -K077; flareAlpha = 0.0f; newMode = 3;
    playerSelectionTask__Fv();
    CHECK(selectMode == 3 && flareDelta == K077);

    // mode 3: allocation
    reset(); selectMode = 3; statAllocation = 5; curStat = 1; info(0)[38] = 10; info(0)[32] = 2; targets[636 * 2 + 17] = 2;
    for (int i = 0; i < 80; ++i) abilityTable[i] = 0;
    pad[17] = 0x20; pad[18] = 0x80;  // 0x802000: increase
    playerSelectionTask__Fv();
    CHECK(info(0)[38] == 11 && statAllocation == 4 && has("lump(fx.lmp,menu_main_toggle.anm);"));
    reset(); selectMode = 3; statAllocation = 5; curStat = 1; info(0)[38] = 100; pad[17] = 0x20; pad[18] = 0x80;
    playerSelectionTask__Fv();
    CHECK(info(0)[38] == 100 && statAllocation == 5);
    reset(); selectMode = 3; statAllocation = 0; curStat = 1; info(0)[38] = 10; pad[17] = 0x20; pad[18] = 0x80;
    playerSelectionTask__Fv();
    CHECK(info(0)[38] == 10);
    reset(); selectMode = 3; statAllocation = 5; curStat = 2; info(0)[39] = 10; info(0)[32] = 1; targets[636 + 17] = 1;
    reinterpret_cast<int*>(abilityTable)[4 + 2] = 8;
    pad[17] = 0x80; pad[18] = 0x40;  // 0x408000: decrease
    playerSelectionTask__Fv();
    CHECK(info(0)[39] == 9 && statAllocation == 6);
    reset(); selectMode = 3; statAllocation = 5; curStat = 2; info(0)[39] = 8; info(0)[32] = 1; targets[636 + 17] = 1;
    reinterpret_cast<int*>(abilityTable)[4 + 2] = 8; pad[17] = 0x80; pad[18] = 0x40;
    playerSelectionTask__Fv();
    CHECK(info(0)[39] == 8 && statAllocation == 5);
    reset(); selectMode = 3; curStat = 0; pad[17] = 0x10; pad[18] = 0x10;
    playerSelectionTask__Fv();
    CHECK(curStat == 0);
    reset(); selectMode = 3; curStat = 2; pad[17] = 0x10; pad[18] = 0x10;
    playerSelectionTask__Fv();
    CHECK(curStat == 1 && has("menu_main_accept.anm"));
    reset(); selectMode = 3; curStat = 2; pad[17] = 0x40; pad[18] = 0x20;
    playerSelectionTask__Fv();
    CHECK(curStat == 3);
    reset(); selectMode = 3; curStat = 3; pad[17] = 0x40; pad[18] = 0x20;
    playerSelectionTask__Fv();
    CHECK(curStat == 3 && !has("menu_main_accept.anm"));
    // accept with points left warns, accept with none leaves for the name pad, back returns
    reset(); selectMode = 3; statAllocation = 3; pad[12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(statAllocWarn == 1 && msgAlphaDelta == 16);
    reset(); selectMode = 3; statAllocation = 0; pad[12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(has("menu_main_accept.anm"));
    CHECK(has("toNamePad;") && newMode == 4 && flareDelta == -K077);
    reset(); selectMode = 3; pad[12] = 0x20; pad[13] = 0x01;
    playerSelectionTask__Fv();
    CHECK(newMode == 2 && flareDelta == -K077 && has("menu_main_back.anm"));
    // the warning pop-up
    reset(); selectMode = 3; statAllocWarn = 1; msgAlpha = 126; msgAlphaDelta = 16; pad[12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == -16 && has("msg(<selectSetStatsWarnTitle>,<selectSetStatsWarnBody>,FOOT,126,0,0);"));
    reset(); selectMode = 3; statAllocWarn = 1; msgAlpha = 0; msgAlphaDelta = -16;
    playerSelectionTask__Fv();
    CHECK(statAllocWarn == 0);
    reset(); selectMode = 3; statAllocWarn = 1; msgAlpha = 40; msgAlphaDelta = -16;
    playerSelectionTask__Fv();
    CHECK(statAllocWarn == 1 && msgAlpha == 24);
    reset(); selectMode = 3; statAllocWarn = 1; msgAlpha = 125; msgAlphaDelta = 16;
    playerSelectionTask__Fv();
    CHECK(msgAlpha == 126);
    // the allocation page text and rows
    reset(); selectMode = 3; flareAlpha = 1.0f; bodyAlpha = 1.0f; statAllocation = 9; curStat = 1; info(0)[37] = 12; info(0)[38] = 20;
    for (int i = 0; i < 80; ++i) abilityTable[i] = 0;
    playerSelectionTask__Fv();
    CHECK(has("usc(%d,9);pcu(250,83,%d|9,1000000);") && has("pu(41,117,<key0>,1000000);usc(%d,12);pcu(230,117,%d|12,1000000);"));
    CHECK(has("pu(41,134,<key1>,1000000);usc(%d,20);pcu(230,134,%d|20,1000000);icon(32768);usc(%c,101);pcu(212,134,%c|101,1000000);icon(8192);usc(%c,100);pcu(250,134,%c|100,1000000);"));
    CHECK(has("ustrcpy;pwu(41,219,<selectStatIntelDesc>,219,-100,580,17);"));
    reset(); selectMode = 3; flareAlpha = 1.0f; bodyAlpha = 1.0f; curStat = 0; info(0)[37] = 6; info(0)[32] = 1; targets[636 + 17] = 0;
    reinterpret_cast<int*>(abilityTable)[0] = 6;
    playerSelectionTask__Fv();
    CHECK(!has("icon(32768)") && has("icon(8192)"));  // at the minimum the decrease button is not shown

    // mode 4: the name pad
    reset(); selectMode = 4; rName = 0;
    playerSelectionTask__Fv();
    CHECK(has("doNamePad;drawNamePad;") && flareDelta == 0.0f);
    reset(); selectMode = 4; rName = 1; curPlayer = 1; playerSelectStartPlayer = 0; playerSelectCount = 3; info(1)[32] = 5;
    playerSelectionTask__Fv();
    CHECK(newMode == 6 && msgAlphaDelta == 16 && flareDelta == -K077 && has("lump(npc,nm5_intro4.anm);anim(5,0.3,22);"));
    reset(); selectMode = 4; rName = 1; curPlayer = 2; playerSelectStartPlayer = 0; playerSelectCount = 3;
    playerSelectionTask__Fv();
    CHECK(has("resetFade(0);") && selectMode == 5);
    reset(); selectMode = 4; rName = -1;
    playerSelectionTask__Fv();
    CHECK(newMode == 3 && flareDelta == -K077);
    reset(); selectMode = 4; rName = 2;
    playerSelectionTask__Fv();
    CHECK(newMode == 0 && flareDelta == -K077);
    reset(); selectMode = 4; flareDelta = -K077; flareAlpha = 0.0f; newMode = 3;
    playerSelectionTask__Fv();
    CHECK(selectMode == 3 && flareDelta == K077 && has("drawNamePad;") && !has("doNamePad;"));

    // modes 5 to 8
    reset(); selectMode = 5; flareAlpha = 0.5f;
    playerSelectionTask__Fv();
    CHECK(startExitFade == 0 && bodyAlpha == flareAlpha && !has("fade("));
    reset(); selectMode = 5; *reinterpret_cast<unsigned short*>(objects[0] + 294) = 0x80; rFade = 128;
    playerSelectionTask__Fv();
    CHECK(startExitFade == 1 && has("fade(4);allDone(0);"));
    reset(); selectMode = 5; startExitFade = 1; rFade = 7;
    playerSelectionTask__Fv();
    CHECK(has("fade(4);") && !has("allDone"));
    reset(); selectMode = 6; *reinterpret_cast<unsigned short*>(objects[0] + 294) = 0x10;
    playerSelectionTask__Fv();
    CHECK(trace.find("selectNextPlayer") == std::string::npos && trace.find("smooth") == std::string::npos);
    reset(); selectMode = 6; msgAlpha = 126; curPlayer = 1; pad[88 + 12] = 0x40;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == -17 && has("us(<selectNextPlayer>,2,3,0,0);us(<MemcardContinueBack>,77,88,0,0);msg(<selectNextPlayerTitle>,<selectNextPlayer>|,FOOT,126,0,0);"));
    reset(); selectMode = 6; msgAlpha = 126; pad[12] = 0x20; pad[13] = 0x01;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == -16);
    reset(); selectMode = 6; msgAlpha = 0; msgAlphaDelta = -17;
    playerSelectionTask__Fv();
    CHECK(has("postSelect;") && msgAlphaDelta == 0);
    reset(); selectMode = 6; msgAlpha = 0; msgAlphaDelta = -16;
    playerSelectionTask__Fv();
    CHECK(flareDelta == -K077 && newMode == 4 && has("lump(npc,nm0_intro2.anm);anim(0,0.2,14);") && msgAlphaDelta == 0);
    reset(); selectMode = 6; msgAlpha = 0; msgAlphaDelta = 5;
    playerSelectionTask__Fv();
    CHECK(msgAlphaDelta == 5 && !has("postSelect"));
    reset(); selectMode = 6; msgAlpha = 50; msgAlphaDelta = -16;
    playerSelectionTask__Fv();
    CHECK(msgAlpha == 34 && msgAlphaDelta == -16);
    reset(); selectMode = 6; flareDelta = -K077; flareAlpha = 0.0f; newMode = 7;
    playerSelectionTask__Fv();
    CHECK(selectMode == 7 && flareDelta == K077);
    reset(); selectMode = 7; hermiteTime = 0.0f; curPlayer = 1; flareAlpha = 0.4f;
    playerSelectionTask__Fv();
    CHECK(curPlayer == 2 && info(2)[32] == 0 && has("preSelect;") && hermDeltaScale == __builtin_bit_cast(float, 0x3e15818cu) && bodyAlpha != 0.0f);
    reset(); selectMode = 7; hermiteTime = 0.4f; curPlayer = 1;
    playerSelectionTask__Fv();
    CHECK(curPlayer == 1 && !has("preSelect"));
    reset(); selectMode = 8; hermiteTime = 0.0f;
    playerSelectionTask__Fv();
    CHECK(selectMode == 4 && has("toNamePad;"));
    reset(); selectMode = 8; hermiteTime = 0.4f;
    playerSelectionTask__Fv();
    CHECK(selectMode == 8 && !has("toNamePad"));

    // the prompt lines
    reset(); selectMode = 1; info(0)[32] = 3; flareAlpha = 1.0f; bodyAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("us(<SelectPromptPrevNext>,127,77,88,128);") && has("start(1,6,1664182372,128);pcu(320,410,<SelectPromptPrevNext>|,1000000);end;"));
    CHECK(has("smooth(21,29,269,374,6,1275068416,1,0,0);") && has("smooth(") );
    reset(); selectMode = 1; info(0)[32] = 0; flareAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("us(<SelectPromptNext>,77,88,128,0);"));
    reset(); selectMode = 1; info(0)[32] = 9; flareAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("us(<SelectPromptPrev>,127,77,88,0);"));
    reset(); selectMode = 4; flareAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("us(<SelectPromptName>,88,0,0,0);"));
    reset(); selectMode = 5; flareAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("us(<SelectPrompt>,77,88,0,0);"));
    reset(); selectMode = 9; flareAlpha = 1.0f; language = 1; bodyAlpha = 1.0f;
    playerSelectionTask__Fv();
    CHECK(has("smooth(21,29,334,374,6,") && has("smooth(") );
    reset(); selectMode = 9; flareAlpha = 0.0f; flareDelta = -K077; bodyAlpha = 0.0f;
    playerSelectionTask__Fv();
    CHECK(trace.find("pcu(320,410") == std::string::npos);
    std::puts("ok");
    return 0;
}
