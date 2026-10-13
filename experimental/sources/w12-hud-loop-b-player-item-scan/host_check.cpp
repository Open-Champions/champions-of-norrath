// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// Objects are byte arrays with the observed offsets. The 32-bit pointer words (world object word,
// vtable word, vtable function words) are handles that hostWordAddress maps. Callees are stubs that
// append to a trace.
#include "player_item_scan.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
alignas(8) unsigned char player[4 * 23984];
int nmPlayers, activePlayer, pokeMessageAlpha[4], pokeItemMagic[4];
signed char netNetworkMode;
unsigned actionButton[4];
void* mediumFont = reinterpret_cast<void*>(0x30);
void* smallFont = reinterpret_cast<void*>(0x31);
}
struct Blob { alignas(8) unsigned char b[1400]; };
static Blob self, world, pad;
static std::vector<Blob*> candidates;
static std::string trace;
static int kindAnswer, wrapHeight, widthPerChar = 10;
static std::vector<unsigned short> textAnswer;
static void* typeToken;
static const int kTokenItem = 1, kTokenPh = 2;
static std::string text(const unsigned short* t) {
    std::string s;
    for (; *t; ++t) s += *t < 128 ? std::string(1, static_cast<char>(*t)) : "<" + std::to_string(*t) + ">";
    return s;
}
static void put(Blob& b, int off, const void* p, int n) { std::memcpy(b.b + off, p, n); }
extern "C" {
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** out, int max) {
    trace += "box(" + std::to_string((int)lo.x) + "," + std::to_string((int)lo.y) + "," + std::to_string((int)lo.z) + ";" + std::to_string((int)hi.x) + "," +
             std::to_string((int)hi.y) + "," + std::to_string((int)hi.z) + ";" + std::to_string(max) + ");";
    for (size_t i = 0; i < candidates.size(); ++i) out[i] = candidates[i]->b;
    out[candidates.size()] = nullptr;
    return 0;
}
void __throw_bad_typeid() { trace += "throw;"; }
void* __tf8ItemProp() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(kTokenItem)); }
void* __tf6PhProp() { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(kTokenPh)); }
int __eq__C9type_infoRC9type_info(const void* a, const void* b) { return a == b; }
int gameButtonToIcon__Fi(int b) { trace += "icon" + std::to_string(b) + ";"; return 0x300 + b; }
void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from) { trace += "cpy;"; unsigned short* p = to; while ((*p++ = *from++) != 0) {} }
void ustrcat__FPUsT0(unsigned short* to, const unsigned short* from) { trace += "cat;"; while (*to) ++to; while ((*to++ = *from++) != 0) {} }
int ustrlen__FPUs(const unsigned short* t) { int n = 0; while (t[n]) ++n; return n; }
int fontStringSizeu__FP4FontPUsi(void* f, unsigned short* t, int limit) {
    trace += std::string("size") + (f == mediumFont ? "M" : f == smallFont ? "S" : "?") + std::to_string(limit) + ";";
    return widthPerChar * ustrlen__FPUs(t);
}
void fontStart__FP4FontiUii(void*, int size, unsigned color, int flags) { char b[60]; std::snprintf(b, sizeof b, "start%d,%08x,%d;", size, color, flags); trace += b; }
void fontEnd__Fv() { trace += "end;"; }
void fontColor__FUiii(unsigned c, int a, int b) { char x[60]; std::snprintf(x, sizeof x, "color%08x,%d,%d;", c, a, b); trace += x; }
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int limit) { trace += "centered" + std::to_string(x) + "," + std::to_string(y) + "," + text(t) + "," + std::to_string(limit) + ";"; }
int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int a, int b, int c, int d) {
    trace += "wrap" + std::to_string(x) + "," + std::to_string(y) + "," + text(t) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ";";
    return wrapHeight;
}
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned c, int b, int d, int e) {
    char z[100]; std::snprintf(z, sizeof z, "border%d,%d,%d,%d,%d,%08x,%d,%d,%d;", x, y, w, h, a, c, b, d, e); trace += z;
}
}
static int candidateOf(void* selfp, int delta) {
    for (size_t i = 0; i < candidates.size(); ++i)
        if (static_cast<unsigned char*>(selfp) - delta == candidates[i]->b) return static_cast<int>(i);
    return -1;
}
static int msgFn(void* selfp, unsigned short* buffer, int idx) {
    trace += "msg(" + std::to_string(candidateOf(selfp, 8)) + "," + std::to_string(idx) + ");";
    for (size_t i = 0; i < textAnswer.size(); ++i) buffer[i] = textAnswer[i];
    buffer[textAnswer.size()] = 0;
    return kindAnswer;
}
static void* typeFn() { return typeToken; }

