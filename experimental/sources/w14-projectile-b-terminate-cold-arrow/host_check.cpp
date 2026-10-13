// Host contract for terminate__19ColdArrowProjectilei; not an EE input. The callees log. The authored function was also compared
// with the original in a private R5900 run (92 scenarios over the done word, argument, level number, heading, rolls including
// rejected draws; every original word but one padding word covered; particle position and velocity bytes compared).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls, lump_b;
static int roll;
static int rolls_used;
struct Added { void* def; Point3 position, velocity; float age; Point3* position_address; };
static std::vector<Added> added;
static void* the_object;
extern "C" {
unsigned char pack[8], coldSmoke[8], coldSpeck[8];
int eRand__10GameObject(void*) { ++rolls_used; return roll; }
float icos__Fi(int) { return 10.0f; }
float isin__Fi(int) { return 20.0f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a, p}); }
void* lumpFindResource__FPcT0(const char*, const char* b) { calls += "lump;"; lump_b = b; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void* o, float v) { calls += (v == 1.0f && o == the_object) ? "sound;" : "badsound;"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
}
#include "terminate.cpp"

static unsigned char object[256] __attribute__((aligned(16)));
static void setf(int off, float v) { std::memcpy(object + off, &v, 4); }
static void reset(unsigned done, int n, float bx) {
    std::memset(object, 0, sizeof object);
    the_object = object;
    calls.clear(); added.clear(); roll = 49152; rolls_used = 0;
    std::memcpy(object + 96, &done, 4);
    std::memcpy(object + 104, &n, 4);
    setf(16, 1); setf(20, 2); setf(24, 3); setf(28, 2); setf(84, bx); setf(88, 0);
}
static bool same(Point3 p, float x, float y, float z) { return p.x == x && p.y == y && p.z == z; }
int main() {
    reset(7, 1, 0);
    terminate__19ColdArrowProjectilei(object, 1);
    if (!calls.empty() || !added.empty() || rolls_used != 0) return 1;
    reset(0, 1, 0);
    terminate__19ColdArrowProjectilei(object, 0);
    if (calls != "del;" || !added.empty() || object[96] != 1 || rolls_used != 0) return 2;
    // h == 1, n == 1: 37 pack particles, then 10 specks and 3 smoke puffs.
    reset(0, 1, 0);
    terminate__19ColdArrowProjectilei(object, 1);
    if (calls != "lump;sound;del;" || lump_b != "spel_we_coldarrow_hit.anm" || object[96] != 1) return 3;
    if (added.size() != 37 + 13) return 4;
    for (int i = 0; i < 37; ++i) {
        if (added[i].def != pack || added[i].age != 0.0f || !same(added[i].velocity, 0, 0, 0)) return 10;
        // C = (1 + 10*2, 2 + 20*2, 3); D = (0.5, 0.5, 0.5); m = 1/3 + 2 = 2
        if (!same(added[i].position, 22.0f, 43.0f, 4.0f)) return 11;
    }
    int smoke = 0;
    for (int i = 37; i < 50; ++i) {
        if (added[i].position_address != reinterpret_cast<Point3*>(object + 16)) return 12;
        if (added[i].def == coldSmoke) { ++smoke; if (!same(added[i].velocity, 15.5f * 0.75f, 15.5f * 0.75f, 15.5f * 0.75f)) return 13; }
        else if (added[i].def != coldSpeck || !same(added[i].velocity, 15.5f, 15.5f, 15.5f)) return 14;
    }
    if (smoke != 3 || added[37].def != coldSpeck || added[38].def != coldSmoke || added[39].def != coldSpeck) return 15;
    // each of the 37 + 10 draws takes 3 rolls; the specks draw two vectors
    if (rolls_used != 37 * 3 + 10 * 6) return 16;
    // a velocity pointing along B flips x and y of the draw
    reset(0, 1, 4);
    terminate__19ColdArrowProjectilei(object, 1);
    if (!same(added[0].position, 20.0f, 41.0f, 4.0f) || !same(added[37].velocity, -15.5f, -15.5f, 15.5f)) return 17;
    // h == 2 skips the pack ring
    reset(0, 1, 0);
    terminate__19ColdArrowProjectilei(object, 2);
    if (added.size() != 13 || calls != "lump;sound;del;") return 18;
    // a level number of 0 draws only the 27 pack particles; negative numbers draw none
    reset(0, 0, 0);
    terminate__19ColdArrowProjectilei(object, 1);
    if (added.size() != 27) return 19;
    reset(0, -3, 0);
    terminate__19ColdArrowProjectilei(object, 1);
    if (!added.empty() || calls != "lump;sound;del;") return 20;
    return 0;
}
