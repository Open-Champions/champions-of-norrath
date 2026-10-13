// Host check for drawNewestQuickSpellMenu__Fiiicl. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin host_check.cpp
// The image's 32-bit pointer words are offsets into one host arena. Every callee appends to a
// trace. The trig stubs return angle / 65536 and angle / 131072 and the easing stub a settable value,
// so every position is a hand-computable integer; the expected traces are built by small helpers
// that spell out the header rules.
#define NORRATH_HOST_ARENA
#include "draw_newest_quick_spell_menu.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

static unsigned char arena[1 << 20];
extern "C" {
unsigned char* norrath_host_base = arena;
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char netLocalPad[4 * 88];
int gameControlPaused, gamePause;
signed char interfaceOn, debugPauseMenuOn, hudNetWaiting, hudBustCovered;
int nmPlayers, drawFrameNm;
int fontObject;
void* smallFont = &fontObject;
void* gameCam;
unsigned equipButton;
int hudQuickSpellOn[4];
unsigned spellButton1[4], spellButton2[4];
int norrath_quick_spell_alpha[4] __asm__("a.1122");
int norrath_quick_spell_box[4] __asm__("curBoxOld");
alignas(8) unsigned char spellBook[16];
alignas(8) unsigned char emptySpell[16];
unsigned globalIcons[38];
unsigned skills[63];
}

