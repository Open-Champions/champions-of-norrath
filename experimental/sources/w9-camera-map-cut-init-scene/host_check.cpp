// Host contract for cutInitScene__FP9_cutscene. Not an EE plan input.
// The unit stores object pointers as 32-bit words, so this check is built for
// x86_64 with -Wl,-pagezero_size,0x10000 and keeps all fake objects at
// 0x20000000 and above:
//   c++ -arch x86_64 -std=c++17 -Wl,-pagezero_size,0x10000 -o check host_check.cpp
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/mman.h>
#define strcmp unit_strcmp
#include "cut_init_scene.cpp"
#undef strcmp

short myYaw, myPitch, myRoll;
float myFOV;
int nmActors;
unsigned char* actor;
int nmActions;
unsigned char* action;
int flags;
signed char netNetworkMode;
void* gameCam;
float worldCameraPos[3];
char cutSceneName[64];
char cutStartMusic[64];
int displayTextDisable;
char* dlgDir;
int cutLoadedDialogStream;
unsigned char* dialogPacket;
int displayDialogPos, displayMusicPos, animationLoadPos, nmDialogNodes;
CutAnimSlot anim[2];
int eRandState;
unsigned char player[23984 * 2];
int nmPlayers, frontEndRunning, subtitlesOn, engineScreenSlide;

static std::string ev;
static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static std::string num(long v) { return std::to_string(v); }

static unsigned char* arena;
static size_t arena_used;
static unsigned char* take(size_t n) {
    unsigned char* p = arena + arena_used;
    arena_used += (n + 15) & ~size_t(15);
    std::memset(p, 0, n);
    return p;
}

// Rotation model: angle in 1/65536 turns.
static float c_of(int a) { return float(std::cos(double(a) * 2.0 * M_PI / 65536.0)); }
static float s_of(int a) { return float(std::sin(double(a) * 2.0 * M_PI / 65536.0)); }

static unsigned char *origin_obj;
static int found_calls;
static unsigned char* box_results[16];
static float box_low[3], box_high[3];
static int box_count;
static unsigned char* created_obj;
static int created_calls;
static char created_name[64];
static int created_yaw;
static float created_pos[3];
static char** created_tags;
static int player_ctor_calls, player_index_seen;
static float player_pos[3];
static int player_yaw;
static unsigned char* block_mem[2];
static int block_calls;
static unsigned char* tag_hit[2];
static int run_list_calls;

extern "C" int unit_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" {
float icos__Fi(int a) { ev += "c" + num(a) + " "; return c_of(a); }
float isin__Fi(int a) { ev += "s" + num(a) + " "; return s_of(a); }
void* objectFindObject__FPc(char* n) { ev += "find(" + std::string(n) + ") "; ++found_calls; return origin_obj; }
void objectIterateRunLists__FPFP10GameObject_v(void (*cb)(void*)) {
    ++run_list_calls;
    ev += cb == showHiddenObjects__FP10GameObject ? "iter-show " : cb == hideHiddenObjects__FP10GameObject ? "iter-hide " : cb == removeMonster__FP10GameObject ? "iter-remove " : "iter-? ";
}
void showHiddenObjects__FP10GameObject(void*) {}
void hideHiddenObjects__FP10GameObject(void*) {}
void removeMonster__FP10GameObject(void*) {}
void sleepMonsters__8creaturev() { ev += "sleep "; }
int GetYaw__6Camera(void* c) { ev += "yaw "; return c == gameCam ? 1234 : -1; }
int GetPitch__6Camera(void* c) { ev += "pitch "; return c == gameCam ? -4321 : -1; }
char* soundGetDialogDir__Fv() { ev += "dialogdir "; return dlgDir; }
static unsigned char* packet_result;
unsigned char* findCutDialog__Fv() { ev += "finddialog "; return packet_result; }
static unsigned alloc_sizes[4]; static int alloc_n;
void* memAlloc__FiPc(int size, char* name) { alloc_sizes[alloc_n++] = size; ev += "alloc(" + num(size) + "," + name + ") "; return take(16); }
void soundStartDialogStream__FiPci(int s, char* n, int v) { ev += "stream(" + num(s) + "," + n + "," + num(v) + ") "; }
void soundResetStream__Fi(int s) { ev += "reset(" + num(s) + ") "; }
void soundStartMusic__Fii(int i, int v) { ev += "music(" + num(i) + "," + num(v) + ") "; }
void* blockAlloc__Fi(int size) { ev += "block(" + num(size) + ") "; ++block_calls; return block_mem[block_calls - 1 & 1]; }
void __6PlayerG6Point3sii(void* self, float* p, short yaw, int index, int extra) {
    ++player_ctor_calls; player_index_seen = index; player_yaw = yaw;
    std::memcpy(player_pos, p, 12);
    ev += "player(" + num(self == block_mem[0] ? 0 : self == block_mem[1] ? 1 : -1) + "," + num(index) + "," + num(extra) + ") ";
}
void* objectCreateByName__FPcG6Point3sPPc(char* n, float* p, short yaw, char** tags) {
    ++created_calls; std::strcpy(created_name, n); std::memcpy(created_pos, p, 12); created_yaw = yaw; created_tags = tags;
    ev += "create(" + std::string(n) + ") ";
    return created_obj;
}
void objectFindInBox__FG6Point3T0PP10GameObjecti(float* lo, float* hi, void** out, int count) {
    std::memcpy(box_low, lo, 12); std::memcpy(box_high, hi, 12); box_count = count;
    int i = 0;
    for (; box_results[i]; ++i) out[i] = box_results[i];
    out[i] = nullptr;
    ev += "box ";
}
static int tag_calls;
static unsigned char* tag_owner;
char* findTagString__10GameObjectPci(void* o, char* tag, int) {
    ev += std::string("tag(") + tag + ") ";
    ++tag_calls;
    if (o == tag_hit[0] && std::strcmp(tag, "autoprop") == 0) return tag;
    if (o == tag_hit[1] && std::strcmp(tag, "autodoor") == 0) return tag;
    return nullptr;
}
void objectUpdateInGrid__FP10GameObject(void*) { ev += "grid "; }
void worldSetFarClip__Ff(float d) { ev += "far(" + num(long(d)) + ") "; }
}

