// Host contract for cutStop__Fv. Not an EE plan input.
#include "cut_stop.cpp"

#include <cstdio>
#include <cstring>
#include <string>

int engineScissory1, engineScissory2, engineScreenSlide;
unsigned char cutMode, cutSkipMode, thereIsACutPlayer, animationLoading;
unsigned char* actor;
int nmActors;
unsigned char player[23984 * 4];
int nmPlayers;
void* world;
int frontEndRunning;
int flags;
unsigned char* dialogActor;
CutAnimSlot anim[2];

static std::string ev;
static int status0, status1_calls;
static int status_values[2];
static std::string tag(const char* name, long a = 0, long b = 0) {
    char buf[96];
    std::snprintf(buf, sizeof buf, "%s(%ld,%ld) ", name, a, b);
    return buf;
}
static unsigned char* objects[8];
static int obj_index(const void* p) {
    for (int i = 0; i < 8; ++i) if (objects[i] == p) return i;
    return -1;
}

extern "C" {
void soundResetStream__Fi(int s) { ev += tag("reset", s); }
int soundGetStreamPlayStatus__Fi(int s) { ev += tag("status", s); return status_values[status1_calls++ & 1]; }
void soundClearStreamQue__Fi(int s) { ev += tag("clear", s); }
void engineRemoveTask__FPFv_v(void (*task)()) { ev += tag("remove", task == cutRunFrameTask__Fv ? 1 : task == cutDrawFrame__Fv ? 2 : 9); }
void cutRunFrameTask__Fv() {}
void cutDrawFrame__Fv() {}
void objectCancelAllAlwaysDraw__Fv() { ev += tag("cancel"); }
void objectUpdateInGrid__FP10GameObject(void* o) { ev += tag("grid", obj_index(o)); }
static int find_calls;
void objectFindEmptySpot__FG6Point3iiPc(float* r, const float* p, int a, int b, char* n) {
    ev += tag("spot", a, b);
    if (n) ev += "name ";
    r[0] = p[0] + 100.0f * float(++find_calls); r[1] = p[1] + 7.0f; r[2] = p[2] - 3.0f;
}
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, const float* p, float extra) {
    ev += tag("height", w == world ? 1 : 0, (long)extra);
    return p[0] + p[1] * 2.0f;
}
void animRemoveNonpermanentAnimations__FP14AnimationState(void* s) { ev += tag("animremove", obj_index((unsigned char*)s - 288)); }
void worldSetFarClip__Ff(float d) { ev += tag("farclip", (long)d); }
void worldSetFrustum__Fffff(float a, float b, float c, float d) {
    unsigned bits; std::memcpy(&bits, &b, 4);
    ev += tag("frustum", (long)a, (long)c); ev += (bits == 0x3faa3d70u && d == 1.0f) ? "ok " : "BAD ";
}
void lightGlobalCharLightScale__Ff(float s) { ev += tag("lightscale", (long)s); }
void objectIterateRunLists__FPFP10GameObject_v(void (*cb)(void*)) { ev += tag("iterate", cb == unremoveMonster__FP10GameObject); }
void unremoveMonster__FP10GameObject(void*) {}
void wakeMonsters__8creaturev() { ev += tag("wake"); }
void cdWait__Fv() { ev += tag("cdwait", animationLoading); }
void memFree__FPv(void* b) { ev += tag("free", (long)(((unsigned char*)b - (unsigned char*)0) & 0xff)); }
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}

static int virtual_calls, virtual_arg, virtual_self;
static unsigned char* virtual_object;
static void virtual_target(void* self, int a) { ++virtual_calls; virtual_arg = a; virtual_self = (int)((unsigned char*)self - virtual_object); }

struct Obj { unsigned char b[700]; };
static float rf(const unsigned char* p, int o) { float v; std::memcpy(&v, p + o, 4); return v; }
static void wf(unsigned char* p, int o, float v) { std::memcpy(p + o, &v, 4); }