static std::string trace;
static std::string num(long v) { return std::to_string(v); }
static std::string hex(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%x", v); return b; }
static int localAnswers[4], localCalls;
static int dlgAnswer, busyAnswer, taskAnswer;
static float hermiteValue;
static std::vector<std::string> foundNames;

extern "C" int netPlayerIsLocal__Fi(int pn) { trace += "local" + num(pn) + ";"; return localAnswers[localCalls++ < 3 ? localCalls - 1 : 3]; }
extern "C" int DLG_Active__Fv() { trace += "dlg;"; return dlgAnswer; }
extern "C" int MC_Busy__Fv() { trace += "busy;"; return busyAnswer; }
extern "C" int engineTaskActive__FPc(const char* n) { trace += std::string("task ") + n + ";"; return taskAnswer; }
extern "C" void netBlankControllerButtons__Fiii(int pn, int a, unsigned b) { trace += "blank(" + num(pn) + "," + num(a) + "," + hex(b) + ");"; }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    trace += std::string("find ") + lump + " " + name + ";";
    foundNames.push_back(name);
    return reinterpret_cast<void*>(0x9000 + foundNames.size());
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* seq, void* obj, float volume) {
    trace += "snd(" + hex(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(seq))) + "," + num(obj != nullptr) + "," + num(static_cast<int>(volume * 10)) + ");";
}
extern "C" int sprintf(char* out, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    std::string f = format, r;
    if (f == "%d" || f == "spellbookarrow%d.tex") {
        const int v = __builtin_va_arg(ap, int);
        r = f == "%d" ? num(v) : "spellbookarrow" + num(v) + ".tex";
    } else if (f == "%c") {
        r = std::string(1, static_cast<char>(__builtin_va_arg(ap, int)));
    } else if (f == "%s") {
        r = __builtin_va_arg(ap, const char*);
    }
    __builtin_va_end(ap);
    std::strcpy(out, r.c_str());
    trace += "fmt(" + f + "," + r + ");";
    return static_cast<int>(r.size());
}
extern "C" void textureDrawColor__FP8_textureiiiUi(void* tex, int x, int y, int mode, unsigned color) {
    trace += "tex(" + hex(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(tex))) + "," + num(x) + "," + num(y) + "," + num(mode) + "," + hex(color) + ");";
}
extern "C" void* get__11ResourceMap(void* map) {
    trace += map == spellBook ? "get(book);" : map == emptySpell ? "get(empty);" : "get(?);";
    return reinterpret_cast<void*>(map == spellBook ? 0x5001 : 0x5002);
}
extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    trace += "herm(" + num(static_cast<long>(t * 1000.0f + 0.5f)) + "," + num(static_cast<long>(a)) + "," + num(static_cast<long>(b)) + "," + num(static_cast<long>(c)) + "," + num(static_cast<long>(d)) + ");";
    return hermiteValue;
}
extern "C" float icos__Fi(int angle) { return static_cast<float>(angle) / 65536.0f; }
extern "C" float isin__Fi(int angle) { return static_cast<float>(angle) / 131072.0f; }
extern "C" void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    trace += std::string("fstart") + (font == smallFont ? "S" : "?") + num(size) + "," + hex(color) + "," + num(flags) + ";";
}
extern "C" void fontEnd__Fv() { trace += "fend;"; }
extern "C" void fontPrint__FiiPci(int x, int y, char* t, int limit) { trace += "fprint(" + num(x) + "," + num(y) + "," + t + "," + num(limit) + ");"; }
extern "C" int hudGetRemappedButtonIcon__Fii(int pn, int button) { trace += "icon(" + num(pn) + "," + num(button) + ");"; return 'A' + button; }
extern "C" void msgHudAssignQuickSpell__Fiii(int pn, int b, int slot) { trace += "quick(" + num(pn) + "," + num(b) + "," + num(slot) + ");"; }
extern "C" void msgHudAssignSpellBookSpell__Fiiil(int pn, int slot, int kind, long index) { trace += "book(" + num(pn) + "," + num(slot) + "," + num(kind) + "," + num(index) + ");"; }
extern "C" unsigned short* textGetString__FPc(const char* name) {
    trace += std::string("text(") + name + ");";
    return reinterpret_cast<unsigned short*>(0x8000 + std::strlen(name));
}
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int limit) {
    trace += "fpc(" + num(x) + "," + num(y) + "," + hex(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(t))) + "," + num(limit) + ");";
}
extern "C" unsigned norrath_host_call_icon(unsigned functionWord, unsigned char* object) {
    trace += "skillicon(" + hex(functionWord) + "," + num((object - (arena + 0x10000)) / 0x100) + ");";
    return 0x7000 + static_cast<unsigned>((object - (arena + 0x10000)) / 0x100);
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

template <class T> static void put(unsigned char* base, int off, T v) { std::memcpy(base + off, &v, sizeof v); }
template <class T> static T get(const unsigned char* base, int off) { T v; std::memcpy(&v, base + off, sizeof v); return v; }

struct Setup {
    int pn = 1, half = 0, flag = 0, kind = 0;
    long index = 0;
    unsigned held = 0;                 // netLocalPad word +8
    unsigned pressed = 0;              // netLocalPad word +12
    int alpha = 100;
    float timer = 30.0f;
    int step = 4;
    int box = -1;
    int slotSpell[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    bool slotNew[8] = {};
    unsigned button1 = 4, button2 = 8, equip = 0x20;
    unsigned char assigned[2] = {0xff, 0xff};
    int interface = 1, paused = 0, pause = 0, debug = 0, netWaiting = 0, camMode = 1, players = 1;
    int dlg = 0, busy = 0, task = 0;
    bool local[4] = {true, true, true, true};
    float hermite = 1.0f;
};

static void prepare(const Setup& s) {
    std::memset(arena, 0, 0x20000);
    std::memset(player, 0, sizeof player);
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    unsigned char* rec = player + 23984 * s.pn;
    for (int i = 0; i < 8; ++i) {
        std::strcpy(reinterpret_cast<char*>(rec + 21976 + 104 * i), s.slotNew[i] ? "newstyle" : "old");
        put<short>(rec + 22000 + 104 * i, 8, static_cast<short>(s.slotSpell[i]));
    }
    put<float>(rec, 23564, s.timer);
    put<int>(rec, 23568, s.step);
    rec[23580] = s.assigned[0]; rec[23581] = s.assigned[1];
    put<unsigned>(netLocalPad + 88 * s.pn, 8, s.held);
    put<unsigned>(netLocalPad + 88 * s.pn, 12, s.pressed);
    norrath_quick_spell_alpha[s.pn] = s.alpha;
    norrath_quick_spell_box[s.pn] = s.box;
    spellButton1[s.pn] = s.button1; spellButton2[s.pn] = s.button2; equipButton = s.equip;
    interfaceOn = static_cast<signed char>(s.interface); gameControlPaused = s.paused; gamePause = s.pause;
    debugPauseMenuOn = static_cast<signed char>(s.debug); hudNetWaiting = static_cast<signed char>(s.netWaiting); hudBustCovered = 0;
    nmPlayers = s.players; drawFrameNm = 7;
    unsigned char* cam = arena + 0x1000;
    put<int>(cam, 52, s.camMode);
    gameCam = cam;
    for (int i = 0; i < 38; ++i) globalIcons[i] = (i % 5 == 4) ? 0 : 0x6000 + i;
    for (int i = 0; i < 63; ++i) {
        unsigned char* skillObject = arena + 0x10000 + 0x100 * i;
        const unsigned objectWord = static_cast<unsigned>(skillObject - arena);
        skills[i] = objectWord;
        put<unsigned>(skillObject, 0, 0x2000 + 16 * i);       // name pointer word of the skill
        put<unsigned>(skillObject, 96, 0x4000);               // vtable word
    }
    put<short>(arena + 0x4000, 64, 4);
    put<unsigned>(arena + 0x4000, 68, 0xabc);                 // the function word
    for (int i = 0; i < 63; ++i) std::snprintf(reinterpret_cast<char*>(arena + 0x2000 + 16 * i), 16, "sk%d", i);
    for (int k = 0; k < 4; ++k) hudQuickSpellOn[k] = 9;
    localCalls = 0;
    for (int k = 0; k < 4; ++k) localAnswers[k] = s.local[k] ? 1 : 0;
    dlgAnswer = s.dlg; busyAnswer = s.busy; taskAnswer = s.task;
    hermiteValue = s.hermite;
    foundNames.clear();
    trace.clear();
}

static void run(const Setup& s) {
    drawNewestQuickSpellMenu__Fiiicl(s.pn, s.half, s.flag, static_cast<char>(s.kind), s.index);
}

int main() {
    // 1. a non-local player: nothing else happens
    {
        Setup s; s.local[0] = s.local[1] = s.local[2] = s.local[3] = false;
        prepare(s); run(s);
        CHECK(trace == "local1;" && hudQuickSpellOn[1] == 9 && norrath_quick_spell_alpha[1] == 100);
    }
    // 2. the input gate: any idle check failing leaves only interfaceOn; the check order is fixed
    {
        Setup s; s.held = 0x2000; s.interface = 0;
        prepare(s); run(s);
        CHECK(trace.rfind("local1;dlg;busy;task jumpgate;blank(1,3,c);", 0) == 0);
        struct Gate { const char* what; int Setup::*field; int value; std::string prefix; } gates[] = {
            {"paused", &Setup::paused, 1, "local1;"}, {"dlg", &Setup::dlg, 1, "local1;dlg;"}, {"cam", &Setup::camMode, 2, "local1;dlg;"},
            {"pause", &Setup::pause, 1, "local1;dlg;"}, {"debug", &Setup::debug, 1, "local1;dlg;"}, {"busy", &Setup::busy, 1, "local1;dlg;busy;"},
            {"task", &Setup::task, 1, "local1;dlg;busy;task jumpgate;"}, {"netwait", &Setup::netWaiting, 1, "local1;dlg;busy;task jumpgate;"}};
        for (const Gate& g : gates) {
            Setup t; t.held = 0x2000; t.interface = 0; t.alpha = 100; t.*(g.field) = g.value;
            prepare(t); run(t);
            if (trace.rfind(g.prefix, 0) != 0 || trace.find("blank(") != std::string::npos) { std::printf("gate %s: %s\n", g.what, trace.c_str()); return 1; }
            // interfaceOn forces the input back on
            t.interface = 1;
            prepare(t); run(t);
            if (trace.find("blank(1,3,c);") == std::string::npos) { std::printf("gate %s with interface: %s\n", g.what, trace.c_str()); return 1; }
        }
    }
    // 3. directions to the selected slot, and the changed-selection sound
    {
        struct Dir { unsigned bits; int sel; } dirs[] = {
            {0x1000, 0}, {0x2000 | 0x1000, 1}, {0x2000, 2}, {0x2000 | 0x4000, 3}, {0x4000, 4},
            {0x8000 | 0x4000, 5}, {0x8000, 6}, {0x8000 | 0x1000, 7}, {0, -1}};
        for (const Dir& d : dirs) {
            Setup s; s.held = d.bits; s.box = d.sel == 3 ? 3 : -1;
            prepare(s); run(s);
            CHECK(norrath_quick_spell_box[s.pn] == d.sel);
            if (d.sel != -1) {
                const std::string arrow = "find hud.lmp spellbookarrow" + num(d.sel) + ".tex;tex(";
                CHECK(trace.find(arrow) != std::string::npos);
            } else {
                CHECK(trace.find("find hud.lmp spellbookarrow") == std::string::npos && trace.find("fmt(spellbookarrow%d.tex,spellbookarrow-1.tex);") != std::string::npos);
            }
            const bool changed = (d.sel == 3 ? 3 : -1) != d.sel;
            CHECK((trace.find("find fx.lmp menu_spell_high.anm;snd(") != std::string::npos) == changed);
            CHECK((trace.find("blank(1,3,c);") != std::string::npos) == (d.bits != 0));
        }
        // opposite bits: the later test wins (0x8000 over 0x2000, 0x4000 over 0x1000)
        Setup s; s.held = 0x2000 | 0x8000 | 0x1000 | 0x4000;
        prepare(s); run(s);
        CHECK(norrath_quick_spell_box[s.pn] == 5);
        // the sound is only for local players: the second netPlayerIsLocal answer is the one used
        Setup quiet; quiet.held = 0x2000; quiet.local[1] = true; quiet.box = 0;
        prepare(quiet); localAnswers[1] = 0; run(quiet);
        CHECK(trace.find("menu_spell_high.anm") == std::string::npos);
    }
    // 3b. the four screen positions of the centre, and the fixed centre with the flag set
    for (int pn = 0; pn < 4; ++pn) {
        const int xs[4] = {0, 471, 156, 316};
        Setup s; s.pn = pn; s.held = 0; s.alpha = 100; s.timer = 5.0f; s.step = 0; s.half = pn & 1;
        prepare(s); run(s);
        const int halfX = (pn & 1) * 320;
        CHECK(trace.find("get(book);tex(5001," + num(halfX + xs[pn] + 67 - 68) + ",80,6,") != std::string::npos);
        Setup f = s; f.flag = 1; f.kind = 0; f.index = 5;
        prepare(f); run(f);
        CHECK(trace.find("get(book);tex(5001," + num(halfX + 141 - 68) + "," + num(249 - 65) + ",6,") != std::string::npos);
    }
    // 4. the fade: alpha moves by 4 * step, is limited, and the countdown drops
    {
        struct Fade { int alpha; float timer; int step; int wantAlpha; float wantTimer; int wantStep; } fades[] = {
            {100, 30.0f, 4, 116, 29.0f, 4}, {126, 30.0f, 4, 128, 29.0f, 4}, {100, 0.0f, 4, 96, -1.0f, -1}, {2, 0.0f, 4, 0, -1.0f, -1},
            {50, 5.0f, -1, 46, 4.0f, -1}, {128, 1.0f, 0, 128, 0.0f, 0},
            {100, -1.0f, 4, 116, -2.0f, 4}, {113, 30.0f, 4, 128, 29.0f, 4}, {112, 30.0f, 4, 128, 29.0f, 4}, {111, 30.0f, 4, 127, 29.0f, 4}, {3, 0.0f, 4, 0, -1.0f, -1}, {4, 0.0f, 4, 0, -1.0f, -1}};
        for (const Fade& f : fades) {
            Setup s; s.alpha = f.alpha; s.timer = f.timer; s.step = f.step; s.held = 0;
            s.flag = 1;     // flag keeps the menu active (and resets the countdown and step)
            // without flag and without a direction the menu resets nothing: use flag 0 and no direction
            s.flag = 0;
            prepare(s); run(s);
            unsigned char* rec = player + 23984 * s.pn;
            if (norrath_quick_spell_alpha[s.pn] != f.wantAlpha || get<float>(rec, 23564) != f.wantTimer || get<int>(rec, 23568) != f.wantStep)
                { std::printf("fade %d %g %d -> %d %g %d\\n", f.alpha, f.timer, f.step, norrath_quick_spell_alpha[s.pn], get<float>(rec, 23564), get<int>(rec, 23568)); return 1; }
        }
        // a direction or the flag re-arms: countdown 30 - 1, step 4, quick-spell flag on
        for (int mode = 0; mode < 2; ++mode) {
            Setup s; s.alpha = 10; s.timer = 0.0f; s.step = -4; s.held = mode == 0 ? 0x2000 : 0; s.flag = mode;
            prepare(s); run(s);
            unsigned char* rec = player + 23984 * s.pn;
            CHECK(get<float>(rec, 23564) == 29.0f && get<int>(rec, 23568) == 4 && norrath_quick_spell_alpha[s.pn] == 26 && hudQuickSpellOn[s.pn] == 1);
        }
        // nothing to show: the quick-spell flag drops and the menu ends right after the fade
        Setup s; s.alpha = 2; s.timer = 0.0f; s.step = 4; s.held = 0;
        prepare(s); run(s);
        CHECK(hudQuickSpellOn[s.pn] == 0 && norrath_quick_spell_alpha[s.pn] == 0 && trace == "local1;dlg;busy;task jumpgate;" || trace.find("tex(") == std::string::npos);
        Setup off; off.alpha = 0; off.timer = 0.0f; off.step = 0; off.held = 0;
        prepare(off); run(off);
        CHECK(hudQuickSpellOn[off.pn] == 0 && trace.find("tex(") == std::string::npos && trace.find("herm(") == std::string::npos);
        // the covered-bust flag: above 64, from player 2 on, with at least four players
        for (int pn : {1, 2, 3})
            for (int players : {3, 4})
                for (int alpha : {60, 61, 64, 65, 100}) {
                    Setup c; c.pn = pn; c.players = players; c.alpha = alpha; c.timer = 5.0f; c.step = 0; c.held = 0;
                    prepare(c); run(c);
                    const bool want = alpha > 64 && pn >= 2 && players >= 4;
                    if ((hudBustCovered == 1) != want) { std::printf("cover pn=%d players=%d alpha=%d\n", pn, players, alpha); return 1; }
                }
    }
    // 5. the ring of eight slots, with a selection, icons of every kind and the button glyphs
    {
        Setup s; s.pn = 1; s.half = 1; s.alpha = 100; s.timer = 30.0f; s.step = 4; s.held = 0x2000 | 0x4000;   // right-down: sel 3
        s.box = 3; s.hermite = 1.0f;
        s.slotSpell[0] = -1;                                   // empty
        s.slotSpell[1] = 3;                                    // global icon 3
        s.slotSpell[2] = 4;  s.slotNew[2] = false;             // global icon 4 is zero: falls back to the empty icon
        s.slotSpell[3] = 7;  s.slotNew[3] = true;              // a skill: the virtual icon function
        s.slotSpell[4] = 9;                                    // global icon 9 is zero
        s.slotSpell[5] = 12;                                   // global icon 12
        s.slotSpell[6] = 2;  s.slotNew[6] = true;
        s.slotSpell[7] = 33;
        s.assigned[0] = 5; s.assigned[1] = 3;
        prepare(s); run(s);
        // alpha 100 + 16 = 116 after the fade
        const int alpha = 116;
        const unsigned base = static_cast<unsigned>(alpha) << 24;
        const int halfX = 320, baseX = 471 + 67, baseY = 145;
        std::string want = "local1;dlg;busy;task jumpgate;blank(1,3,c);";
        want += "fmt(spellbookarrow%d.tex,spellbookarrow3.tex);find hud.lmp spellbookarrow3.tex;tex(9001," + num(halfX + baseX - 24) + "," + num(baseY - 24) + ",7," + hex(base | 0x808080) + ");";
        want += "get(book);tex(5001," + num(halfX + baseX - 68) + "," + num(baseY - 65) + ",6," + hex(base | 0x808080) + ");";
        want += "herm(906,0,1,0,0);";
        for (int i = 0; i < 8; ++i) {
            const int angle = -16384 + 8192 * i;
            const int slotX = baseX + static_cast<int>(60.0f * (static_cast<float>(angle) / 65536.0f)) + halfX;
            const int slotY = baseY + static_cast<int>(60.0f * (static_cast<float>(angle) / 131072.0f));
            std::string icon;
            const int spell = s.slotSpell[i];
            if (spell == -1) { want += "get(empty);"; icon = "5002"; }
            else if (s.slotNew[i]) { want += "skillicon(abc," + num(spell) + ");"; icon = hex(0x7000 + spell); }
            else if (globalIcons[spell] == 0) { want += "get(empty);"; icon = "5002"; }
            else icon = hex(globalIcons[spell]);
            if (i == 3) {
                want += "find hud.lmp smallspelliconcorona.tex;tex(9002," + num(slotX - 6) + "," + num(slotY - 6) + ",7," + hex(0x80808080u) + ");";
                want += "tex(" + icon + "," + num(slotX) + "," + num(slotY) + ",6," + hex(base | 0xefefefu) + ");";
            } else {
                want += "tex(" + icon + "," + num(slotX) + "," + num(slotY) + ",6," + hex(base | 0x808080) + ");";
            }
            want += "fstartS7," + hex(base | 0x323c3c) + ",-1;";
            const int textX = baseX + static_cast<int>(33.0f * (static_cast<float>(angle) / 65536.0f)) + halfX;
            const int textY = baseY + static_cast<int>(33.0f * (static_cast<float>(angle) / 131072.0f));
            for (int button = 0; button < 2; ++button) {
                want += "icon(1," + num(button + 1) + ");fmt(%c," + std::string(1, static_cast<char>('A' + button + 1)) + ");";
                if (s.assigned[button] == i) want += "fprint(" + num(textX + 5) + "," + num(textY) + "," + std::string(1, static_cast<char>('A' + button + 1)) + ",100000);";
            }
            want += "fend;";
        }
        CHECK(trace == want + "fmt(%s,sk7);text(sk7_name);fstartS7," + hex(static_cast<unsigned>(alpha / 2) << 24 | 0x808080) + ",-1;fpc(" + num(halfX + baseX + 24) + "," + num(baseY + 87) + ",8008,1000000);fend;");
    }
    // 6. the selected slot's label: empty, a skill slot, any other slot
    {
        for (int variant = 0; variant < 3; ++variant) {
            Setup s; s.held = 0x1000; s.box = 0; s.alpha = 40; s.timer = 30.0f;   // sel 0; alpha 40 + 16 = 56
            s.slotSpell[0] = variant == 0 ? -1 : 5;
            s.slotNew[0] = variant == 1;
            prepare(s); run(s);
            const std::string tail = trace.substr(trace.rfind("text("));
            const std::string label = variant == 0 ? "text(EmptySpellSlot);" : variant == 1 ? "fmt(%s,sk5);text(sk5_name);" : "fmt(%s,old);text(old_name);";
            const std::string full = trace.substr(trace.rfind(variant == 0 ? "text(EmptySpellSlot);" : "fmt(%s,"));
            CHECK(full.rfind(label, 0) == 0 && full.find("fstartS7," + hex(28u << 24 | 0x808080) + ",-1;fpc(") != std::string::npos);
            (void)tail;
        }
        // half alpha of an odd value rounds towards zero
        Setup s; s.held = 0x1000; s.alpha = 41; s.timer = 30.0f;      // 41 + 16 = 57 -> 28
        prepare(s); run(s);
        CHECK(trace.find("fstartS7," + hex(28u << 24 | 0x808080) + ",-1;fpc(") != std::string::npos);
    }
    // 7. button presses (flag zero): quick spell 0 or 1; the first button wins when both are down
    {
        struct Press { unsigned pressed; const char* want; } presses[] = {
            {0, ""}, {4, "quick(1,0,2);"}, {8, "quick(1,1,2);"}, {12, "quick(1,0,2);"}, {0x20, ""}, {0x10, ""}};
        for (const Press& p : presses) {
            Setup s; s.held = 0x2000; s.pressed = p.pressed; s.box = 2;
            prepare(s); run(s);
            CHECK(std::string(p.want).empty() ? trace.find("quick(") == std::string::npos : trace.find(p.want) != std::string::npos);
            CHECK(trace.find("book(") == std::string::npos);
        }
        // no selection: no assignment
        Setup s; s.held = 0; s.pressed = 4; s.timer = 10.0f;
        prepare(s); run(s);
        CHECK(trace.find("quick(") == std::string::npos && trace.find("text(") == std::string::npos);
    }
    // 8. flag set: the equip button assigns a spell book spell, the tail draws the panel
    {
        Setup s; s.flag = 1; s.kind = 1; s.index = 5; s.held = 0x4000; s.pressed = 0x20; s.equip = 0x20; s.half = 1; s.alpha = 100; s.timer = 30.0f; s.step = 4;
        s.slotSpell[4] = 6; prepare(s); run(s);
        const std::string tail = trace.substr(trace.find("book("));
        CHECK(tail.rfind("book(1,4,1,5);", 0) == 0);
        const unsigned base = 116u << 24;
        CHECK(trace.find("skillicon(abc,5);tex(7005,462,248,6," + hex(base | 0x808080) + ");") != std::string::npos);
        CHECK(trace.find("find hud.lmp spellbookarrow4.tex;tex(", trace.rfind("skillicon")) != std::string::npos);
        CHECK(trace.find("find hud.lmp spellbookarrow4.tex;tex(9003," + num(320 + 141 - 24) + "," + num(249 - 24) + ",6," + hex(base | 0x808080) + ");", trace.rfind("skillicon")) != std::string::npos);
        CHECK(trace.rfind("find inventry.lmp spellbookparch.tex;tex(", std::string::npos) != std::string::npos);
        CHECK(trace.find(",377,165,6," + hex(base | 0x808080) + ");", trace.rfind("spellbookparch")) != std::string::npos);
        // flag set: no button glyphs and no corona/arrow before the book
        CHECK(trace.find("icon(1,") == std::string::npos && trace.find("fmt(spellbookarrow%d.tex,spellbookarrow4.tex);find hud.lmp spellbookarrow4.tex;tex(9001") == std::string::npos);
        // kind other than 1 takes the global icon 37 (zero is possible: 37 % 5 == 2, so non-zero here)
        Setup g; g.flag = 1; g.kind = 0; g.index = 5; g.held = 0; g.alpha = 100; g.timer = 30.0f; g.step = 4;
        prepare(g); run(g);
        CHECK(trace.find("tex(" + hex(globalIcons[37]) + ",142,248,6,") != std::string::npos && trace.find("skillicon(") == std::string::npos);
        // an icon of zero is not drawn
        Setup z; z.flag = 1; z.kind = 0; z.held = 0; z.alpha = 100; z.timer = 30.0f; z.step = 4;
        prepare(z); globalIcons[37] = 0; run(z);
        CHECK(trace.find(",142,248,6,") == std::string::npos && trace.find("spellbookparch") != std::string::npos);
        // with the flag set and no selection, no arrow is drawn in the tail either
        Setup n; n.flag = 1; n.held = 0; n.alpha = 100; n.timer = 30.0f; n.step = 4;
        prepare(n); run(n);
        CHECK(trace.find("find hud.lmp spellbookarrow") == std::string::npos && trace.find("book(") == std::string::npos);
    }
    std::puts("draw newest quick spell menu passed");
    return 0;
}