// A fake object: 700 bytes. Table at +72 (native pointer), name text word at +52.
struct Fake { unsigned char* mem; };
static int type_calls, virtual_calls, virtual_arg, virtual_adjust;
static const unsigned char* type_record_for(const char* name) {
    static const char* names[8]; static unsigned char* recs[8]; static int n;
    for (int i = 0; i < n; ++i) if (std::strcmp(names[i], name) == 0) return recs[i];
    unsigned char* r = (unsigned char*)std::malloc(16);
    char* copy = (char*)std::malloc(32);
    std::strcpy(copy, name);
    std::memcpy(r, &copy, sizeof copy);
    names[n] = copy; recs[n++] = r;
    return r;
}
static const char* current_type;
static const unsigned char* type_fn() { ++type_calls; return type_record_for(current_type); }
static void virtual_fn(void* self, int arg) { ++virtual_calls; virtual_arg = arg; virtual_adjust = int((unsigned char*)self - (unsigned char*)0); }

static unsigned char* make_object(float x, float y, float z, const char* text) {
    unsigned char* o = take(700);
    float f[3] = {x, y, z};
    std::memcpy(o + 16, f, 12);
    if (text) {
        char* t = (char*)take(32);
        std::strcpy(t, text);
        unsigned w = (unsigned)(unsigned long)t;
        std::memcpy(o + 52, &w, 4);
    }
    unsigned char* table = take(256);
    TypeEntry te{0, 0, type_fn};
    VirtualEntry ve{4, 0, virtual_fn};
    std::memcpy(table, &te, sizeof te);
    std::memcpy(table + 184, &ve, sizeof ve);
    std::memcpy(o + 72, &table, sizeof table);
    return o;
}

static void setw(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }
static int getw(const unsigned char* base, int off) { int v; std::memcpy(&v, base + off, 4); return v; }
static float getf(const unsigned char* base, int off) { float v; std::memcpy(&v, base + off, 4); return v; }
static void setf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static short geth(const unsigned char* base, int off) { short v; std::memcpy(&v, base + off, 2); return v; }
static void seth(unsigned char* base, int off, int v) { short s = (short)v; std::memcpy(base + off, &s, 2); }

