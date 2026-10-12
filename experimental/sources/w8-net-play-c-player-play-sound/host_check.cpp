#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#define sprintf unit_sprintf
#define strcmp unit_strcmp
#define strstr unit_strstr
#include "player_play_sound.cpp"
#undef sprintf
#undef strcmp
#undef strstr
alignas(8) unsigned char player[3 * 23984];
signed char cutMode, netNetworkMode, interfaceOn;
unsigned eRandState;
char levelName[322];

static std::string trace;
static int local_ret, flags_ret, flags_calls;
extern "C" int unit_sprintf(char* d, const char* f, ...) { va_list ap; va_start(ap, f); int n = vsnprintf(d, 64, f, ap); va_end(ap); return n; }
extern "C" int unit_strcmp(const char* a, const char* b) { return strcmp(a, b); }
extern "C" char* unit_strstr(const char* a, const char* b) { return (char*)strstr(a, b); }
extern "C" int netPlayerIsLocal__Fi(int) { return local_ret; }
extern "C" int soundReallyAllFlags__Fi(int w) { ++flags_calls; return w == 2 ? flags_ret : 0; }
extern "C" void soundStartGlobalDialogStream__FiPci(int a, char* n, int b) { trace += "S(" + std::to_string(a) + "," + n + "," + std::to_string(b) + ")"; }
extern "C" void soundStartGlobalDialogStreamRandom__FiPci(int a, char* n, int b) { trace += "R(" + std::to_string(a) + "," + n + "," + std::to_string(b) + ")"; }

