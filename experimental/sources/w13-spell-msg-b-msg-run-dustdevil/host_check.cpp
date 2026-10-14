// Host contract for msg_run__9DustDevil; not an EE input. The callees log. The authored function was also compared with the
// original in a private R5900 run (112 scenarios over frameNm, the limit word, the two floats and roll lists with rejected
// draws; every original word covered, with the particle and matrix arguments compared byte for byte).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static std::vector<int> rolls;
static unsigned roll_index;
struct Added { void* def; Point3 position, velocity; float age; };
static std::vector<Added> added;
static int rot_axis, rot_angle;
static Point3 trans_point;
#include "msg_run.cpp"
extern "C" {
int frameNm;
unsigned char fxDust[8];
int eRand__10GameObject(void*) { return rolls[roll_index++ % rolls.size()]; }
float icos__Fi(int a) { calls += "cos" + std::to_string(static_cast<short>(a)) + ";"; return 0.5f; }
float isin__Fi(int a) { (void)a; calls += "sin;"; return 0.25f; }
void P_OpenParticleSystem__Fi(int s) { calls += "open" + std::to_string(s) + ";"; }
void P_CloseParticleSystem__Fi(int s) { calls += "close" + std::to_string(s) + ";"; }
void P_SetParticleSystemMatrix__FiP8Matrix34(int s, Matrix34*) { calls += "setm" + std::to_string(s) + ";"; }
void matMakeTrans__FP8Matrix34G6Point3(Matrix34*, Point3 p) { calls += "trans;"; trans_point = p; }
void matRot__FP8Matrix34is(Matrix34*, int axis, short angle) { calls += "rot;"; rot_axis = axis; rot_angle = angle; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float a) { added.push_back({d, *p, *v, a}); }
}

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[256] __attribute__((aligned(16)));
static float getf(int off) { float v; std::memcpy(&v, object + off, 4); return v; }
static void reset(int frame, int limit, float height, float count) {
    std::memset(object, 0, sizeof object);
    calls.clear(); added.clear(); roll_index = 0; rolls = {49152};
    frameNm = frame; std::memcpy(object + 80, &limit, 4); int system = 9; std::memcpy(object + 84, &system, 4);
    setf(object, 76, height); setf(object, 32, count);
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3);
}
int main() {
    reset(5, 5, 10.0f, 1.0f);  // frameNm is not below the limit
    msg_run__9DustDevil(object);
    if (!calls.empty() || getf(76) != 10.0f) return 1;
    reset(5, 6, 10.0f, 0.0f);
    msg_run__9DustDevil(object);
    if (calls != "open9;trans;rot;setm9;close9;" || getf(76) != 12.0f || trans_point.y != 2.0f || rot_axis != 2 || rot_angle != 5 << 12) return 2;
    reset(0x1f, 100, 96.0f, 1.0f);
    msg_run__9DustDevil(object);
    if (getf(76) != 96.0f || rot_angle != static_cast<short>(0xf000)) return 3;
    // one count: two passes. roll 49152 = 0.75: h = 0.75 * 96 = 72, a = (short)49152 = -16384, d = 0.75 * (72 * 0.25 + 16) = 25.5
    if (added.size() != 2 || calls.find("cos-16384;") == std::string::npos) return 4;
    for (const Added& a : added) {
        if (a.def != static_cast<void*>(fxDust) || a.age != 0.0f) return 5;
        if (a.position.x != 0.5f * 25.5f || a.position.y != 0.25f * 25.5f || a.position.z != 72.0f) return 6;
        if (a.velocity.x != 0.5f * w13smb::bits(0x41273333u)) return 7;
    }
    // a first unit-ball draw outside the ball is redrawn
    reset(0, 100, 10.0f, 0.5f);  // (int)0.5 * 2 = 0 passes
    msg_run__9DustDevil(object);
    if (!added.empty() || calls.find("close9;") == std::string::npos) return 8;
    reset(0, 100, 10.0f, 1.0f); rolls = {0, 0, 0, 65535, 65535, 65535, 49152, 49152, 49152};
    msg_run__9DustDevil(object);
    if (added.size() != 2) return 9;
    return 0;
}
