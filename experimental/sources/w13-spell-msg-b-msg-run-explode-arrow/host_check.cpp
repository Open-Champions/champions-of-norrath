// Host contract for msg_run__22ExplodeArrowProjectile; not an EE input. The callees log. The authored function was also
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
unsigned char explode_arrow_msg_run_particle_8d7090[8];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
int move__10ProjectileG6Point3(void*, Point3) { calls += "move;"; return move_result; }
void terminate__22ExplodeArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
}
#include "msg_run.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[256] __attribute__((aligned(16)));
static void reset() {
    std::memset(object, 0, sizeof object);
    calls.clear(); added.clear(); move_result = 0; rolls_used = 0; rolls = {49152};
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3); setf(object, 28, 4); setf(object, 84, 60); setf(object, 88, 120); setf(object, 92, 180);
}
int main() {
    reset(); object[96] = 1;
    msg_run__22ExplodeArrowProjectile(object);
    if (!calls.empty() || object[108] != 0) return 1;
    reset(); move_result = 3;
    msg_run__22ExplodeArrowProjectile(object);
    if (calls != "move;term1;" || !added.empty()) return 2;
    reset(); object[108] = 0x58; object[109] = 0x02;
    msg_run__22ExplodeArrowProjectile(object);
    if (calls != "move;term0;") return 3;
    reset(); object[108] = 0x57; object[109] = 0x02;
    msg_run__22ExplodeArrowProjectile(object);
    if (calls != "move;" || added.size() != 6) return 4;
    auto near = [](float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; };
    for (int i = 0; i < 6; ++i) {
        if (added[i].def != static_cast<void*>(explode_arrow_msg_run_particle_8d7090) || added[i].age != 0.0f) return 10 + i;
        // D = (0.5, 0.5, 0.5), V = 1.5; the fraction draw is 0.75 and the drift is the velocity (60, 120, 180) / 60
        if (added[i].velocity.x != 1.5f || added[i].velocity.y != 1.5f || added[i].velocity.z != 1.5f) return 20 + i;
        if (!near(added[i].position.x, 1.0f + 0.75f * 1.0f) || !near(added[i].position.y, 2.0f + 0.75f * 2.0f) || !near(added[i].position.z, 3.0f + 0.75f * 3.0f)) return 30 + i;
    }
    // a first draw outside the ball is redrawn
    reset(); object[108] = 0x57; object[109] = 0x02; rolls = {65535, 65535, 65535, 49152, 49152, 49152, 0};
    msg_run__22ExplodeArrowProjectile(object);
    if (added.size() != 6 || added[0].velocity.x != 1.5f) return 40;
    return 0;
}
