#include "frontend_run_controller_config.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

int inGameConfigShuttingDown, finishFrameCount, frontendControllerConfigRunning, initializedLump, askedForConfigLoad;
int engineDisplayDisable, configMode, curFocus, curSel, newSel, gameNoDraw, frontEndRunning;
int padAcceptButtonFontChar = 5, padBackButtonFontChar = 6;
void* myLumpBuffer;
signed char configLumpLoaded, hudNoDraw;
float t;
Font* smallFont = reinterpret_cast<Font*>(0x1000);
unsigned padAcceptButton = 0x40, padBackButton = 0x100;
alignas(4) unsigned char pad[352];
alignas(4) unsigned char MC_GameConfig[252];
char* configStrings[16];
unsigned buttonValues[14] = {0x20, 0x10, 0x80, 0x40, 0, 0, 0, 0, 4, 1, 8, 2, 0x400, 0};

static std::string log_;
static void ev(const char* fmt, ...) {
    char tmp[200];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(tmp, sizeof tmp, fmt, ap);
    va_end(ap);
    log_ += tmp;
    log_ += ";";
}

alignas(256) static unsigned char arena[1024];
static unsigned char* lock_result = arena + 1;
static unsigned short texts[64][1];
static int text_n;
static char text_keys[64][40];
static char* resources[8];
static char resource_names[8][40];
static int resource_n;
alignas(8) static unsigned char lump_find_blob[16];
static char texture_marker;
static int default_calls;
static float hermite_result = 0.5f;
static void* last_font;

extern "C" void lumpUnload__FPv(void* b) { ev("UNLOAD(%d)", b == myLumpBuffer); }
extern "C" void textureReleaseCacheBuffer__Fv() { ev("RELEASE"); }
extern "C" unsigned char* textureLockCacheBuffer__Fv() { ev("LOCK"); return lock_result; }
extern "C" void cdLoadAsync__FPcPvT0ii(char* n, void* b, void* d, int a, int c) {
    ev("CDLOAD(%s,%d,%d,%d,%d)", n, b == myLumpBuffer, d == &configLumpLoaded, a, c);
}
extern "C" void lumpAdd__FPvPCc(void* b, const char* n) { ev("LUMPADD(%d,%s)", b == myLumpBuffer, n); }
extern "C" void* lumpFindResource__FPcT0(char* g, char* n) {
    ev("LFR(%s,%s)", g, n);
    std::strcpy(resource_names[resource_n], n);
    resources[resource_n] = resource_names[resource_n];
    return resources[resource_n++];
}
extern "C" unsigned char* lumpFind__FPvPc(void* b, char* n) { ev("LUMPFIND(%d,%s)", b == myLumpBuffer, n); return lump_find_blob; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    ev("SND(%s,%d,%g)", static_cast<char*>(s), o == 0, v);
}
extern "C" void gameSetDefaultGameConfig__Fi(int p) { ev("DEFAULTS(%d)", p); ++default_calls; }
extern "C" void coreStartLoading__Fv() { ev("CORESTART"); }
extern "C" unsigned short* textGetString__FPc(char* key) {
    ev("TGS(%s)", key);
    std::strcpy(text_keys[text_n], key);
    texts[text_n][0] = static_cast<unsigned short>(0xa000 + text_n);
    return texts[text_n++];
}
static const char* key_of(unsigned short* text) {
    int n = text[0] - 0xa000;
    return text_keys[n];
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ev("FMT(%s,%g,%g,%g,%g,%g,%g,%g,%g)", key_of(f), a, b, c, dd, e, ff, g, h);
    d[0] = 0xb000;
}
extern "C" void fontStart__FP4FontiUii(void* f, int a, unsigned c, int b) { last_font = f; ev("FS(%d,%x,%d,%p)", a, c, b, f); }
extern "C" void fontEnd__Fv() { ev("FE"); }
extern "C" void fontColor__FUiii(unsigned c, int a, int b) { ev("FC(%x,%d,%d)", c, a, b); }
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* s, int w) {
    ev("FPC(%d,%d,%s,%d)", x, y, s[0] == 0xb000 ? "buf" : key_of(s), w);
}
extern "C" void fontPrintu__FiiPUsi(int x, int y, unsigned short* s, int w) {
    if (s[0] >= 0xa000) ev("FP(%d,%d,%s,%d)", x, y, s[0] == 0xb000 ? "buf" : key_of(s), w);
    else ev("FP(%d,%d,glyph%d,%d)", x, y, s[0], w);
}
extern "C" int fontStringSizeu__FP4FontPUsi(void* f, unsigned short* s, int w) {
    ev("FSIZE(%d,%s,%d)", f == last_font, key_of(s), w);
    return 30;
}
extern "C" void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned c, char* g, int b, int d) {
    ev("TDB(%d,%d,%d,%d,%d,%x,%s,%d,%d)", x, y, w, h, a, c, g ? g : "-", b, d);
}
extern "C" void textureDrawColor__FP8_textureiiiUi(void* tex, int x, int y, int z, unsigned c) {
    ev("TDC(%s,%d,%d,%d,%x)", tex == &texture_marker ? "blob" : (tex ? static_cast<char*>(tex) : "null"), x, y, z, c);
}
extern "C" void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned f) { ev("RECT(%d,%d,%d,%d,%d,%x)", a, b, c, d, e, f); }
extern "C" float evalHermite__Ffffff(float tt, float a, float b, float c, float d) {
    ev("HERMITE(%g,%g,%g,%g,%g)", tt, a, b, c, d);
    return hermite_result;
}