static void reset_world() {
    ev.clear(); found_calls = 0; created_calls = 0; player_ctor_calls = 0; block_calls = 0; alloc_n = 0; tag_calls = 0;
    run_list_calls = 0; type_calls = 0; virtual_calls = 0;
    box_results[0] = nullptr; tag_hit[0] = tag_hit[1] = nullptr;
    origin_obj = nullptr; created_obj = nullptr; packet_result = nullptr; dlgDir = nullptr;
    netNetworkMode = 0; frontEndRunning = 0; subtitlesOn = 0; engineScreenSlide = 77; nmPlayers = 1;
    std::memset(player, 0, sizeof player);
    anim[0].block = anim[1].block = nullptr; anim[0].second = anim[1].second = 9;
    nmDialogNodes = 0; displayDialogPos = 5; displayMusicPos = 5; animationLoadPos = 5; displayTextDisable = 0;
    std::strcpy(cutSceneName, "origin");
    worldCameraPos[0] = 11.9f; worldCameraPos[1] = -22.9f; worldCameraPos[2] = 33.5f;
    eRandState = 0;
}

struct Scene { unsigned char* base; unsigned char* acts; unsigned char* actors; };
static Scene make_scene(int flag_word, int n_actions, int n_actors, int spare_actions) {
    Scene s;
    s.base = take(1200 + 20 * (n_actions + spare_actions + 4) + 44 * (n_actors + 1));
    s.acts = s.base + 200;
    s.actors = s.acts + 20 * (n_actions + spare_actions + 4);
    setw(s.base, 0, flag_word);
    setw(s.base, 4, int(s.acts - s.base));
    setw(s.base, 8, n_actions);
    setw(s.base, 12, int(s.actors - s.base));
    setw(s.base, 16, n_actors);
    return s;
}

