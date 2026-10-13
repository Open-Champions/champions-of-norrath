// Host contract for msg_draw__7AntWave; not an EE input. The callees log. The authored function was also compared with the
// original in a private R5900 run (42 scenarios over the frame counter, including negative values, and roll lists with
// rejected draws; every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::vector<int> rolls;
static unsigned roll_index;
struct Added { void* def; Point3 position, velocity; float age; };
static std::vector<Added> added;
extern "C" {
unsigned char AntWaveParticle1[8];
int eRand__10GameObject(void*) { return rolls[roll_index++ % rolls.size()]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
}
#include "msg_draw.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[1024] __attribute__((aligned(16)));
static void reset(int n) {
    std::memset(object, 0, sizeof object); added.clear(); roll_index = 0; rolls = {49152};
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3);
    std::memcpy(object + 808, &n, 4);
}
int main() {
    for (int n : {1, 2, 4, 5, -1, -2}) {
        reset(n);
        msg_draw__7AntWave(object);
        if (!added.empty()) return 1;
    }
    for (int n : {0, 3, 6, 300, -3}) {
        reset(n);
        msg_draw__7AntWave(object);
        if (added.size() != 1 || roll_index != 3) return 2;
        // D = (0.5, 0.5, 0.5): P = O+16 + 5
        if (added[0].def != static_cast<void*>(AntWaveParticle1) || added[0].position.x != 6.0f || added[0].position.y != 7.0f || added[0].position.z != 8.0f) return 3;
        if (added[0].velocity.x != 0.0f || added[0].velocity.y != 0.0f || added[0].velocity.z != 0.0f || added[0].age != 0.0f) return 4;
    }
    reset(0); rolls = {65535, 65535, 65535, 49152, 49152, 49152};  // the first draw is outside the ball
    msg_draw__7AntWave(object);
    if (added.size() != 1 || roll_index != 6 || added[0].position.x != 6.0f) return 5;
    return 0;
}