int main() {
    static Obj obj[8];
    for (int i = 0; i < 8; ++i) objects[i] = obj[i].b;
    static unsigned char actors[44 * 3];
    static unsigned char vtable[32];
    VirtualEntry entry{12, 0, virtual_target};
    std::memcpy(vtable + 8, &entry, sizeof entry);
    int cases = 0;
    for (int stat : {0, 0x100, 0x80, 0x180})
    for (int fe : {0, 1})
    for (int fl : {0, 8, 0x10, 0x18})
    for (int loading : {0, 1})
    for (int np : {1, 3})
    for (int act_kind : {0, 1, 2, 3}) {
        std::memset(actors, 0, sizeof actors);
        for (int i = 0; i < 8; ++i) { std::memset(obj[i].b, 0, sizeof obj[i].b); for (int k = 0; k < 8; ++k) wf(obj[i].b, 16 + 4 * (k % 3), float(10 * i + k)); }
        std::memset(player, 0, sizeof player);
        for (int j = 0; j < 4; ++j) {
            unsigned char* o = j < 3 ? obj[1 + j].b : nullptr;
            std::memcpy(player + 23984 * j + 21872, &o, sizeof o);
            wf(player + 23984 * j, 172, j == 2 ? 0.0f : 1.0f);
        }
        // actor 0: the player actor; actor 1: an ordinary actor; actor 2: depends on kind.
        unsigned char* o4 = obj[4].b;
        std::memcpy(actors, &o4, sizeof o4);
        std::strcpy((char*)actors + 8, "Player");
        unsigned short hw = 0xf000;
        std::memcpy(obj[4].b + 56, &hw, 2);
        unsigned char* o5 = obj[5].b;
        std::memcpy(actors + 44, &o5, sizeof o5);
        std::strcpy((char*)actors + 44 + 8, "Other");
        actors[44 + 43] = act_kind & 1;
        unsigned flagword = (act_kind & 2) ? 0x20u | 0x00088000u | 0x5u : 0x00088000u;
        std::memcpy(obj[5].b + 12, &flagword, 4);
        obj[5].b[570] = 0xff;
        virtual_object = obj[5].b;
        unsigned char* tbl = vtable;
        std::memcpy(obj[5].b + 72, &tbl, sizeof tbl);
        virtual_calls = 0;
        std::memcpy(actors + 88, &o4, sizeof o4);   // third actor: the player actor again, flag set
        std::strcpy((char*)actors + 88 + 8, "Player");
        actors[88 + 43] = 0;
        std::memcpy(obj[4].b + 12, &flagword, 4);
        actor = actors; nmActors = 3; nmPlayers = np; world = (void*)0x1234; frontEndRunning = fe; flags = fl;
        engineScreenSlide = 77; animationLoading = (unsigned char)loading;
        static unsigned char block0[8], block1[8];
        anim[0].block = block0; anim[0].second = 5; anim[1].block = act_kind == 3 ? nullptr : block1; anim[1].second = 6;
        static unsigned char dlg[200];
        std::memset(dlg, 0xff, sizeof dlg);
        dialogActor = act_kind == 1 ? nullptr : dlg;
        status_values[0] = stat; status_values[1] = stat; status1_calls = 0; find_calls = 0;
        ev.clear();
        cutStop__Fv();
        ++cases;

        std::string want;
        want += tag("reset", 2);
        want += tag("status", 0);
        if (!(stat & 0x100)) {
            want += tag("status", 0);
            want += (stat & 0x80) ? tag("reset", 0) : tag("clear", 0);
        }
        want += tag("remove", 1) + tag("remove", 2) + tag("cancel");
        expect(ev.compare(0, want.size(), want) == 0, "sound and task prologue");
        expect(engineScissory2 == 447 && cutMode == 0 && thereIsACutPlayer == 0 && cutSkipMode == 0 && engineScissory1 == 0, "state words");
        expect(ev.find("grid(1,0)") != std::string::npos, "first player object updated in the grid");
        // Player 0 is object 1: position copied from actor object 4, halfword copied.
        unsigned short got_hw;
        std::memcpy(&got_hw, obj[1].b + 56, 2);
        expect(got_hw == 0xf000, "player 0 halfword");
        if (np == 3) {
            // player 1 = object 2 (spot call, height call, 8000 offset), player 2 = object 3 skipped (float 0).
            std::memcpy(&got_hw, obj[2].b + 56, 2);
            expect(got_hw == (unsigned short)(0xf000 + 8000), "player 1 halfword plus 8000");
            expect(ev.find("spot(36,14) height(1,12) grid(2,0)") != std::string::npos || ev.find("spot(36,14)") != std::string::npos, "spot and height for player 1");
            expect(obj_index(player + 0) < 0, "sanity");
            expect(ev.find("grid(3,0)") == std::string::npos, "player with zero float is skipped");
        }
        // Third actor repeats the pass with the same player actor.
        size_t first = ev.find("grid(1,0)");
        expect(first != std::string::npos && ev.find("grid(1,0)", first + 1) != std::string::npos, "player pass is run per matching actor");
        // Release part.
        if (act_kind & 1) expect(virtual_calls == 1 && virtual_arg == 3 && virtual_self == 12, "virtual call through the +8 entry");
        else {
            expect(virtual_calls == 0, "no virtual call without the flag");
            unsigned got; std::memcpy(&got, obj[5].b + 12, 4);
            expect(got == (flagword & 0xfff77fffu), "flag word bits cleared");
            expect(obj[5].b[570] == ((act_kind & 2) ? 0xfe : 0xff), "bit 0 of byte 570 only with the 0x20 flag");
        }
        expect((ev.find("animremove(1,0)") != std::string::npos) == (fe == 0), "animations removed only outside the front end");
        if (fe == 0) {
            expect((ev.find("animremove(2,0)") != std::string::npos) == (np == 3), "second player animations removed");
            expect(ev.find("animremove(3,0)") == std::string::npos, "zero-float player skipped for animations");
        }
        expect(ev.find("farclip(1800,0)") != std::string::npos && ev.find("frustum(50,80) ok ") != std::string::npos && ev.find("lightscale(1,0)") != std::string::npos, "far clip, frustum, light scale");
        expect((ev.find("iterate(1,0)") != std::string::npos) == ((fl & 8) != 0) && (ev.find("wake") != std::string::npos) == ((fl & 8) == 0), "run lists or wake");
        expect(engineScreenSlide == ((fl & 0x10) ? 0 : 77), "screen slide");
        expect(anim[0].block == nullptr && anim[0].second == 0 && anim[1].block == nullptr && anim[1].second == (act_kind == 3 ? 6 : 0), "anim slots cleared");
        expect((ev.find("cdwait(0,0)") != std::string::npos) == (loading != 0), "cd wait after clearing the flag, once");
        expect(animationLoading == 0 || loading == 0, "loading flag cleared");
        if (act_kind == 1) expect(dialogActor == nullptr, "dialog actor null stays null");
        else { expect(dialogActor == nullptr, "dialog actor cleared"); expect(dlg[160] == 0xff && dlg[163] == 0xef && dlg[164] == 0xff, "bit 28 cleared in the doubleword at +160"); }
    }
    expect(cases == 4 * 2 * 4 * 2 * 2 * 4, "all cases ran");
    return failures ? 1 : 0;
}