static void useFn(void* selfp, int idx) { trace += "use(" + std::to_string(candidateOf(selfp, 16)) + "," + std::to_string(idx) + "," + std::to_string(activePlayer) + ");"; }

static unsigned char vtable[400];
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    switch (word) {
        case 0x100: return reinterpret_cast<__UINTPTR_TYPE__>(world.b);
        case 0x200: return reinterpret_cast<__UINTPTR_TYPE__>(vtable);
        case 0x301: return reinterpret_cast<__UINTPTR_TYPE__>(&typeFn);
        case 0x302: return reinterpret_cast<__UINTPTR_TYPE__>(&msgFn);
        case 0x303: return reinterpret_cast<__UINTPTR_TYPE__>(&useFn);
    }
    return 0;
}
static int bad;
static void expect(const char* what, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("FAIL %s\n got  %s\n want %s\n", what, got.c_str(), want.c_str()); bad = 1; }
}
static Blob* makeCandidate(unsigned flags, float x, float y, float z) {
    Blob* c = new Blob();
    std::memset(c->b, 0, sizeof c->b);
    put(*c, 12, &flags, 4); put(*c, 16, &x, 4); put(*c, 20, &y, 4); put(*c, 24, &z, 4);
    const unsigned vtableWord = 0x200;
    put(*c, 72, &vtableWord, 4);
    std::memset(vtable, 0, sizeof vtable);
    const short typeDelta = 0, messageDelta = 8, useDelta = 16;
    const unsigned typeWord = 0x301, messageWord = 0x302, useWord = 0x303;
    std::memcpy(vtable, &typeDelta, 2); std::memcpy(vtable + 4, &typeWord, 4);
    std::memcpy(vtable + 104, &messageDelta, 2); std::memcpy(vtable + 108, &messageWord, 4);
    std::memcpy(vtable + 112, &useDelta, 2); std::memcpy(vtable + 116, &useWord, 4);
    return c;
}
static void reset(int idx) {
    std::memset(self.b, 0, sizeof self.b); std::memset(player, 0, sizeof player); std::memset(world.b, 0, sizeof world.b); std::memset(pad.b, 0, sizeof pad.b);
    put(self, 960, &idx, 4);
    float hp = 50.0f; std::memcpy(player + idx * 23984 + 172, &hp, 4);
    unsigned w = 0x100; std::memcpy(player + idx * 23984 + 21872, &w, 4);
    float pos[3] = {10.0f, 20.0f, 30.0f}; put(world, 16, pos, 12);
    candidates.clear(); trace.clear(); textAnswer.clear(); kindAnswer = 0; wrapHeight = 33; widthPerChar = 10; typeToken = nullptr;
    for (int i = 0; i < 4; ++i) { pokeMessageAlpha[i] = pokeItemMagic[i] = 0; actionButton[i] = 1u << i; }
    activePlayer = 99; nmPlayers = 1; netNetworkMode = 0;
    for (auto& m : norrath_player_item_scan_message) std::memset(m, 0, sizeof m);
}
static void run(int flag) { playerItemScan__6PlayerP3padi(self.b, pad.b, flag); }
static std::string state(int idx) { return std::to_string(pokeMessageAlpha[idx]) + "," + std::to_string(pokeItemMagic[idx]) + "," + text(norrath_player_item_scan_message[idx]); }
static std::vector<unsigned short> chars(const char* s) { std::vector<unsigned short> v; for (; *s; ++s) v.push_back(static_cast<unsigned char>(*s)); return v; }

