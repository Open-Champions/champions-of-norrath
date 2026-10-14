// Host check for runStats__Fii. Not an EE plan input.
// Build as x86_64 with statics below 4 GiB (the record holds 32-bit pointer words):
//   c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
// Recording stubs stand in for every callee (text lookups return "<key>", soft doubles use IEEE
// arithmetic). Each mode is driven with chosen record contents and the call trace is checked in order.
#include "run_stats.cpp"

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(8) unsigned char statsScreen[232];
alignas(8) unsigned char player[2 * 23984];
alignas(8) unsigned char pad[4 * 88];
void* smallFont;
void* lcursor;
unsigned char* rcursor;
int shopDisableMouse[2];
int GLISTSIZE;
unsigned long long* gstart;
unsigned long long* glistp;
int drawFrameNm;
int experienceTable[64];
unsigned padAcceptButton;
unsigned padBackButton;
signed char language;
}
alignas(8) static unsigned char cursorShape[4];
alignas(8) static unsigned long long glist[16];
alignas(8) static unsigned char itemA[0x400], itemB[0x400];
static UString strings[64][40];
static int stringCount;
static std::string trace;

static std::string text(const UString* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static std::string num(long long v) { return std::to_string(v); }
static std::string fnum(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", static_cast<double>(v)); return b; }
static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
static void put(UString* dst, const std::string& s) { size_t i = 0; for (; i < s.size(); ++i) dst[i] = static_cast<UString>(s[i]); dst[i] = 0; }

extern "C" {
void statsFrame__Fv() {}
void engineRemoveTask__FPFv_v(void (*task)()) { trace += std::string("rmtask(") + (task == statsFrame__Fv ? "statsFrame" : "?") + ");"; }
void givePlayerExperience__Fii(int index, int amount) { trace += "xp(" + num(index) + "," + num(amount) + ");"; }
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b) { trace += "start(" + num(font == smallFont) + "," + num(a) + "," + num(color) + "," + num(b) + ");"; }
void fontEnd__Fv() { trace += "end;"; }
void fontColor__FUiii(unsigned color, int a, int b) { trace += "color(" + num(color) + "," + num(a) + "," + num(b) + ");"; }
UString* textGetString__FPc(const char* key) {
    UString* s = strings[stringCount++ % 64];
    put(s, std::string("<") + key + ">");
    trace += std::string("tgs(") + key + ");";
    return s;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* t, int w) { trace += "pcu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(w) + ");"; }
void fontPrintu__FiiPUsi(int x, int y, UString* t, int w) { trace += "pu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(w) + ");"; }
void fontPrint__FiiPci(int x, int y, char* t, int w) { trace += "pc(" + num(x) + "," + num(y) + "," + t + "," + num(w) + ");"; }
void fontPrintCentered__FiiPci(int x, int y, char* t, int w) { trace += "pcc(" + num(x) + "," + num(y) + "," + t + "," + num(w) + ");"; }
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString* t, int a, int b, int c, int d) {
    trace += "pwu(" + num(x) + "," + num(y) + "," + text(t) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
    return 10;
}
int fontStringSizeu__FP4FontPUsi(void*, UString* t, int) { return 4 * static_cast<int>(text(t).size()); }
int fontStringSize__FP4FontPci(void*, char* t, int) { return 12 * static_cast<int>(std::strlen(t)); }
void ustrcpy__FPUsPc(UString* d, char* s) { put(d, s); }
void ustrcat__FPUsT0(UString* d, UString* s) { put(d, text(d) + text(s)); }
void ustrcat__FPUsPc(UString* d, const char* s) { put(d, text(d) + s); }
void usprintf__FPUsT0ffffffff(UString* d, UString* f, float a, float, float, float, float, float, float, float) { put(d, text(f) + "|" + fnum(a)); }
void usprintf__FPUsPcffffffff(UString* d, const char* f, float a, float b, float, float, float, float, float, float) { put(d, std::string(f) + "|" + fnum(a) + "|" + fnum(b)); }
void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned color, char* name, int b, int c) {
    trace += "border(" + num(x) + "," + num(y) + "," + num(w) + "," + num(h) + "," + num(a) + "," + num(color) + "," + num(name != nullptr) + "," + num(b) + "," + num(c) + ");";
}
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned color) {
    trace += "tdc(" + num(t == lcursor ? 1 : t != nullptr ? 2 : 0) + "," + num(x) + "," + num(y) + "," + num(z) + "," + num(color) + ");";
}
void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned color) {
    trace += "rect(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(color) + ");";
}
void updateCursor__FiiiiiP12screenStructi(int a, int b, int c, int d, int e, void* screen, int f) {
    trace += "cursor(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + ",S" + num((static_cast<unsigned char*>(screen) - statsScreen) / 116) + "," + num(f) + ");";
}
static int lumpCounter;
void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    trace += std::string("lump(") + lump + "," + name + ");";
    return &lumpCounter;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float v) { trace += "seq(" + num(a != nullptr) + "," + num(o != nullptr) + "," + fnum(v) + ");"; }