static void setword(int record, int offset, unsigned v) { *reinterpret_cast<unsigned*>(pad + record * 88 + offset) = v; }
static unsigned* cfg(int focus, int row) { return reinterpret_cast<unsigned*>(MC_GameConfig + 52 + 36 * focus + 4 * row); }
static void reset() {
    log_.clear(); text_n = 0; resource_n = 0; default_calls = 0;
    inGameConfigShuttingDown = 0; finishFrameCount = 0; frontendControllerConfigRunning = 1; initializedLump = 1;
    askedForConfigLoad = 1; engineDisplayDisable = 0; configMode = 0; curFocus = 0; curSel = 0; newSel = 0; gameNoDraw = 7;
    frontEndRunning = 0; myLumpBuffer = arena; configLumpLoaded = 0; hudNoDraw = 5; t = 0.0f;
    *reinterpret_cast<void**>(lump_find_blob + 4) = &texture_marker;
    std::memset(pad, 0, sizeof pad); std::memset(MC_GameConfig, 0, sizeof MC_GameConfig);
    setword(0, 4, 1); setword(1, 4, 1);
    static char names[16][16];
    const char* base[9] = {"c_attack", "c_magic1", "c_magic2", "c_action", "c_block", "c_health", "c_mana", "c_automap", "c_weapontoggle"};
    for (int i = 0; i < 9; ++i) { std::strcpy(names[i], base[i]); configStrings[i] = names[i]; }
    for (int i = 9; i < 16; ++i) { std::strcpy(names[i], "c_extra"); configStrings[i] = names[i]; }
    const unsigned rows[9] = {0x20, 0x10, 0x80, 0x40, 4, 1, 8, 2, 0x400};
    for (int i = 0; i < 9; ++i) *cfg(0, i) = rows[i];
    for (int i = 0; i < 9; ++i) *cfg(1, i) = rows[i];
}
#define RUN() frontendRunControllerConfig__Fv()
#define CHECK(n, cond) do { if (!(cond)) { std::printf("fail %d log=%s\n", n, log_.c_str()); return n; } } while (0)

static bool starts(const char* part) { return log_.compare(0, std::strlen(part), part) == 0; }
static bool ends(const char* part) { size_t n = std::strlen(part); return log_.size() >= n && log_.compare(log_.size() - n, n, part) == 0; }
static bool has(const char* part) { return log_.find(part) != std::string::npos; }
static int count(const char* part) { int n = 0; for (size_t p = log_.find(part); p != std::string::npos; p = log_.find(part, p + 1)) ++n; return n; }