static std::string icon(int n) { return "<" + std::to_string(0x300 + n) + ">"; }
int main() {
    // Early exits.
    reset(1); { int w = 5; put(self, 1328, &w, 4); } run(0); expect("word 1328", trace, "");
    reset(1); { float hp = 0.0f; std::memcpy(player + 23984 + 172, &hp, 4); } run(0); expect("no health", trace, "");
    reset(1); { unsigned n = 0; std::memcpy(player + 23984 + 21872, &n, 4); } run(0); expect("no world", trace, "");
    // No candidates: the box is made around P; a candidate without bit 0x100000 is ignored.
    reset(1); run(0); expect("empty", trace, "box(-38,-28,30;58,68,90;256);");
    reset(1); candidates = {makeCandidate(0x1, 10, 20, 30)}; run(0); expect("flag bit missing", trace, "box(-38,-28,30;58,68,90;256);");
    // Alpha falls by 7 with no message; at zero or below everything is cleared; flag 1 never changes it.
    reset(1); pokeMessageAlpha[1] = 20; pokeItemMagic[1] = 2; norrath_player_item_scan_message[1][0] = 'x'; run(0);
    expect("decay", state(1), "13,2,x");
    reset(1); pokeMessageAlpha[1] = 7; pokeItemMagic[1] = 2; norrath_player_item_scan_message[1][0] = 'x'; norrath_player_item_scan_message[1][1] = 'y'; run(0);
    expect("decay clears", state(1), "0,0,");
    reset(1); pokeMessageAlpha[1] = 20; run(1); expect("flag one keeps alpha", std::to_string(pokeMessageAlpha[1]), "20");
    // The nearest of several flagged candidates is asked for its message.
    reset(2); candidates = {makeCandidate(0x100000, 30, 20, 30), makeCandidate(0x100000, 12, 20, 30), makeCandidate(0x100000, 10, 20, 33)};
    textAnswer = chars("hi"); run(0);
    expect("nearest", trace.substr(trace.find("msg")), "msg(" + std::string("1") + ",2);cpy;sizeM1000000;");
    expect("kind 0 state", state(2), "7,0,hi");
    // Alpha rises by 7 up to 128.
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; textAnswer = chars("hello"); pokeMessageAlpha[2] = 125; run(0);
    expect("alpha cap", state(2), "128,0,hello");
    // Kind 1: action-button icon first. Item prop with byte 361 != 255 -> magic 1; PhProp with 255 and the bit -> 2; with 255 and no bit -> 0.
    reset(0); actionButton[0] = 5; candidates = {makeCandidate(0x100000, 10, 20, 30)}; candidates[0]->b[361] = 7;
    typeToken = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(kTokenItem)); kindAnswer = 1; textAnswer = chars("take"); run(0);
    expect("kind 1 state", state(0), "7,1," + icon(5) + "take");
    reset(0); candidates = {makeCandidate(0x100000, 10, 20, 30)}; candidates[0]->b[361] = 255;
    { unsigned long long bit = 0x2000000000ull; std::memcpy(candidates[0]->b + 336, &bit, 8); }
    typeToken = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(kTokenPh)); kindAnswer = 1; textAnswer = chars("take"); run(0);
    expect("ph magic 2", std::to_string(pokeItemMagic[0]), "2");
    expect("ph trace", trace.substr(trace.find("msg")), "msg(0,0);icon1;cat;sizeM1000000;");
    reset(0); candidates = {makeCandidate(0x100000, 10, 20, 30)}; candidates[0]->b[361] = 255;
    typeToken = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(kTokenPh)); kindAnswer = 1; textAnswer = chars("take"); pokeItemMagic[0] = 2; run(0);
    expect("ph magic 0", std::to_string(pokeItemMagic[0]), "0");
    // Other types leave the magic at zero; kind 2 and 3 use the icon of button 16.
    reset(0); candidates = {makeCandidate(0x100000, 10, 20, 30)}; typeToken = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(77));
    kindAnswer = 2; textAnswer = chars("open"); pokeItemMagic[0] = 2; run(0);
    expect("other type", state(0), "7,0," + icon(16) + "open");
    // A single space message builds nothing but keeps the kind for the action step; an empty one skips it.
    reset(0); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 1; textAnswer = {32}; pad.b[12] = 1; actionButton[0] = 1; run(0);
    expect("lone space", trace.substr(trace.find("msg")), "msg(0,0);use(0,0,0);");
    expect("lone space state", std::to_string(activePlayer), "0");
    reset(0); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 1; textAnswer = {}; pad.b[12] = 1; actionButton[0] = 1; run(0);
    expect("empty message", trace.substr(trace.find("msg")), "msg(0,0);");
    // Too long: 56 characters at width 10 is 560; one character is cut, then the last four become "...".
    reset(3); candidates = {makeCandidate(0x100000, 10, 20, 30)}; { std::string s(56, 'a'); for (int i = 0; i < 56; ++i) s[i] = static_cast<char>('a' + i % 26); textAnswer = chars(s.c_str()); } run(0);
    expect("trim", state(3), "7,0," + std::string("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy").substr(0, 51) + "...");
    reset(3); candidates = {makeCandidate(0x100000, 10, 20, 30)}; textAnswer = chars(std::string(55, 'b').c_str()); run(0);
    expect("no trim at 550", state(3), "7,0," + std::string(55, 'b'));
    reset(3); candidates = {makeCandidate(0x100000, 10, 20, 30)}; widthPerChar = 0; textAnswer = chars("short"); run(0);
    expect("zero width", state(3), "7,0,short");
    // Action step: kind 1 needs the player's action button bit; kind 2 needs bit 0x10; neither fires with flag 1.
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 1; textAnswer = chars("go"); pad.b[12] = 4; actionButton[2] = 4; run(0);
    expect("use kind 1", trace.substr(trace.find("use")), "use(0,2,2);");
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 1; textAnswer = chars("go"); pad.b[12] = 0x10; actionButton[2] = 4; run(0);
    expect("kind 1 wrong button", std::to_string(trace.find("use") == std::string::npos) + std::to_string(activePlayer), "199");
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 2; textAnswer = chars("go"); pad.b[12] = 0x10; run(0);
    expect("use kind 2", trace.substr(trace.find("use")), "use(0,2,2);");
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 2; textAnswer = chars("go"); pad.b[12] = 0x20; run(0);
    expect("kind 2 wrong button", std::to_string(trace.find("use") == std::string::npos), "1");
    reset(2); candidates = {makeCandidate(0x100000, 10, 20, 30)}; kindAnswer = 2; textAnswer = chars("go"); pad.b[12] = 0x10; run(1);
    expect("flag one no use", std::to_string(trace.find("use") == std::string::npos), "1");
    // Drawing (flag 1): single player, centred text with a border.
    reset(1); pokeMessageAlpha[1] = 100; { auto t = chars("abc"); for (size_t i = 0; i < t.size(); ++i) norrath_player_item_scan_message[1][i] = t[i]; } run(1);
    expect("draw single", trace.substr(trace.find("start")),
           "start7,64464646,-1;sizeS1000000;centered320,380,abc,1000000;end;border285,379,70,33,7,32000000,0,0,0;");
    reset(1); pokeMessageAlpha[1] = 101; pokeItemMagic[1] = 1; norrath_player_item_scan_message[1][0] = 'a'; run(1);
    expect("draw magic 1", trace.substr(trace.find("color")), "color655c2913,-1,0;centered320,380,a,1000000;end;border295,379,50,33,7,32000000,0,0,0;");
    reset(1); pokeMessageAlpha[1] = 101; pokeItemMagic[1] = 2; norrath_player_item_scan_message[1][0] = 'a'; run(1);
    expect("draw magic 2", trace.substr(trace.find("color")).substr(0, 21), "color65004444,-1,0;ce");
    reset(1); netNetworkMode = 1; nmPlayers = 3; pokeMessageAlpha[1] = 100; norrath_player_item_scan_message[1][0] = 'a'; run(1);
    expect("draw networked", std::to_string(trace.find("centered") != std::string::npos), "1");
    // Split screen: x from the table entry 4 * nmPlayers + idx - 8, wrapped text above y 422.
    reset(2); nmPlayers = 3; pokeMessageAlpha[2] = 100; norrath_player_item_scan_message[2][0] = 'a'; wrapHeight = 33; run(1);
    expect("draw split", trace.substr(trace.find("start")),
           "start7,64464646,-1;wrap238,422,a,110,-100,-200,17;wrap238,389,a,110,-100,580,17;end;border168,387,140,47,7,32000000,0,0,0;");
    reset(0); nmPlayers = 2; pokeMessageAlpha[0] = 100; norrath_player_item_scan_message[0][0] = 'a'; wrapHeight = 10; run(1);
    expect("draw split small", trace.substr(trace.find("wrap")), "wrap88,422,a,110,-100,-200,17;wrap88,412,a,110,-100,580,17;end;border18,410,140,32,7,32000000,0,0,0;");
    reset(3); nmPlayers = 4; pokeMessageAlpha[3] = 100; norrath_player_item_scan_message[3][0] = 'a'; run(1);
    expect("draw split four", trace.substr(trace.find("wrap"), 20), "wrap396,422,a,110,-1");
    // No alpha or no health: nothing is drawn.
    reset(1); norrath_player_item_scan_message[1][0] = 'a'; run(1); expect("alpha zero", std::to_string(trace.find("start") == std::string::npos), "1");
    reset(1); pokeMessageAlpha[1] = 50; norrath_player_item_scan_message[1][0] = 'a'; run(0); expect("flag zero no draw", std::to_string(trace.find("start") == std::string::npos), "1");
    std::printf(bad ? "FAIL\n" : "ok: player item scan\n");
    return bad;
}