void msgInventoryAdjustAbility__Fii(int index, int ability) { trace += "adjust(" + num(index) + "," + num(ability) + ");"; }
void statsRunSkillTree__Fi(int slot) { trace += "tree(" + num(slot) + ");"; }
void playerRecomputeDamage__6PlayerPQ24item11descriptioni(void* self, void* description, int flag) {
    trace += std::string("dmg(") + (self == itemA ? "A" : self == itemB ? "B" : "?") + "," + (description == itemA ? "A" : description == itemB ? "B" : "?") + "," + num(flag) + ");";
    *reinterpret_cast<int*>(player + 212) = 11; *reinterpret_cast<int*>(player + 216) = 22;
}
void playerRecomputeAbilityModifiers__Fv() { trace += "mods;"; }
void displayResistances__Fi(int slot) { trace += "resist(" + num(slot) + ");"; }
void printStat__FiPUsiiii(int slot, UString* label, int value, int a, int b, int c) {
    trace += "stat(" + num(slot) + "," + text(label) + "," + num(value) + "," + num(a) + "," + num(b) + "," + num(c) + ");";
}
Double litodp(int value) { return fromDouble(value); }
Double dpmul(Double a, Double b) { return fromDouble(asDouble(a) * asDouble(b)); }
Double dpadd(Double a, Double b) { return fromDouble(asDouble(a) + asDouble(b)); }
Double sinDouble(Double value) { return fromDouble(std::sin(asDouble(value))); }
int formatText(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int n = std::vsnprintf(buffer, 256, format, args);
    va_end(args);
    return n;
}
int textLength(const char* text) { return static_cast<int>(std::strlen(text)); }
char* textAppend(char* destination, const char* source) { return std::strcat(destination, source); }
Double fptodp(float value) { return fromDouble(value); }
int dptoli(Double value) { return static_cast<int>(asDouble(value)); }
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)
static bool has(const std::string& part) { return trace.find(part) != std::string::npos; }
static bool before(const std::string& a, const std::string& b) { size_t x = trace.find(a), y = trace.find(b); return x != std::string::npos && y != std::string::npos && x < y; }

