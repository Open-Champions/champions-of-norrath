// Host contract for msg_run__21PoisonArrowProjectile; not an EE input. The callees log. The authored function was also
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
unsigned char poisonSmoke[8], poisonFleck[8];
int eRand__10GameObject(void*) { return rolls[rolls_used++ % rolls.size()]; }
int move__10ProjectileG6Point3(void*, Point3) { calls += "move;"; return move_result; }
void terminate__21PoisonArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
}
#include "msg_run.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[256] __attribute__((aligned(16)));
static void reset() {
    std::memset(object, 0, sizeof object);
    calls.clear(); added.clear(); move_result = 0; rolls_used = 0; rolls = {49152};
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3); setf(object, 28, 4);
}
int main() {
    reset(); object[96] = 1;
    msg_run__21PoisonArrowProjectile(object);
    if (!calls.empty() || object[108] != 0) return 1;
    reset(); move_result = 3;
    msg_run__21PoisonArrowProjectile(object);
    if (calls != "move;term1;" || !added.empty()) return 2;
    reset(); object[108] = 0x58; object[109] = 0x02;
    msg_run__21PoisonArrowProjectile(object);
    if (calls != "move;term0;") return 3;
    reset(); object[108] = 0x57; object[109] = 0x02;
    msg_run__21PoisonArrowProjectile(object);
    if (calls != "move;light;" || added.size() != 46) return 4;
    float copied[3]; std::memcpy(copied, object + 160, 12);
    if (copied[0] != 1.0f || copied[1] != 2.0f || copied[2] != 3.0f) return 5;
    for (int i = 0; i < 46; ++i) {
        if (added[i].def != ((i & 1) ? static_cast<void*>(poisonSmoke) : static_cast<void*>(poisonFleck))) return 10 + i;
        if (added[i].velocity.x != 15.0f || added[i].velocity.z != 15.0f || added[i].age != 0.0f) return 70;
        // E = (0.5, 0.5, 0.5), r = 4: offset 2, scaled by 0.50799495
        const float shift = 2.0f * w13smb::bits(0x3f020bf5u);
        if (added[i].position.x != 1.0f + shift || added[i].position.y != 2.0f + shift || added[i].position.z != 3.0f + shift) return 71;
    }
    // a first draw outside the ball is redrawn: three near-1 components, then the usual ones
    reset(); object[108] = 0x57; object[109] = 0x02; rolls = {65535, 65535, 65535, 49152, 49152, 49152};
    msg_run__21PoisonArrowProjectile(object);
    if (added.size() != 46 || added[0].velocity.x != 15.0f) return 40;
    return 0;
}
