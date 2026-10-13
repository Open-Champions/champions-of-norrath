// Host contract for msg_run__8GutChunk; not an EE input. 32-bit words are offsets from a host base; every callee
// logs its arguments and eRand returns scripted rolls. Each scenario lists the exact call log the rule in
// msg_run.cpp gives. The authored function was also compared with the original in a private R5900 run (500
// scenarios). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_a_host_base;
#include "msg_run.cpp"

int g_BloodLevel;
void* world;
unsigned char particleBloodDrop[80], particleEctoDrop[80], particleSlimeDrop[80];
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::vector<unsigned> rolls;
static size_t nextRoll;
static float heightResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
unsigned w11_actor_a_host_call(unsigned fn, void* adjusted, int argc, unsigned long a1, unsigned long) {
    calls += "vcall(" + num(fn) + "," + num(static_cast<unsigned char*>(adjusted) - obj) + "," + num(argc) + "," + num(a1) + ");"; return 0;
}
extern "C" {
void msg_run__6PhProp(void* s) { calls += "run" + std::to_string(static_cast<unsigned char*>(s) - arena) + ";"; }
unsigned eRand__10GameObject(void*) { calls += "r;"; return rolls[nextRoll++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    calls += std::string("add(") + (d == particleSlimeDrop ? "slime" : d == particleEctoDrop ? "ecto" : d == particleBloodDrop ? "blood" : "?") + "," + pt(*p) + "," + pt(*v) + "," + num(age) + ");";
}
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 p, float f) { calls += std::string("height(") + (w == world ? "w" : "?") + "," + pt(p) + "," + num(f) + ");"; return heightResult; }
void floorAdd__FG6Point3sfii(Point3 p, short angle, float size, int a, int b) { calls += "floor(" + pt(p) + "," + num(angle) + "," + num(size) + "," + num(a) + "," + num(b) + ");"; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return arena + 0x3000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float v) { calls += "sound(" + num(static_cast<unsigned char*>(a) - arena) + "," + num(static_cast<unsigned char*>(o) - arena) + "," + num(v) + ");"; }
}
struct Scenario {
    const char* name;
    float A, G; unsigned F, C; unsigned char H; float f444, f448, pz, height; int blood; std::vector<unsigned> rolls; const char* want;
    float wantG; unsigned wantC; unsigned char wantH;
};
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
int main() {
    int bad = 0;
    const Scenario list[] = {
        {"quiet", 0.5f, 0.5f, 0, 0, 0, 0.05f, 0.05f, 1, 5, 1, {1},
         "run4096;height(w,1:2:3,8);", 0.5f, 0, 1},
        {"tail drop plain", 0.5f, 0.5f, 0, 0, 0, 0.05f, 0.05f, 1, 0.3f, 1, {65537},
         "run4096;height(w,1:2:3,8);r;floor(1:2:0.3,1,12,0,1);", 0.5f, 0, 1},
        {"tail drop bit 4", 0.5f, 0.5f, 4, 0, 0, 0.05f, 0.05f, 1, 0.3f, 1, {7},
         "run4096;height(w,1:2:3,8);r;floor(1:2:0.3,7,12,3,1);", 0.5f, 0, 1},
        {"tail drop bit 2", 0.5f, 0.5f, 2, 0, 0, 0.05f, 0.05f, 1, 0.3f, 1, {7},
         "run4096;height(w,1:2:3,8);r;floor(1:2:0.3,7,12,6,1);", 0.5f, 0, 1},
        {"tail needs a clear flag byte", 0.5f, 0.5f, 0, 0, 1, 0.05f, 0.05f, 1, 0.3f, 1, {7},
         "run4096;height(w,1:2:3,8);", 0.5f, 0, 1},
        {"tail needs both floats below the limit", 0.5f, 0.5f, 0, 0, 0, 0.05f, 0.5f, 1, 0.3f, 1, {7},
         "run4096;height(w,1:2:3,8);", 0.5f, 0, 0},
        {"tail bit 8 sets the flag only", 0.5f, 0.5f, 8, 0, 0, 0.05f, 0.05f, 1, 0.3f, 1, {7},
         "run4096;height(w,1:2:3,8);", 0.5f, 0, 1},
        {"blood drop", 2.5f, 0.5f, 0, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {9},
         "run4096;r;add(blood,1:2:3,0:0:0,0);height(w,1:2:3,8);", 2.5f, 0, 1},
        {"ecto drop", 2.5f, 0.5f, 2, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {3},
         "run4096;r;add(ecto,1:2:3,0:0:0,0);height(w,1:2:3,8);", 2.5f, 0, 1},
        {"slime drop", 2.5f, 0.5f, 4, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {0},
         "run4096;r;add(slime,1:2:3,0:0:0,0);height(w,1:2:3,8);", 2.5f, 0, 1},
        {"no drop when the roll is not divisible", 2.5f, 0.5f, 4, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {4},
         "run4096;r;height(w,1:2:3,8);", 2.5f, 0, 1},
        {"no drop without blood", 2.5f, 0.5f, 4, 0, 1, 0.5f, 0.5f, 1, 0.3f, 0, {3},
         "run4096;r;height(w,1:2:3,8);", 2.5f, 0, 1},
        {"landing splash, bit 4", -1.5f, -0.5f, 5, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {100, 3},
         "run4096;height(w,1:2:3,8);r;r;floor(1:2:0.3,100,12,5,0);", -1.5f, 0, 1},
        {"landing splash, bit 2", -1.5f, -0.5f, 3, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {100, 2},
         "run4096;height(w,1:2:3,8);r;r;floor(1:2:0.3,100,12,7,0);", -1.5f, 0, 1},
        {"landing splash, plain", -1.5f, -0.5f, 1, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {100, 3},
         "run4096;height(w,1:2:3,8);r;r;floor(1:2:0.3,100,12,2,0);", -1.5f, 0, 1},
        {"counter step, no splash", 0.5f, -0.5f, 0, 3, 1, 0.5f, 0.5f, 1, 5, 1, {5, 6},
         "run4096;height(w,1:2:3,8);", 0.5f, 2, 1},
        {"counter step with a drop", 0.5f, -0.5f, 4, 3, 1, 0.5f, 0.5f, 1, 0.3f, 1, {300, 6},
         "run4096;height(w,1:2:3,8);r;r;floor(1:2:0.3,300,12,4,0);", 0.5f, 2, 1},
        {"counter at zero", 0.5f, -0.5f, 0, 0, 1, 0.5f, 0.5f, 1, 0.3f, 1, {300, 6},
         "run4096;height(w,1:2:3,8);", 0.5f, 0, 1},
        {"counter with bit 8", 0.5f, -0.5f, 8, 3, 1, 0.5f, 0.5f, 1, 0.3f, 1, {300, 6},
         "run4096;height(w,1:2:3,8);", 0.5f, 2, 1},
        {"deep sound", 0.5f, -1.5f, 0, 0, 1, 0.5f, 0.5f, 1, 5, 1, {1},
         "run4096;height(w,1:2:3,8);lump(fx.lmp,bf_water.anm);sound(12288,4096,0.5);", 0.5f, 0, 1},
        {"self delete", 0.5f, -1.5f, 1, 0, 1, 0.5f, 0.5f, 1, 5, 1, {1},
         "run4096;height(w,1:2:3,8);lump(fx.lmp,bf_water.anm);sound(12288,4096,0.5);vcall(8192,16,1,3);", -1.5f, 0, 1},
    };
    for (const Scenario& s : list) {
        std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
        put32(72, 0x2000); *reinterpret_cast<short*>(arena + 0x2000 + 8) = 16;
        std::memcpy(arena + 0x2000 + 12, "\x00\x20\x00\x00", 4);
        float p[3] = {1, 2, 3}; std::memcpy(obj + 16, p, 12); putf(24, s.pz);
        putf(452, s.A); putf(588, s.G); put32(584, s.F); put32(604, s.C); obj[608] = s.H; putf(444, s.f444); putf(448, s.f448);
        g_BloodLevel = s.blood; world = arena + 0x3800; heightResult = s.height; rolls = s.rolls; nextRoll = 0;
        msg_run__8GutChunk(obj);
        std::string want = s.want;
        // The expected strings name the object as offset 4096 and use pz instead of 3 for the height call's z.
        for (size_t at = 0; (at = want.find("1:2:0.3", at)) != std::string::npos; at += 7) want.replace(at, 7, "1:2:h");
        for (size_t at = 0; (at = want.find("1:2:3", at)) != std::string::npos; at += 5) want.replace(at, 5, "1:2:z");
        for (size_t at = 0; (at = want.find("1:2:h")) != std::string::npos;) want.replace(at, 5, "1:2:" + num(s.height) + "#");
        for (size_t at = 0; (at = want.find("1:2:z")) != std::string::npos;) want.replace(at, 5, "1:2:" + num(s.pz) + "#");
        for (size_t at = 0; (at = want.find('#')) != std::string::npos;) want.erase(at, 1);
        float G; std::memcpy(&G, obj + 588, 4); unsigned C; std::memcpy(&C, obj + 604, 4);
        const bool deleted = (s.F & 1) && s.G < 0 && s.A > 0 && s.G < -1;
        if (calls != want || (!deleted && G != s.A) || (deleted && G != s.G) || C != s.wantC || (!deleted && obj[608] != s.wantH)) {
            std::printf("fail: %s\n  want %s\n  got  %s\n  G=%g C=%u H=%u\n", s.name, want.c_str(), calls.c_str(), G, C, obj[608]); bad = 1;
        }
    }
    return bad;
}