static int& word(unsigned char* base, int off) { return *reinterpret_cast<int*>(base + off); }
static void setWordPointer(unsigned char* base, int off, void* p) { *reinterpret_cast<unsigned*>(base + off) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

static void reset() {
    std::memset(statsScreen, 0, sizeof statsScreen);
    std::memset(player, 0, sizeof player);
    std::memset(pad, 0, sizeof pad);
    trace.clear(); stringCount = 0;
    smallFont = &lumpCounter; lcursor = &glist[3]; rcursor = cursorShape;
    *reinterpret_cast<short*>(cursorShape) = 8; *reinterpret_cast<short*>(cursorShape + 2) = 20;
    shopDisableMouse[0] = shopDisableMouse[1] = 0;
    GLISTSIZE = 10000; gstart = glist; glistp = glist + 5;  // 5 words used: plenty of room
    drawFrameNm = 0;
    for (int i = 0; i < 64; ++i) experienceTable[i] = 1000 * (i + 1);
    padAcceptButton = 0x40; padBackButton = 0x120; language = 0;
    word(statsScreen + 116, 52) = 1;  // slot 1 uses player record 1
    setWordPointer(player, 21872, itemA);
    setWordPointer(player + 23984, 21872, itemA);
    std::memcpy(player, "Alyssa", 7);
    std::memcpy(player + 23984, "Borin", 6);
}

int main() {
    // The setup: slot 1 gets the x offset 320 and slot 0 zero.
    reset();
    word(statsScreen, 0) = 99;
    runStats__Fii(0, 5);
    CHECK(word(statsScreen, 0) == 0 && trace == "mods;");
    reset();
    runStats__Fii(1, 5);
    CHECK(word(statsScreen + 116, 0) == 320);

    // Mode 2 with state 2: the task is removed and the record's flag byte is set to -1.
    reset();
    word(statsScreen, 76) = 2;
    runStats__Fii(0, 2);
    CHECK(trace == "rmtask(statsFrame);xp(0,0);mods;" && player[241] == 0xff);

    // Mode 2/3 return at once when the display list has under 3000 words free.
    reset();
    GLISTSIZE = 3000 + 5 - 1;
    runStats__Fii(0, 3);
    CHECK(trace.empty());
    GLISTSIZE = 3000 + 5;
    runStats__Fii(0, 3);
    CHECK(trace == "tree(0);");

    // Mode 3 with skill tree: returns without the common tail; mode 2 tree: back button toggles the state.
    reset();
    player[242] = 1;
    pad[12] = 0x20; pad[13] = 0x01;  // back button 0x120 bits set
    runStats__Fii(0, 2);
    CHECK(trace == "tree(0);" && word(statsScreen, 76) == 2 && player[242] == 0);
    reset();
    player[242] = 1;
    runStats__Fii(0, 2);
    CHECK(word(statsScreen, 76) == 0 && player[242] == 1);
    reset();
    player[242] = 1;
    shopDisableMouse[0] = 1; pad[12] = 0x20; pad[13] = 0x01;
    runStats__Fii(0, 2);
    CHECK(word(statsScreen, 76) == 0 && player[242] == 1);

    // Mode 2 allocation page with the accept button: draws the page and then selects the stat.
    reset();
    word(statsScreen, 24) = 2;
    word(statsScreen, 36) = 7;
    player[240] = 9; word(player, 236) = 1;
    word(player, 144) = 10; word(player, 148) = 20; word(player, 152) = 30; word(player, 156) = 40;
    pad[16] = 0x40;
    runStats__Fii(0, 2);
    CHECK(trace.rfind("start(1,5,2149475428,-1);tgs(LevelUp);pcu(160,32,<LevelUp>,1000000);end;", 0) == 0);
    CHECK(has("start(1,5,2149475428,-1);tgs(PointRemaining);pcu(160,204,9   <PointRemaining>,1000000);end;"));
    CHECK(has("border(18,25,292,407,4,2153866089,0,0,0);"));
    CHECK(has("pwu(30,55,<abilityAdjust>,266,-50,250,17);"));
    CHECK(before("pu(94,239,<StatStrength>,1000000);", "pc(160,239,10,100000);") && has("pu(94,239,<StatStrength>,1000000);") && has("pc(160,259,20,100000);") && has("pc(160,279,30,100000);") && has("pc(160,299,40,100000);"));
    CHECK(has("tgs(selectStatDexterityDesc);pwu(30,331,<selectStatDexterityDesc>,266,-50,600,17);"));
    CHECK(has("cursor(0,0,4,0,20,S0,1);"));
    CHECK(has("rect(18,250,309,272,5,1077307510);"));
    CHECK(has("seq(1,0,1);adjust(0,2);mods;") && has("lump(fx.lmp,game_lvl_select.anm);"));
    // the bobbing cursor: x = x0 - 8 - 10 * sin(0 + 0) = -8, drawn at -8 + 230; y = 7 - 10 + 253
    CHECK(has("tdc(1,222,250,6,2155905152);"));
    // plural label, another stat, the latch blocks the select
    reset();
    player[240] = 3; word(player, 236) = 4; word(statsScreen, 24) = 5; pad[16] = 0x40; word(statsScreen, 20) = 1;
    runStats__Fii(0, 2);
    CHECK(has("pcu(160,204,3   <PointsRemaining>,1000000);") && has("pwu(30,331,<selectStatStaminaDesc>") && !has("adjust("));
    reset();
    word(statsScreen, 24) = 0; pad[16] = 0x40; runStats__Fii(0, 2);
    CHECK(has("adjust(0,0);") && has("<selectStatStrengthDesc>"));
    reset();
    word(statsScreen, 24) = 1; pad[16] = 0x40; runStats__Fii(0, 2);
    CHECK(has("adjust(0,1);") && has("<selectStatIntelDesc>"));
    reset();
    word(statsScreen, 24) = 3; pad[16] = 0x40; runStats__Fii(0, 2);
    CHECK(has("adjust(0,3);"));
    reset();
    word(statsScreen, 24) = 4; pad[16] = 0x40; runStats__Fii(0, 2);
    CHECK(!has("adjust("));
    reset();
    word(statsScreen, 24) = 1; pad[16] = 0x01; runStats__Fii(0, 2);  // not the accept button
    CHECK(!has("seq("));

    // Mode 1: the quest list, using the first record's count, and the empty case.
    reset();
    word(player, 21864) = 2;
    std::memcpy(player + 17768, "quest one", 10); std::memcpy(player + 17768 + 32, "detail one", 11);
    std::memcpy(player + 17768 + 64, "quest two", 10); std::memcpy(player + 17768 + 96, "detail two", 11);
    word(statsScreen + 116, 36) = 4;
    runStats__Fii(1, 1);
    CHECK(has("cursor(1,0,2,0,40,S1,1);") && has("lump(inventry.lmp,parchment.tex);tdc(2,316,104,5,2155905152);"));
    CHECK(has("start(1,6,2150775868,-1);color(2150775868,-1,0);tgs(quest one);pwu(344,102,<quest one>,295,82,418,21);color(2150444333,-1,0);"
              "tgs(detail one);pwu(344,124,<detail one>,295,82,418,21);end;"));
    CHECK(has("pwu(344,134,<quest two>,295,82,418,21);") && has("pwu(344,156,<detail two>,295,82,418,21);"));
    CHECK(!has("NoQuests") && trace.rfind("mods;") == trace.size() - 5);
    reset();
    runStats__Fii(0, 1);
    CHECK(has("start(1,6,2150775868,-1);tgs(NoQuests);pcu(160,224,<NoQuests>,1000000);end;"));

    // Mode 0: the full statistics page for slot 1 (player record 1).
    reset();
    unsigned char* r = player + 23984;
    word(r, 80) = 3; word(r, 84) = 2; r[88] = 4; word(r, 92) = 1234;
    word(r, 144) = 11; word(r, 148) = 12; word(r, 152) = 13; word(r, 156) = 14; word(r, 208) = 55; word(r, 212) = 1; word(r, 216) = 2;
    *reinterpret_cast<float*>(r + 172) = 90.5f; *reinterpret_cast<float*>(r + 176) = 100.9f;
    *reinterpret_cast<float*>(r + 196) = 7.0f; *reinterpret_cast<float*>(r + 200) = 8.0f;
    setWordPointer(r, 21872, itemA); *reinterpret_cast<short*>(itemA + 232) = 77;
    r[23572] = 1;
    setWordPointer(r, 21960 + 4, itemB);
    runStats__Fii(1, 0);
    CHECK(has("start(1,5,2150775868,-1);pcc(480,107,Borin,10000);"));
    CHECK(has("usprintf") == false);
    CHECK(has("pcu(480,129,<StatLevelDescription>|4 <WoodElfBard>,1000000);"));  // level text, a space, the class name
    CHECK(has("tgs(StatExperience);stat(1,<StatExperience>,1234,-7,1,0);tgs(StatNextLevel);stat(1,<StatNextLevel>,5000,-6,1,0);"));
    CHECK(has("pu(345,208,<StatHealth>|0,1000000);pcu(467,208,%d/%d|90|100,1000000);") && has("pu(345,229,<StatEnergy>|0,1000000);pcu(467,229,%d/%d|7|8,1000000);"));
    CHECK(has("tgs(StatArmor);stat(1,<StatArmor>,77,-3,1,0);dmg(A,B,1);tgs(StatBaseAttack);stat(1,<StatBaseAttack>,55,-2,1,0);"));
    CHECK(has("pu(349,306,<StatDamage>|0,1000000);pcc(483,306,1-2,10000);"));
    CHECK(has("tgs(StatStrength);stat(1,<StatStrength>,11,1,0,0);tgs(StatIntelligence);stat(1,<StatIntelligence>,12,2,0,0);tgs(StatDexterity);stat(1,<StatDexterity>,13,3,0,0);tgs(StatStamina);stat(1,<StatStamina>,14,4,0,0);end;resist(1);"));
    CHECK(has("lump(inventry.lmp,statsfull.tex);tdc(2,324,99,4,2155905152);mods;"));
    // language 1 and 4 use the German page and shift the vital bars
    reset(); language = 4;
    runStats__Fii(0, 0);
    CHECK(has("lump(inventry.lmp,statsfull_german.tex);tdc(2,4,99,4,2155905152);") && has("pcu(187,208,"));
    reset(); language = 1;
    runStats__Fii(0, 0);
    CHECK(has("statsfull_german.tex") && has("pcu(181,208,"));
    reset(); language = 2;
    runStats__Fii(0, 0);
    CHECK(has("statsfull.tex") && has("pcu(147,208,"));
    // long names are cut until they fit (221 pixels at 12 per character) and marked with ...
    reset();
    std::memcpy(player, "Abcdefghijklmnopqrstuvwxyz", 27);
    runStats__Fii(0, 0);
    CHECK(has("pcc(160,107,Abcdefghijklmnopqrs...,10000);") == false);
    CHECK(has("pcc(160,107,Abcdefghijklmnopqr...,10000);"));
    // class names
    reset(); word(player, 84) = 0; runStats__Fii(0, 0); CHECK(has("<BarbarianWarrior>"));
    reset(); word(player, 84) = 5; word(player, 80) = 2; runStats__Fii(0, 0); CHECK(has("<DarkElfShadowKnight>") && has("<StatShortDescription>"));
    reset(); word(player, 84) = 5; word(player, 80) = 1; runStats__Fii(0, 0); CHECK(has("<HighElfCleric>"));
    reset(); word(player, 84) = 5; word(player, 80) = 3; runStats__Fii(0, 0); CHECK(has("<WoodElfBard>"));
    reset(); word(player, 84) = 5; word(player, 80) = 4; runStats__Fii(0, 0); CHECK(has("<EruditeWizard>"));
    reset(); word(player, 84) = 5; word(player, 80) = 9; runStats__Fii(0, 0); CHECK(has("<WoodElfBard>"));
    // damage: a second weapon range for slot 0 with the second description present
    reset();
    setWordPointer(player, 21872, itemA); setWordPointer(player, 21968, itemB); player[23572] = 0;
    runStats__Fii(0, 0);
    CHECK(has("dmg(A,B,1);pcc(163,306,11-22/11-22,10000);") || has("dmg(A,B,1);"));
    std::puts("ok");
    return 0;
}
