// Host contract for msg_run__19MultiFireProjectile; not an EE input. The callees log. The authored function was also
// compared with the original in a private R5900 run (38 scenarios: skip word, age limits, hit bits, rejection
// draws; every original word covered, particle arguments compared byte for byte).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static int move_result, rolls_used;
static std::vector<int> rolls;
struct Added { void* def; Point3 position, velocity; float age; };
static std::vector<Added> added;
extern "C" {
unsigned char multifire_msg_run_particle_8d7040[8];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
int move__10ProjectileG6Point3(void*, Point3) { calls += "move;"; return move_result; }
void terminate__19MultiFireProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
void P_ExtendTrail__FiG6Point3(int handle, Point3 p) { calls += "trail" + std::to_string(handle) + "@" + std::to_string(static_cast<int>(p.x)) + ";"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
}
#include "msg_run.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[256] __attribute__((aligned(16)));
static void reset() {
    std::memset(object, 0, sizeof object);
    calls.clear(); added.clear(); move_result = 0; rolls_used = 0; rolls = {49152};
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3); setf(object, 28, 4); object[116] = 7;
}
int main() {
    reset(); object[96] = 1;
    msg_run__19MultiFireProjectile(object);
    if (!calls.empty() || object[108] != 0) return 1;
    reset(); move_result = 3;
    msg_run__19MultiFireProjectile(object);
    if (calls != "trail7@1;move;term1;" || !added.empty()) return 2;
    reset(); object[108] = 0x58; object[109] = 0x02;
    msg_run__19MultiFireProjectile(object);
    if (calls != "trail7@1;move;term0;") return 3;
    reset(); object[108] = 0x57; object[109] = 0x02;
    msg_run__19MultiFireProjectile(object);
    if (calls != "trail7@1;move;" || added.size() != 10) return 4;
    for (int i = 0; i < 10; ++i) {
        if (added[i].def != static_cast<void*>(multifire_msg_run_particle_8d7040) || added[i].age != 0.0f) return 10 + i;
        // D = (0.5, 0.5, 0.5), V = 4; the fraction draw 49152 * 2^-16 = 0.75, so P = pos + 3
        if (added[i].velocity.x != 4.0f || added[i].velocity.y != 4.0f || added[i].velocity.z != 4.0f) return 20 + i;
        if (added[i].position.x != 4.0f || added[i].position.y != 5.0f || added[i].position.z != 6.0f) return 30 + i;
    }
    // a first draw outside the ball is redrawn
    reset(); object[108] = 0x57; object[109] = 0x02; rolls = {65535, 65535, 65535, 49152, 49152, 49152, 0};
    msg_run__19MultiFireProjectile(object);
    if (added.size() != 10 || added[0].velocity.x != 4.0f || added[0].position.x != 1.0f) return 40;
    return 0;
}