int main() {
    arena = (unsigned char*)mmap((void*)0x20000000, 1 << 24, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if (arena != (unsigned char*)0x20000000) { std::fprintf(stderr, "mmap failed\n"); return 2; }

    // ---- Block 1: no origin object found; globals and idempotent flag.
    {
        reset_world();
        Scene s = make_scene(0, 2, 1, 0);
        std::strcpy((char*)s.base + 116, "tune");
        cutInitScene__FP9_cutscene(s.base);
        expect(myYaw == -8192 && myPitch == 10000 && myRoll == 0 && myFOV == 50.0f, "view defaults");
        expect(nmActors == 1 && nmActions == 2 && actor == s.actors && action == s.acts && flags == 0x10, "globals from the scene (bit 4 set at the end outside the front end)");
        expect(getw(s.base, 0) == 1, "scene flag bit 0 set");
        expect(found_calls == 1 && ev.find("find(origin)") == 0, "origin lookup first");
        expect(std::strcmp(cutStartMusic, "tune") == 0 && displayTextDisable == 2 && cutLoadedDialogStream == 0, "music name and text disable");
        expect(getw(s.base, 112) == 10, "default word 112");
        expect(animationLoadPos == 0 && displayDialogPos == 0 && displayMusicPos == 0, "dialog positions cleared");
        // Second call: bit 0 set, so no lookup.
        ev.clear(); found_calls = 0;
        setw(s.base, 112, 55);
        cutInitScene__FP9_cutscene(s.base);
        expect(found_calls == 0 && getw(s.base, 112) == 55, "second call skips the lookup and keeps word 112");
    }

    // ---- Block 2: transform with an origin object.
    {
        reset_world();
        Scene s = make_scene(0, 6, 2, 0);
        origin_obj = make_object(100.0f, 200.0f, 300.0f, nullptr);
        seth(origin_obj, 56, 0x2000);   // a quarter of a turn / 2
        // Actors (no create flag, nothing found nearby -> objects cleared).
        float pos0[3] = {10.0f, 20.0f, 30.0f}, pos1[3] = {-5.0f, 8.0f, 0.5f};
        std::memcpy(s.actors + 28, pos0, 12); seth(s.actors, 40, 0x100);
        std::memcpy(s.actors + 44 + 28, pos1, 12); seth(s.actors + 44, 40, -0x200);
        // Actions: kind -100 type 0 (position), -100 type 1 (yaw), -100 type 2 (untouched),
        // kind 0 type 5 (position), kind 0 type 6 (yaw), kind 0 type 3 (untouched).
        struct A { int kind, type, w8, w12, w16; } acts[6] = {
            {-100, 0, 50, 60, 70}, {-100, 1, 0x1234, 7, 8}, {-100, 2, 9, 10, 11},
            {0, 5, -40, 25, 100}, {0, 6, 0x0010, 1, 2}, {0, 3, 123, 456, 789}};
        for (int i = 0; i < 6; ++i) {
            unsigned char* r = s.acts + 20 * i;
            setf(r, 0, float(i));
            seth(r, 4, acts[i].kind); seth(r, 6, acts[i].type);
            setw(r, 8, acts[i].w8); setw(r, 12, acts[i].w12); setw(r, 16, acts[i].w16);
        }
        unsigned char before[6 * 20];
        std::memcpy(before, s.acts, sizeof before);
        cutInitScene__FP9_cutscene(s.base);
        const int rot = -0x2000;
        const double c = std::cos(rot * 2.0 * M_PI / 65536.0), sn = std::sin(rot * 2.0 * M_PI / 65536.0);
        const float* P[2] = {pos0, pos1};
        for (int i = 0; i < 2; ++i) {
            const unsigned char* a = s.actors + 44 * i;
            expect(std::fabs(getf(a, 28) - (P[i][0] * c + P[i][1] * sn + 100.0)) < 1e-3, "actor x");
            expect(std::fabs(getf(a, 32) - (-P[i][0] * sn + P[i][1] * c + 200.0)) < 1e-3, "actor y");
            expect(std::fabs(getf(a, 36) - (P[i][2] + 300.0)) < 1e-3, "actor z");
        }
        expect(geth(s.actors, 40) == 0x100 + 0x2000 && geth(s.actors + 44, 40) == -0x200 + 0x2000, "actor yaw halfwords");
        auto pos_expect = [&](int i) {
            const double x = acts[i].w8, y = acts[i].w12, z = acts[i].w16;
            const unsigned char* r = s.acts + 20 * i;
            expect(getw(r, 8) == int(x * c + y * sn + 100.0 + 0.5), "action position x");
            expect(getw(r, 12) == int(-x * sn + y * c + 200.0 + 0.5), "action position y");
            expect(getw(r, 16) == int(z + 300.0 + 0.5), "action position z");
        };
        expect(ev.find("c-8192 s-8192 ") != std::string::npos, "rotation is the negated halfword for both calls");
        expect(ev.find(" c8192 ") == std::string::npos && ev.find("c8192 ") != 0 && ev.find("s8192 ") == std::string::npos, "no positive angle is ever passed");
        pos_expect(0); pos_expect(3);
        auto yaw_expect = [&](int i) {
            const unsigned char* r = s.acts + 20 * i;
            const unsigned short h = (unsigned short)((unsigned short)(0u - (unsigned short)acts[i].w8) + 0x2000);
            expect(getw(r, 8) == -(int)(short)h, "action yaw word");
            expect(getw(r, 12) == acts[i].w12 && getw(r, 16) == acts[i].w16, "yaw leaves the other words");
        };
        yaw_expect(1); yaw_expect(4);
        expect(std::memcmp(s.acts + 40, before + 40, 20) == 0, "type 2 untouched");
        expect(std::memcmp(s.acts + 100, before + 100, 20) == 0, "type 3 untouched");
        // Kind -100 / type 5 is not a position record, nor is kind 0 / type 1.
        reset_world();
        Scene t = make_scene(0, 2, 0, 0);
        origin_obj = make_object(1, 2, 3, nullptr);
        seth(origin_obj, 56, 0x400);
        for (int i = 0; i < 2; ++i) { seth(t.acts + 20 * i, 4, i == 0 ? -100 : 0); seth(t.acts + 20 * i, 6, i == 0 ? 5 : 1); setw(t.acts + 20 * i, 8, 777); }
        cutInitScene__FP9_cutscene(t.base);
        expect(getw(t.acts, 8) == 777 && getw(t.acts + 20, 8) == 777, "other combinations untouched");
    }

    // ---- Block 3: flag bit 2 inserts three leading records; bit 4 appends three.
    {
        reset_world();
        Scene s = make_scene(2, 2, 0, 3);
        for (int i = 0; i < 2; ++i) { setf(s.acts + 20 * i, 0, 5.0f + i); seth(s.acts + 20 * i, 4, 3 + i); setw(s.acts + 20 * i, 8, 100 + i); }
        gameCam = &failures;
        cutInitScene__FP9_cutscene(s.base);
        expect(getw(s.base, 8) == 5 && nmActions == 5, "counts grow by 3");
        expect(getw(s.acts + 60, 8) == 100 && getw(s.acts + 80, 8) == 101 && getf(s.acts + 60, 0) == 5.0f, "old records moved up three places");
        for (int i = 0; i < 3; ++i) {
            expect(getf(s.acts + 20 * i, 0) == 0.0f && geth(s.acts + 20 * i, 4) == -100 && geth(s.acts + 20 * i, 6) == i, "camera record header");
        }
        expect(getw(s.acts, 8) == 11 && getw(s.acts, 12) == -22 && getw(s.acts, 16) == 33, "truncated camera position");
        expect(getw(s.acts + 20, 8) == 1234 && getw(s.acts + 40, 8) == -4321, "yaw and pitch records");
        // Network mode suppresses the insertion contents (the shift still happens).
        reset_world();
        Scene n = make_scene(2, 1, 0, 3);
        setw(n.acts, 8, 99);
        netNetworkMode = 1; gameCam = &failures;
        cutInitScene__FP9_cutscene(n.base);
        expect(getw(n.acts + 60, 8) == 99 && getw(n.acts, 8) == 99 && ev.find("yaw") == std::string::npos, "network mode: shift only");
        // Bit 4 appends records at the end with a time two after the fourth from last.
        reset_world();
        Scene e = make_scene(4, 4, 0, 3);
        for (int i = 0; i < 4; ++i) setf(e.acts + 20 * i, 0, 10.0f * (i + 1));
        gameCam = &failures;
        cutInitScene__FP9_cutscene(e.base);
        expect(nmActions == 7 && getw(e.base, 8) == 7, "bit 4 adds three");
        for (int i = 0; i < 3; ++i) {
            const unsigned char* r = e.acts + 20 * (4 + i);
            expect(getf(r, 0) == 42.0f && geth(r, 4) == -100 && geth(r, 6) == i, "tail record header");
        }
        expect(getw(e.acts + 80, 8) == 11 && getw(e.acts + 80, 12) == -22 && getw(e.acts + 80, 16) == 33, "tail position record");
        expect(getw(e.acts + 100, 8) == 1234 && getw(e.acts + 120, 8) == -4321, "tail yaw and pitch records");
    }

    // ---- Block 4: dialog directory, animation buffers, music cue.
    {
        reset_world();
        Scene s = make_scene(1, 2, 0, 0);
        std::strcpy((char*)s.base + 116, "tune");
        seth(s.acts, 4, -99); seth(s.acts, 6, 7);
        seth(s.acts + 20, 4, -99); seth(s.acts + 20, 6, 8);
        dlgDir = (char*)take(80 * 6);
        const char* names[5] = {"a", "tune", "x.anm", "y.anm", "w.anm"};
        const int offs[5] = {0, 100, 1000, 3500, 4000};
        for (int i = 0; i < 5; ++i) { std::strcpy(dlgDir + 80 * i, names[i]); setw((unsigned char*)dlgDir + 80 * i, 64, offs[i]); }
        for (int i = 0; i < 4; ++i) setw((unsigned char*)dlgDir + 80 * i, 144 - 0, 0);
        // The record at +144 of entry i is the +64 word of entry i + 1 (80 bytes later).
        for (int i = 0; i < 5; ++i) setw((unsigned char*)dlgDir + 80 * (i + 1), 64, i + 1 < 5 ? offs[i + 1] : 0);
        packet_result = take(1096 * 4);
        std::strcpy((char*)packet_result + 49, "x");
        std::strcpy((char*)packet_result + 1096 + 49, "y");
        std::strcpy((char*)packet_result + 2192 + 49, "zzz");
        nmDialogNodes = 3;
        ev.clear();
        cutInitScene__FP9_cutscene(s.base);
        expect(displayMusicPos == 1, "music entry index");
        expect(ev.find("dialogdir finddialog") != std::string::npos, "directory then packet");
        expect(alloc_n == 2 && alloc_sizes[0] == 4096 && alloc_sizes[1] == 4096, "2500 rounds up to 4096 for both buffers");
        expect(ev.find("alloc(4096,anim1) alloc(4096,anim2)") != std::string::npos, "buffer names");
        expect(anim[0].second == 0 && anim[1].second == 0 && anim[0].block != nullptr && anim[1].block != nullptr, "anim slots set");
        expect(ev.find("stream(2,x.vag,40)") != std::string::npos, "first node stream");
        expect(ev.find("reset(0) music(1,136)") != std::string::npos, "music cue");
        // Largest size wins regardless of order: swap the x/y offsets so y is larger.
        reset_world();
        Scene t = make_scene(1, 0, 0, 0);
        std::strcpy((char*)t.base + 116, "none");
        dlgDir = (char*)take(80 * 4);
        std::strcpy(dlgDir, "y.anm"); std::strcpy(dlgDir + 80, "x.anm"); std::strcpy(dlgDir + 160, "end");
        setw((unsigned char*)dlgDir, 64, 0); setw((unsigned char*)dlgDir + 80, 64, 5000); setw((unsigned char*)dlgDir + 160, 64, 5100);
        setw((unsigned char*)dlgDir + 240, 64, 5300);
        packet_result = take(1096 * 3);
        std::strcpy((char*)packet_result + 49, "x"); std::strcpy((char*)packet_result + 1096 + 49, "y");
        nmDialogNodes = 2;
        cutInitScene__FP9_cutscene(t.base);
        expect(alloc_sizes[0] == 6144 && displayMusicPos == 0, "largest of 5000 and 100 pads to 6144; unknown music stays 0");
        expect(ev.find("music") == std::string::npos, "no cue, no music");
        // A size of exactly 2048 stays 2048.
        reset_world();
        Scene w = make_scene(1, 0, 0, 0);
        std::strcpy((char*)w.base + 116, "none");
        dlgDir = (char*)take(80 * 3);
        std::strcpy(dlgDir, "x.anm"); std::strcpy(dlgDir + 80, "end");
        setw((unsigned char*)dlgDir, 64, 1000); setw((unsigned char*)dlgDir + 80, 64, 3048);
        packet_result = take(1096 * 2);
        std::strcpy((char*)packet_result + 49, "x");
        nmDialogNodes = 1;
        cutInitScene__FP9_cutscene(w.base);
        expect(alloc_sizes[0] == 2048, "exact multiple stays");
        // No dialog directory: nothing allocated.
        reset_world();
        Scene u = make_scene(1, 0, 0, 0);
        packet_result = take(1096);
        cutInitScene__FP9_cutscene(u.base);
        expect(alloc_n == 0 && ev.find("stream") == std::string::npos, "no directory, no buffers");
    }

    // ---- Block 5: actor objects.
    {
        reset_world();
        nmPlayers = 2;
        setf(player, 172, 1.0f); setf(player + 23984, 172, 1.0f);
        block_mem[0] = make_object(0, 0, 0, nullptr); block_mem[1] = make_object(0, 0, 0, nullptr);
        setw(block_mem[0], 12, 0x20); setw(block_mem[1], 12, 0);
        unsigned char* thing = make_object(1, 1, 1, nullptr);
        setw(thing, 12, 0x20);
        created_obj = thing;
        Scene s = make_scene(1, 0, 3, 0);
        const char* nm[3] = {"Player", "Player2", "Thing"};
        for (int i = 0; i < 3; ++i) {
            unsigned char* a = s.actors + 44 * i;
            std::strcpy((char*)a + 8, nm[i]);
            a[43] = 1; a[42] = 0x40 + i;
            float p[3] = {float(i), float(i + 1), float(i + 2)}; std::memcpy(a + 28, p, 12);
            seth(a, 40, 500 + i);
            setw(a, 4, 0x1111);
        }
        unsigned char* tagblock = take(32);
        setw(s.actors + 88, 4, (int)(unsigned long)tagblock);
        eRandState = 0;
        cutInitScene__FP9_cutscene(s.base);
        expect(ev.find("iter-show") != std::string::npos && ev.find("iter-show") < ev.find("block(1408)"), "show pass before any object work");
        expect(player_ctor_calls == 2 && block_calls == 2, "two players built");
        expect(ev.find("block(1408) player(0,0,1) ") != std::string::npos, "player 0 built with index 0 and extra 1");
        expect(ev.find("block(1408) player(1,1,1) ") != std::string::npos, "player 1 built with index 1");
        expect(created_calls == 1 && std::strcmp(created_name, "Thing") == 0 && created_yaw == 502 && created_tags == nullptr, "generic object created with a null tag word (word +4 was just cleared)");
        expect(created_pos[0] == 2.0f && created_pos[1] == 3.0f && created_pos[2] == 4.0f, "creation position");
        expect(getw(s.actors, 0) == (int)(unsigned long)block_mem[0] && getw(s.actors + 44, 0) == (int)(unsigned long)block_mem[1] && getw(s.actors + 88, 0) == (int)(unsigned long)thing, "objects stored");
        expect((getw(block_mem[0], 12) & 0x88000) == 0x88000 && (getw(block_mem[1], 12) & 0x88000) == 0x88000 && (getw(thing, 12) & 0x88000) == 0x88000, "flag bits 0x88000 set");
        expect(player_yaw == 501, "player yaw halfword");
        expect(eRandState == 0x392fa + 2, "random state seeded per actor, last one wins");
        // Virtual call only for objects with 0x20 in the flag word.
        expect(virtual_calls == 2 && virtual_arg == 0, "virtual call for the two flagged objects");
        expect(block_mem[0][569] == 0x40 && thing[569] == 0x42 && block_mem[1][569] == 0, "byte 569 copied from the actor");
        expect(block_mem[0][570] == 1 && thing[570] == 1 && block_mem[1][570] == 0, "bit 0 of byte 570 set only for flagged objects");
        expect(getw(s.actors, 4) == 0 && getw(s.actors + 44, 4) == 0 && getw(s.actors + 88, 4) == 0, "word +4 is cleared for created actors");
        // Players with a non-positive float: no object.
        reset_world();
        nmPlayers = 2;
        setf(player, 172, 0.0f); setf(player + 23984, 172, 0.5f);
        Scene t = make_scene(1, 0, 2, 0);
        std::strcpy((char*)t.actors + 8, "Player"); t.actors[43] = 1;
        std::strcpy((char*)t.actors + 44 + 8, "Player2"); t.actors[44 + 43] = 1;
        block_mem[0] = make_object(0, 0, 0, nullptr); block_mem[1] = make_object(0, 0, 0, nullptr);
        cutInitScene__FP9_cutscene(t.base);
        expect(getw(t.actors, 0) == 0 && getw(t.actors + 44, 0) != 0 && block_calls == 1, "player 0 absent, player 1 present");
        reset_world();
        nmPlayers = 1; setf(player + 23984, 172, 5.0f);
        Scene u = make_scene(1, 0, 1, 0);
        std::strcpy((char*)u.actors + 8, "Player2"); u.actors[43] = 1;
        cutInitScene__FP9_cutscene(u.base);
        expect(getw(u.actors, 0) == 0 && block_calls == 0, "Player2 needs two players");
        // Failed allocation leaves a null object and no constructor call.
        reset_world();
        setf(player, 172, 1.0f);
        Scene v = make_scene(1, 0, 1, 0);
        std::strcpy((char*)v.actors + 8, "Player"); v.actors[43] = 1;
        block_mem[0] = block_mem[1] = nullptr;
        cutInitScene__FP9_cutscene(v.base);
        expect(player_ctor_calls == 0 && getw(v.actors, 0) == 0, "null block: no constructor, null object");
    }

    // ---- Block 6: finding existing objects near the actor position.
    {
        auto run = [&](unsigned char** cands, const char* actor_name, float ax, float ay, float az, unsigned char** out_scene_actor, Scene& s) {
            reset_world();
            s = make_scene(1, 0, 1, 0);
            unsigned char* a = s.actors;
            std::strcpy((char*)a + 8, actor_name);
            float p[3] = {ax, ay, az}; std::memcpy(a + 28, p, 12);
            seth(a, 40, 0x321); a[42] = 7;
            int n = 0;
            for (; cands[n]; ++n) box_results[n] = cands[n];
            box_results[n] = nullptr;
            cutInitScene__FP9_cutscene(s.base);
            *out_scene_actor = a;
        };
        Scene s; unsigned char* a;
        // Named object beats a nearer class match.
        {
            unsigned char* named = make_object(200, 0, 0, "Chest");
            unsigned char* klass = make_object(50, 0, 0, nullptr);
            unsigned char* c[3] = {named, klass, nullptr};
            current_type = "5Chest";
            setw(named, 44, 4242);
            run(c, "Chest", 0, 0, 0, &a, s);
            expect(getw(a, 0) == (int)(unsigned long)named && getw(a, 4) == 4242, "named candidate wins");
            expect(box_low[0] == -240.0f && box_low[1] == -240.0f && box_low[2] == -100000.0f && box_high[0] == 240.0f && box_high[2] == 100000.0f && box_count == 64, "search box");
            expect(type_calls == 0, "class name not consulted after a named match");
            expect(geth(named, 56) == 0x321 && getf(named, 16) == 0.0f && ev.find("grid") != std::string::npos, "winner moved to the actor");
            expect((getw(named, 12) & 0x88000) == 0x88000, "winner flagged");
            expect(ev.find("tag(autoprop) tag(autodoor)") != std::string::npos, "both tags asked");
        }
        // Class-name matches choose the nearest.
        {
            unsigned char* far_ = make_object(100, 0, 0, nullptr);
            unsigned char* near_ = make_object(30, 40, 0, nullptr);
            unsigned char* skip = make_object(1, 0, 0, nullptr);
            setw(skip, 12, 0x80000);
            unsigned char* c[4] = {far_, skip, near_, nullptr};
            current_type = "12Wooden";
            run(c, "Wooden", 0, 0, 0, &a, s);
            expect(getw(a, 0) == (int)(unsigned long)near_, "nearest class match");
            expect(type_calls == 2, "class name asked for the two eligible candidates");
        }
        // A different class name never matches; far candidates are ignored.
        {
            unsigned char* c1 = make_object(10, 0, 0, nullptr);
            unsigned char* c2 = make_object(700, 0, 0, "Wooden");
            unsigned char* c[3] = {c1, c2, nullptr};
            current_type = "7Barrel";
            run(c, "Wooden", 0, 0, 0, &a, s);
            expect(getw(a, 0) == 0 && getw(a, 4) == 0, "nothing within 600 that matches");
        }
        // Autoprop / autodoor: the object stays where it is.
        {
            unsigned char* o = make_object(10, 20, 30, "Door");
            unsigned char* c[2] = {o, nullptr};
            reset_world();
            current_type = "4Door";
            tag_hit[1] = o;
            s = make_scene(1, 0, 1, 0);
            std::strcpy((char*)s.actors + 8, "Door");
            box_results[0] = o; box_results[1] = nullptr;
            tag_hit[1] = o;
            cutInitScene__FP9_cutscene(s.base);
            expect(getf(o, 16) == 10.0f && ev.find("grid") == std::string::npos && (getw(o, 12) & 0x88000) == 0x88000, "autodoor object left in place but flagged");
            (void)c;
        }
    }

    // ---- Block 7: wrap-up.
    {
        for (int fl = 0; fl < 32; ++fl) {
            for (int fe : {0, 1}) for (int subs : {0, 1}) {
                reset_world();
                Scene s = make_scene(fl | 1, 0, 0, 3);
                // Scene flag bit 0 is set so the lookup and the shifts are skipped.
                frontEndRunning = fe; subtitlesOn = subs;
                gameCam = &failures;
                cutInitScene__FP9_cutscene(s.base);
                const bool sleepy = (fl & 8) == 0;
                expect((ev.find("sleep") != std::string::npos) == sleepy, "sleep or remove");
                expect((ev.find("iter-remove") != std::string::npos) == !sleepy, "remove pass");
                const int want_flags = (fl | 1) | ((((fl | 1) & 6) == 0 && fe == 0) ? 0x10 : 0);
                expect(flags == want_flags, "flag bit 0x10 rule");
                expect(engineScreenSlide == ((want_flags & 0x10) && subs ? 16 : 77), "screen slide rule");
                expect(ev.rfind("iter-hide") < ev.rfind(sleepy ? "sleep" : "iter-remove"), "hide pass first");
            }
        }
        // Far clip from the first kind -100 type 6 record with a non-negative time.
        for (float t6 : {0.0f, 1.5f, -2.0f}) {
            reset_world();
            Scene s = make_scene(1, 3, 0, 0);
            seth(s.acts, 4, -100); seth(s.acts, 6, 6); setf(s.acts, 0, -1.0f); setw(s.acts, 8, 99);
            seth(s.acts + 20, 4, -100); seth(s.acts + 20, 6, 5); setf(s.acts + 20, 0, 0.0f); setw(s.acts + 20, 8, 98);
            seth(s.acts + 40, 4, -100); seth(s.acts + 40, 6, 6); setf(s.acts + 40, 0, t6); setw(s.acts + 40, 8, 1000);
            cutInitScene__FP9_cutscene(s.base);
            const bool call = t6 == 0.0f;
            expect((ev.find("far(12000)") != std::string::npos) == call, "far clip is 12 times the word when the time is 0");
        }
    }
    return failures ? 1 : 0;
}