int main() {
    // Shutdown counter.
    for (int n : {0, 1, 2, 4}) {
        reset(); inGameConfigShuttingDown = 1; finishFrameCount = n; RUN();
        CHECK(1, finishFrameCount == n + 1 && log_.empty() && frontendControllerConfigRunning == 1);
    }
    reset(); inGameConfigShuttingDown = 1; finishFrameCount = 3; RUN();
    CHECK(2, log_ == "UNLOAD(1);RELEASE;" && finishFrameCount == 4 && frontendControllerConfigRunning == 0);

    // Lump load.
    reset(); initializedLump = 0; myLumpBuffer = 0; askedForConfigLoad = 0; RUN();
    CHECK(3, log_ == "LOCK;" && engineDisplayDisable == 10 && myLumpBuffer == arena + 256 && initializedLump == 0);
    reset(); initializedLump = 0; askedForConfigLoad = 0; RUN();
    CHECK(4, log_ == "CDLOAD(config.lmp,1,1,0,0);" && askedForConfigLoad == 1 && engineDisplayDisable == 10 && initializedLump == 0);
    reset(); initializedLump = 0; configLumpLoaded = 0; RUN();
    CHECK(5, log_.empty() && initializedLump == 0 && engineDisplayDisable == 10);
    reset(); initializedLump = 0; configLumpLoaded = 1; RUN();
    CHECK(7, initializedLump == 1 && starts("LUMPADD(1,config.lmp);") && has("TGS(SelectDefaultsBack)"));
    reset(); initializedLump = 0; myLumpBuffer = 0; askedForConfigLoad = 1; configLumpLoaded = 1; RUN();  // buffer step wins
    CHECK(8, log_ == "LOCK;" && initializedLump == 0);

    // Baseline configMode 0 pass: golden call order.
    reset(); RUN();
    {
        std::string want = "FS(7,64464646,-1,0x1000);TGS(SelectDefaultsBack);FMT(SelectDefaultsBack,5,125,6,0,0,0,0,0);FPC(320,399,buf,1000000);FE;";
        want += "TDB(342,117,268,217,6,80808080,-,1,0);FS(6,802d5049,-1,0x1000);TGS(PlayerConfiguration);FMT(PlayerConfiguration,1,0,0,0,0,0,0,0);FPC(475,47,buf,1000000);";
        const int glyph[9] = {0, 1, 2, 3, 8, 9, 10, 11, 12};
        const char* key[9] = {"c_attack", "c_magic1", "c_magic2", "c_action", "c_block", "c_health", "c_mana", "c_automap", "c_weapontoggle"};
        for (int r = 0; r < 9; ++r) {
            char b[200];
            std::snprintf(b, sizeof b, "FC(802d5049,-1,0);FP(360,%d,glyph%d,1000000);TGS(%s);FP(392,%d,%s,1000000);", 23 * r + 118, 123 + glyph[r], key[r], 23 * r + 118, key[r]);
            want += b;
        }
        want += "FC(80323232,-1,0);TGS(cstatic_move);FSIZE(1,cstatic_move,1000000);TGS(cstatic_move);FP(74,279,cstatic_move,1000000);";
        want += "TGS(cstatic_inventory);FSIZE(1,cstatic_inventory,1000000);TGS(cstatic_inventory);FP(104,313,cstatic_inventory,1000000);";
        want += "TGS(cstatic_camera);FP(250,280,cstatic_camera,1000000);TGS(cstatic_pause);FP(224,312,cstatic_pause,1000000);";
        want += "TGS(cstatic_spells);FSIZE(1,cstatic_spells,1000000);TGS(cstatic_spells);FP(52,246,cstatic_spells,1000000);FE;";
        want += "HERMITE(0,0,1,0,0);RECT(349,121,603,145,6,50111824);";
        want += "TDB(340,42,270,40,6,80808080,-,0,0);TDB(342,117,268,217,6,80808080,-,0,1);LUMPFIND(1,dualshock2.tex);TDC(blob,0,0,6,80808080);";
        // The pointers print as host addresses, so compare after masking them.
        std::string got = log_;
        auto mask = [](std::string& s) {
            for (size_t p = s.find("0x"); p != std::string::npos; p = s.find("0x", p + 1)) {
                size_t e = p + 2;
                while (e < s.size() && std::isxdigit(static_cast<unsigned char>(s[e]))) ++e;
                s.replace(p, e - p, "PTR");
            }
        };
        mask(got); mask(want);
        if (got != want) { std::printf("baseline mismatch\n got=%s\nwant=%s\n", got.c_str(), want.c_str()); return 9; }
    }
    CHECK(11, configMode == 0 && curSel == 0 && newSel == 0 && t == 0.0f);

    // Row move: down pressed (0x4000), toggle sound, newSel, animation starts at once.
    reset(); setword(0, 16, 0x4000); RUN();
    CHECK(12, starts("LFR(fx.lmp,menu_main_toggle.anm);SND(menu"));
    CHECK(13, newSel == 1 && curSel == 0 && t == 0.25f && has("SND(menu_main_toggle.anm,1,1);FS(7"));
    CHECK(14, has("HERMITE(0.25,0,1,0,0)") && has("RECT(349,"));
    CHECK(15, count("SND(menu_main_accept.anm,1,1);") == 1 && ends("SND(menu_main_accept.anm,1,1);"));  // the pending-move sound ends the pass
    // Up with curSel zero does nothing; up with curSel > 0 moves.
    reset(); setword(0, 16, 0x1000); RUN();
    CHECK(16, newSel == 0 && t == 0.0f && !has("menu_main_toggle"));
    reset(); curSel = 3; newSel = 3; setword(0, 16, 0x100000); RUN();
    CHECK(17, newSel == 2 && curSel == 3 && t == 0.25f && count("menu_main_toggle.anm") == 2);  // LFR + SND names
    reset(); curSel = 8; newSel = 8; setword(0, 16, 0x200000); RUN();
    CHECK(18, newSel == 8 && !has("menu_main_toggle"));
    reset(); curSel = 7; newSel = 7; setword(0, 16, 0x4000); RUN();
    CHECK(19, newSel == 8);
    reset(); curSel = 1; newSel = 1; setword(0, 16, 0x101000 | 0x204000); RUN();
    CHECK(20, newSel == 2 && count("SND(menu_main_toggle.anm") == 2);  // up then down; the later write wins
    reset(); curSel = 1; newSel = 1; setword(0, 16, 0x1); RUN();
    CHECK(21, newSel == 1 && !has("menu_main_toggle"));
    // Animation steps.
    reset(); curSel = 1; newSel = 3; t = 0.25f; RUN();
    CHECK(22, t == 0.5f && curSel == 1 && !has("SND("));
    reset(); curSel = 1; newSel = 3; t = 0.75f; RUN();
    CHECK(23, t == 1.0f && curSel == 1);
    reset(); curSel = 1; newSel = 3; t = 1.0f; RUN();
    CHECK(24, t == 0.0f && curSel == 3 && newSel == 3 && !has("SND("));
    reset(); curSel = 1; newSel = 3; t = 0.0f; RUN();
    CHECK(25, t == 0.25f && has("SND(menu_main_accept.anm"));
    reset(); curSel = 1; newSel = 3; t = 0.5f; RUN();
    CHECK(26, !has("SND("));
    reset(); curSel = 2; newSel = 4; t = 0.5f; RUN();
    CHECK(27, has("HERMITE(0.75,0,1,0,0);RECT(349,190,603,214,6,50111824);"));
    // t == 1.0 leaves the move input unused; t strictly between 0 and 1 reads neither input.
    reset(); curSel = 1; newSel = 1; t = 0.5f; setword(0, 16, 0x4000); setword(0, 12, padAcceptButton); RUN();
    CHECK(28, newSel == 1 && configMode == 0 && !has("SND("));

    // Accept, defaults, back.
    reset(); setword(0, 12, padAcceptButton); RUN();
    CHECK(29, configMode == 1 && starts("LFR(fx.lmp,menu_main_accep") && count("SND(menu_main_accept.anm") == 1);
    CHECK(30, has("FPC(320,399,buf") && !has("newConfigButton"));  // this pass still draws the row-choice caption
    log_.clear(); text_n = 0; setword(0, 12, 0); RUN();
    CHECK(30, has("TGS(newConfigButton)") && has("FPC(320,360,newConfigButton,1000000)") && has("FPC(320,377,c_attack,1000000)"));
    reset(); curSel = 2; newSel = 2; setword(0, 12, 0x80); RUN();
    CHECK(31, default_calls == 1 && has("DEFAULTS(0)") && configMode == 0 && count("SND(menu_main_accept.anm") == 1);
    reset(); curFocus = 1; setword(1, 12, 0x80); RUN();
    CHECK(32, has("DEFAULTS(1)"));
    reset(); setword(0, 12, padBackButton); RUN();
    CHECK(33, log_ == "LFR(fx.lmp,menu_main_back.anm);SND(menu_main_back.anm,1,1);CORESTART;" && curFocus == 0 && gameNoDraw == 0 && hudNoDraw == 0 && inGameConfigShuttingDown == 1);
    reset(); curFocus = 1; setword(1, 4, 0); RUN();
    CHECK(34, has("SND(menu_main_back.anm") && inGameConfigShuttingDown == 1 && curFocus == 0 && gameNoDraw == 0 && !has("FS("));
    reset(); curFocus = 1; setword(1, 12, padBackButton); setword(0, 12, 0); RUN();
    CHECK(35, inGameConfigShuttingDown == 1);
    reset(); curFocus = 1; setword(0, 12, padBackButton | 0x80 | padAcceptButton); RUN();
    CHECK(36, inGameConfigShuttingDown == 0 && configMode == 0 && default_calls == 0);
    // Accept is ignored while t is not zero.
    reset(); t = 0.5f; curSel = 1; newSel = 3; setword(0, 12, padAcceptButton); RUN();
    CHECK(37, configMode == 0);

    // frontEndRunning selects the group, the font and the texture source.
    reset(); frontEndRunning = 1; RUN();
    CHECK(38, has("LFR(frontend.lmp,gamefontsmall.fnt)") && has("TDB(342,117,268,217,6,80808080,frontend.lmp,1,0)") &&
                  has("TDB(340,42,270,40,6,80808080,frontend.lmp,0,0)") && has("LFR(frontend.lmp,dualshock2.tex);TDC(dualshock2.tex,0,0,6,80808080)") &&
                  !has("LUMPFIND"));
    CHECK(39, count("LFR(frontend.lmp,gamefontsmall.fnt)") == 2);  // one lookup per font acquisition

    // configMode 1.
    reset(); configMode = 1; curSel = 4; setword(0, 12, 0); RUN();
    CHECK(40, has("FS(7,64464646") && has("FPC(320,377,c_block,1000000)") && has("FC(802d5049,-1,0);TGS(c_block);FPC(320,377") && configMode == 1);
    CHECK(41, has("FC(80162824,64,0)") && count("FC(80162824,64,0)") == 8 && has("FP(360,210,glyph") && !has("DEFAULTS"));
    reset(); configMode = 1; curSel = 4; setword(0, 12, 0); RUN();
    CHECK(42, !has("FPC(320,399"));
    // Pressing a mapped button swaps with the existing owner.
    reset(); configMode = 1; curSel = 1; setword(0, 12, 0x80); RUN();  // row 2 holds 0x80
    CHECK(43, *cfg(0, 1) == 0x80 && *cfg(0, 2) == 0x10 && configMode == 0 && *cfg(0, 0) == 0x20 && *cfg(1, 1) == 0x10);
    CHECK(44, count("SND(menu_main_accept.anm") == 1);
    reset(); configMode = 1; curSel = 0; setword(0, 12, 0x20); RUN();  // same button as the current row
    CHECK(45, *cfg(0, 0) == 0x20 && *cfg(0, 1) == 0x10 && configMode == 0);
    reset(); configMode = 1; curSel = 8; setword(0, 12, 0x20); RUN();  // row 0 owns it
    CHECK(46, *cfg(0, 8) == 0x20 && *cfg(0, 0) == 0x400 && configMode == 0);
    reset(); configMode = 1; curSel = 3; setword(0, 12, 0x4000); RUN();  // no mapped button
    CHECK(47, configMode == 1 && *cfg(0, 3) == 0x40 && !has("SND("));
    reset(); configMode = 1; curSel = 3; *cfg(0, 6) = 0x4000; setword(0, 12, 0x800); RUN();  // button 0x400? bit mismatch
    CHECK(48, configMode == 1);
    reset(); configMode = 1; curSel = 5; setword(0, 12, 0x2); RUN();  // value 2 is unowned here? row 7 owns it
    CHECK(49, *cfg(0, 5) == 2 && *cfg(0, 7) == 1 && configMode == 0);
    reset(); configMode = 1; curSel = 2; setword(0, 12, 0x20 | 0x10); RUN();  // the first set bit in table order wins
    CHECK(50, *cfg(0, 2) == 0x20 && *cfg(0, 0) == 0x80 && configMode == 0);
    reset(); configMode = 1; curSel = 2; curFocus = 1; setword(1, 12, 0x40); RUN();
    CHECK(51, *cfg(1, 2) == 0x40 && *cfg(1, 3) == 0x80 && *cfg(0, 2) == 0x80 && *cfg(0, 3) == 0x40);
    // The owner of the pressed button is the first row holding it; later duplicates are untouched.
    reset(); configMode = 1; curSel = 2; *cfg(0, 5) = 0x20; setword(0, 12, 0x20); RUN();
    CHECK(52, *cfg(0, 2) == 0x20 && *cfg(0, 0) == 0x80 && *cfg(0, 5) == 0x20);
    // Other configMode values neither print nor change anything but the shared drawing.
    reset(); configMode = 2; RUN();
    CHECK(53, !has("FPC(320,399") && !has("FPC(320,360") && configMode == 2 && count("FC(80162824,64,0)") == 8);
    reset(); configMode = -1; RUN();
    CHECK(54, !has("FPC(320,399") && count("FC(80162824,64,0)") == 8);

    // Rows: dimming, the row counter, and unknown button values.
    reset(); curSel = 3; newSel = 3; configMode = 0; RUN();
    CHECK(55, count("FC(80162824,64,0)") == 0 && count("FC(802d5049,-1,0)") == 9);
    reset(); curSel = 3; newSel = 3; configMode = 1; RUN();
    CHECK(56, count("FC(80162824,64,0)") == 8);
    reset(); *cfg(0, 2) = 0x999; RUN();  // not among buttonValues
    CHECK(58, has("FP(360,118,glyph123") && has("FP(360,141,glyph124") && has("FP(360,440,glyph0,1000000)") && has("FP(392,440,c_extra,1000000)") && !has("FP(360,187,"));
    reset(); *cfg(0, 0) = 0; RUN();  // value 0 first matches table entry 4
    CHECK(59, has("FP(360,118,glyph127,1000000)"));
    reset(); *cfg(0, 1) = 0; *cfg(0, 0) = 0x2; RUN();
    CHECK(60, has("FP(360,118,glyph134,1000000)") && has("FP(360,141,glyph127,1000000)"));
    reset(); curFocus = 1; RUN();
    CHECK(61, has("FMT(PlayerConfiguration,2,0,0,0,0,0,0,0)") && has("FMT(SelectDefaultsBack,5,125,6,0,0,0,0,0)"));

    std::puts("frontend run controller config passed");
    return 0;
}