static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
static const char* voices[10] = {"bwm", "bwf", "erm", "erf", "ecm", "ecf", "ewm", "ewf", "esm", "esf"};
static int fails;
#define EXPECT(cond, tag) do { if (!(cond)) { printf("FAIL %s (trace: %s)\n", tag, trace.c_str()); ++fails; } } while (0)
static void reset() {
    memset(player, 0, sizeof player); memset(levelName, 0, sizeof levelName);
    cutMode = 0; netNetworkMode = 0; interfaceOn = 0; eRandState = 12345; local_ret = 1; flags_ret = 0; flags_calls = 0; trace.clear();
    float h = 10.0f; memcpy(player + 172, &h, 4); memcpy(player + 23984 + 172, &h, 4);
}
static void set_class(int idx, int c) { memcpy(player + idx * 23984 + 84, &c, 4); }
struct Level { bool exact; const char* name; int form; int base; };  // form 0 const, 1 mod3, 2 and1
int main() {
    char want[128];
    // simple sounds for every class
    const char* fmts[10] = {"R(2,%s001,36)", "", "", "R(2,%s014,36)", "R(2,%s018,36)", "R(2,%s019,36)", "", "R(2,%s043,36)", "", ""};
    for (int cls = 0; cls < 10; ++cls) {
        for (int snd = 0; snd < 10; ++snd) {
            reset(); set_class(0, cls);
            playerPlaySound__Fii(0, snd);
            if (snd == 6) { EXPECT(trace.empty() && eRandState == 12345, "six"); continue; }
            if (snd == 1) {
                unsigned r = lcg(12345) >> 16;
                snprintf(want, sizeof want, "R(2,%s%03u,36)", voices[cls], r % 3 + 11);
                EXPECT(trace == want && eRandState == lcg(12345), "one");
            } else if (snd == 8 || snd == 9) {
                unsigned r = lcg(12345) >> 16;
                snprintf(want, sizeof want, "R(2,%s%03u,36)", voices[cls], (r & 1) + (snd == 8 ? 41 : 35));
                EXPECT(trace == want && eRandState == lcg(12345), "eight-nine");
            } else if (snd == 2) {
                EXPECT(trace.empty(), "two-no-level");
            } else {
                snprintf(want, sizeof want, fmts[snd], voices[cls]);
                EXPECT(trace == want && eRandState == 12345, "fixed");
            }
        }
    }
    // second player record
    reset(); set_class(1, 9); playerPlaySound__Fii(1, 0); EXPECT(trace == "R(2,esf001,36)", "player1");
    // level sounds, in list order; the checks below restate the table from the image
    static const Level L[] = {
        {1, "kelethin", 1, 45}, {1, "faydark1", 0, 48}, {1, "faydark2", 2, 49}, {1, "faydark3", 1, 51}, {0, "goblin", 1, 112},
        {1, "orccave1", 0, 54}, {1, "orccave2", 0, 55}, {1, "orccave3", 0, 56}, {0, "ruinint", 1, 57}, {1, "gnomehub", 1, 61},
        {0, "anthill", 1, 64}, {0, "lava", 1, 67}, {0, "battle", 1, 70}, {0, "gothic", 1, 73}, {0, "nas", 1, 73},
        {1, "beachhub", 1, 79}, {1, "des1", 1, 85}, {1, "des2", 1, 85}, {0, "grass", 1, 88}, {0, "cloud", 1, 91},
        {0, "oce", 1, 97}, {1, "desert", 1, 100}, {1, "bronzeht", 1, 103}, {0, "island2", 1, 106}, {0, "kerriel", 1, 106},
        {0, "islcave", 1, 106}, {1, "frozesea", 1, 109}, {1, "arena1", 1, 118}, {0, "garden", 1, 124}, {1, "airhub", 1, 127},
        {1, "hate1", 0, 130}, {1, "hate2", 0, 131}, {1, "hate3", 0, 132}, {1, "hate4", 1, 133}};
    for (const Level& lv : L) {
        for (int variant = 0; variant < 2; ++variant) {
            reset(); set_class(0, 4);
            std::string lvl = lv.name;
            if (variant == 1) { if (lv.exact) continue; lvl = "x" + lvl + "9"; }
            strcpy(levelName + 81, lvl.c_str());
            // names that also contain an earlier entry change the expected winner; skip those variants
            bool earlier = false;
            for (const Level& e : L) { if (&e == &lv) break; if (e.exact ? lvl == e.name : lvl.find(e.name) != std::string::npos) earlier = true; }
            if (earlier) continue;
            playerPlaySound__Fii(0, 2);
            unsigned r = lcg(12345) >> 16;
            int n = lv.base + (lv.form == 1 ? (int)(r % 3) : lv.form == 2 ? (int)(r & 1) : 0);
            snprintf(want, sizeof want, "S(2,ecm%03d.vag,36)", n);
            EXPECT(trace == want, lv.name);
            EXPECT(eRandState == (lv.form == 0 ? 12345u : lcg(12345)), "level-rand");
        }
    }
    // exact names do not match when longer; unknown level plays nothing and does not step the generator
    reset(); strcpy(levelName + 81, "kelethin2"); playerPlaySound__Fii(0, 2); EXPECT(trace.empty() && eRandState == 12345, "exact-longer");
    reset(); strcpy(levelName + 81, "nowhere"); playerPlaySound__Fii(0, 2); EXPECT(trace.empty() && eRandState == 12345, "unknown-level");
    // first entry wins: a name containing "goblin" and "lava" matches goblin (112) first
    reset(); strcpy(levelName + 81, "lavagoblin"); playerPlaySound__Fii(0, 2);
    snprintf(want, sizeof want, "S(2,bwm%03u.vag,36)", 112 + (lcg(12345) >> 16) % 3); EXPECT(trace == want, "order");
    // guards
    reset(); local_ret = 0; playerPlaySound__Fii(0, 0); EXPECT(trace.empty() && flags_calls == 0, "not-local");
    reset(); cutMode = 1; playerPlaySound__Fii(0, 0); EXPECT(trace.empty(), "cut");
    reset(); interfaceOn = 1; playerPlaySound__Fii(0, 0); EXPECT(trace.empty(), "interface");
    reset(); interfaceOn = 1; netNetworkMode = 1; playerPlaySound__Fii(0, 0); EXPECT(trace == "R(2,bwm001,36)", "interface-net");
    reset(); flags_ret = 0x20; playerPlaySound__Fii(0, 0); EXPECT(trace.empty() && flags_calls == 1, "flags");
    reset(); flags_ret = 0x20; playerPlaySound__Fii(0, 10); EXPECT(trace.empty() && flags_calls == 0, "flags-high");
    reset(); flags_ret = 0xdf; playerPlaySound__Fii(0, 0); EXPECT(trace == "R(2,bwm001,36)", "flags-other-bits");
    reset(); playerPlaySound__Fii(0, -1); EXPECT(trace.empty() && flags_calls == 1, "negative");
    reset(); playerPlaySound__Fii(0, 11); EXPECT(trace.empty() && flags_calls == 0, "eleven");
    reset(); { float z = 0.0f; memcpy(player + 172, &z, 4); } playerPlaySound__Fii(0, 0); EXPECT(trace.empty(), "dead");
    reset(); { float z = -3.0f; memcpy(player + 172, &z, 4); } playerPlaySound__Fii(0, 0); EXPECT(trace.empty(), "negative-health");
    printf(fails ? "FAILED\n" : "");
    return fails ? 1 : 0;
}
