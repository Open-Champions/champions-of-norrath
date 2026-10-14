// Build with the host compiler and -fno-builtin (strcmp and sprintf are called by name).
// Host contract for msg_hurt__6PlayerfP10GameObjecti; not an EE input. Includes the source in host mode
// (32-bit words are offsets from w11_actor_b_host_base) and logs the callees' arguments.
#define W11_ACTOR_B_HOST 1
#include "msg_hurt.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
signed char recallOn;
signed char interfaceOn;
int gameControlPaused;
int syncRandState;
int gameFlags[4];
char levelName[256];
unsigned char player[4 * 23984];
static unsigned char arena[0x20000] __attribute__((aligned(16)));  // object at 0, attacker at 0x2000, table at 0x8000
static std::string calls;
static int jump, localPlayer, strcmpResult, atanResult;
static void* spellResult;
static float sinValue, cosValue, absorbValue;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return p ? std::to_string(static_cast<const unsigned char*>(p) - arena) : std::string("null"); }
extern "C" {
int compare_text(const char* a, const char* b) __asm__("strcmp");
int compare_text(const char* a, const char* b) { calls += std::string("strcmp(") + a + "," + b + ");"; (void)a; return strcmpResult; }
int format_text(char* d, const char* fmt, ...) __asm__("sprintf");
int format_text(char* d, const char* fmt, ...) { std::strcpy(d, fmt); calls += std::string("sprintf(") + fmt + ");"; return 0; }
int jumpGateJumping__Fv() { calls += "jump;"; return jump; }
int netPlayerIsLocal__Fi(int i) { calls += "local" + std::to_string(i) + ";"; return localPlayer; }
void engineSetScreenColor__Fiiii(int r, int g, int b, int a) { calls += "color" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a) + ";"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int fl) {
    calls += "anim" + off(s) + "," + off(h) + "," + f(a) + "," + f(b) + "," + std::to_string(fl) + ";";
}
float icos__Fi(int a) { calls += "cos" + std::to_string(a) + ";"; return cosValue; }
float isin__Fi(int a) { calls += "sin" + std::to_string(a) + ";"; return sinValue; }
char* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return reinterpret_cast<char*>(0x1000); }
void soundPlaySequence(void* h, Point3 p, float r) { calls += "snd" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(h)) + "," + f(p.x) + "," + f(p.y) + "," + f(p.z) + "," + f(r) + ";"; }
void playerBlockSparks__6Playeri(void* s, int a) { calls += "sparks" + off(s) + "," + std::to_string(a) + ";"; }
void* __tf15SpellHolyShield() { calls += "tf;"; return arena + 0x100; }
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* s, void* t) { calls += "check" + off(s) + "," + off(t) + ";"; return spellResult; }
float manaAbsorbOnHit__15SpellHolyShield(void*) { calls += "absorb;"; return absorbValue; }
int iatan2__Fff(float y, float x) { calls += "atan" + f(y) + "," + f(x) + ";"; return atanResult; }
void listDestroy__FP8ListHead(void* h) { calls += "destroy" + off(h) + ";"; }
void listOnewayLink__FP8ListHeadT0Pv(void* h, void* a, void* b) { calls += "link" + off(h) + "," + off(a) + "," + off(b) + ";"; }
}
static void set_real(unsigned o, float v) { std::memcpy(arena + o, &v, 4); }
static void set_word(unsigned o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static float get_real(unsigned o) { float v; std::memcpy(&v, arena + o, 4); return v; }
static unsigned get_word(unsigned o) { unsigned v; std::memcpy(&v, arena + o, 4); return v; }
static const unsigned AT = 0x2000, TABLE = 0x8000;

static void reset() {
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
    w11_actor_b_host_base = arena;
    recallOn = 0; interfaceOn = 0; gameControlPaused = 0; syncRandState = 0; std::memset(gameFlags, 0, sizeof gameFlags);
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, "elsewhere");
    jump = 0; localPlayer = 0; strcmpResult = 1; atanResult = 0; spellResult = nullptr; sinValue = 0; cosValue = 0; absorbValue = 0;
    set_word(960, 1); set_word(36, TABLE);
    set_real(16, 1.0f); set_real(20, 2.0f); set_real(AT + 16, 5.0f); set_real(AT + 20, 2.0f);
    calls.clear();
}
#define CHECK(cond, msg) do { if (!(cond)) { std::puts(calls.c_str()); std::puts("fail: " msg); return 1; } } while (0)

int main() {
    // each early-out stops everything
    reset(); recallOn = 1; msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls.empty() && get_real(192) == 0.0f, "recallOn");
    reset(); set_word(1188, 1); msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls.empty() && get_real(192) == 0.0f, "+1188");
    reset(); set_word(1328, 1); msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls.empty(), "+1328");
    reset(); strcmpResult = 0; msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls == "strcmp(elsewhere,actintro);", "level name");
    reset(); jump = 1; msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls == "strcmp(elsewhere,actintro);jump;", "jump gate");
    reset(); gameControlPaused = 1; msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, arena + AT, 0);
    CHECK(calls == "strcmp(elsewhere,actintro);jump;", "paused");
    // plain hit without attacker: health goes up by damage, flags merge, bit 0 of gameFlags[player]
    reset(); set_real(192, 1.0f); set_word(744, 1); interfaceOn = 1; localPlayer = 1;
    msg_hurt__6PlayerfP10GameObjecti(arena, 5.0f, nullptr, 6);
    CHECK(calls == "strcmp(elsewhere,actintro);jump;local1;color64,0,0,0;tf;check0,256;", "plain calls");
    CHECK(get_real(192) == 6.0f && get_word(744) == 7 && gameFlags[1] == 1 && gameFlags[0] == 0, "plain result");
    // zero health: no flag
    reset(); msg_hurt__6PlayerfP10GameObjecti(arena, 0.0f, nullptr, 0);
    CHECK(gameFlags[1] == 0, "zero health sets no flag");
    // holy shield absorbs
    reset(); set_real(192, 0.0f); spellResult = arena + 0x300; absorbValue = 0.25f;
    std::memcpy(player + 23984 + 196, &(const float&)(4.0f), 4);
    msg_hurt__6PlayerfP10GameObjecti(arena, 8.0f, nullptr, 0);
    CHECK(calls.find("absorb;") != std::string::npos, "absorb call");
    { float pool; std::memcpy(&pool, player + 23984 + 196, 4); CHECK(get_real(192) == 6.0f && pool == 2.0f, "absorb arithmetic"); }
    reset(); spellResult = arena + 0x300; absorbValue = 2.0f;
    { float p = 1.0f; std::memcpy(player + 23984 + 196, &p, 4); }
    msg_hurt__6PlayerfP10GameObjecti(arena, 8.0f, nullptr, 0);
    { float pool; std::memcpy(&pool, player + 23984 + 196, 4); CHECK(get_real(192) == 0.0f && pool == 0.0f, "absorb clamps"); }
    reset(); spellResult = arena + 0x300; absorbValue = 0.5f;   // pool not positive: no absorb call
    msg_hurt__6PlayerfP10GameObjecti(arena, 8.0f, nullptr, 0);
    CHECK(calls.find("absorb;") == std::string::npos && get_real(192) == 8.0f, "no pool");
    // attacker: direction and link
    reset(); set_word(1400, 7); atanResult = 1000;
    msg_hurt__6PlayerfP10GameObjecti(arena, 2.0f, arena + AT, 0);
    CHECK(calls.find("atan-4,0;") != std::string::npos, "atan arguments");
    CHECK(calls.find("destroy752;link752,8192,8192;") != std::string::npos, "link");
    { short h; std::memcpy(&h, arena + 748, 2); CHECK(h == 16384 - 1000, "heading"); }
    // high bit of the merged flags suppresses the heading
    reset(); set_word(1400, 7); atanResult = 1000;
    msg_hurt__6PlayerfP10GameObjecti(arena, 2.0f, arena + AT, 0x800000);
    CHECK(calls.find("atan") == std::string::npos && calls.find("destroy752;") != std::string::npos, "suppressed heading");
    // attacker with the indirect link bit
    reset(); set_word(1400, 7); set_word(AT + 12, 0x80); set_word(AT + 76, 0x4000); set_word(0x4000 + 12, 0x6000);
    msg_hurt__6PlayerfP10GameObjecti(arena, 2.0f, arena + AT, 0);
    CHECK(calls.find("link752,24576,24576;") != std::string::npos, "indirect link");
    reset(); set_word(1400, 7); set_word(AT + 12, 0x80); set_word(AT + 76, 0);
    msg_hurt__6PlayerfP10GameObjecti(arena, 2.0f, arena + AT, 0);
    CHECK(calls.find("link752,8192,8192;") != std::string::npos, "indirect link falls back");
    reset(); set_word(1400, 7); set_word(AT + 12, 0x80); set_word(AT + 76, 0x4000); set_word(0x4000 + 12, 0);
    msg_hurt__6PlayerfP10GameObjecti(arena, 2.0f, arena + AT, 0);
    CHECK(calls.find("link") == std::string::npos && calls.find("destroy752;") != std::string::npos, "null link target");
    // block branch: type 8 attacker in front, random value picks the animation
    reset(); set_word(1400, 8); syncRandState = 12345; cosValue = 1.0f; sinValue = 0.0f;
    player[23984 + 23582] = 3;
    set_word(TABLE + 3 * 104 + 148, 0x7000); set_word(TABLE + 3 * 104 + 140, 0x7100);
    msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0);
    { unsigned st = 12345u * 1664525u + 1013904223u; unsigned r = st >> 16;
      std::string a = "anim288," + std::to_string(r <= 44999 ? 0x7000 : 0x7100) + "," + (r <= 44999 ? "0.2" : "0.1");
      CHECK(calls.find(a) != std::string::npos, "block animation"); CHECK((unsigned)syncRandState == st, "random state"); }
    CHECK(calls.find("sprintf(hit_mtlplate_bludg_med.anm);lump(fx.lmp,hit_mtlplate_bludg_med.anm);snd4096,1,2,0,1;sparks0,0;") != std::string::npos, "block effects");
    CHECK(get_real(192) == 0.0f, "block removes the damage");
    reset(); set_word(1400, 11); cosValue = 1.0f; sinValue = 0.0f;
    msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0x20000);
    CHECK(get_real(192) == 3.0f, "block with 0x20000 halves");
    // behind: no block effects; flags 0x40 skips the whole branch
    reset(); set_word(1400, 8); cosValue = -1.0f; sinValue = 0.0f;
    msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0);
    CHECK(calls.find("sprintf") == std::string::npos && calls.find("anim") != std::string::npos && get_real(192) == 6.0f, "behind");
    reset(); set_word(1400, 8); cosValue = 1.0f;
    msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0x40);
    CHECK(calls.find("anim") == std::string::npos && get_real(192) == 6.0f, "0x40 skips the block branch");
    reset(); set_word(1400, 9); cosValue = 1.0f;
    msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0);
    CHECK(calls.find("anim") == std::string::npos, "other type skips the block branch");
    // the threshold: upper half 44999 chooses the first table entry, 45000 the second
    for (unsigned want : {44999u, 45000u}) {
        unsigned prev = 0;
        for (unsigned guess = 0; guess < 0x10000; ++guess) {   // find a previous state whose next upper half is `want`
            unsigned candidate = guess * 0x10001u + 12345u * guess;
            if (((candidate * 1664525u + 1013904223u) >> 16) == want) { prev = candidate; break; }
        }
        reset(); set_word(1400, 8); syncRandState = static_cast<int>(prev); cosValue = -1.0f;
        set_word(TABLE + 148, 0x7000); set_word(TABLE + 140, 0x7100);
        msg_hurt__6PlayerfP10GameObjecti(arena, 6.0f, arena + AT, 0);
        const std::string expect = want == 44999u ? "anim288,28672,0.2" : "anim288,28928,0.1";
        CHECK(calls.find(expect) != std::string::npos, "threshold");
    }
    return 0;
}
