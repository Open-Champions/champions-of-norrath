// Host contract for msg_run__5PhIce; not an EE input. The callees log. The authored function was also compared
// with the original in a private R5900 run (34 scenarios over the counters and the flag, with the rolls, particle
// calls, soft-double routines and parent call logged; every one of its 158 words reached).
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
extern "C" { unsigned char phice_msg_run_particle_7d6740[80]; unsigned char phice_msg_run_particle_7d6790[80]; }
#include "msg_run.cpp"

static std::string calls;
static int rolls[2000], rollIndex, rollCount;
static int particles;
static float lastVelocity[3];
extern "C" {
int eRand__10GameObject(void*) { calls += "r;"; return rolls[rollIndex++ % rollCount]; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float s) {
    ++particles;
    std::memcpy(lastVelocity, v, 12);
    calls += d == phice_msg_run_particle_7d6740 ? "puff;" : "hit;";
    if (s != 0.0f || p == nullptr) calls += "badarg;";
}
void msg_run__6PhProp(void*) { calls += "parent;"; }
unsigned long long fptodp(float v) { double d = v; unsigned long long b; std::memcpy(&b, &d, 8); return b; }
unsigned long long dpmul(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double r = x * y; unsigned long long o; std::memcpy(&o, &r, 8); return o; }
float dptofp(unsigned long long a) { double x; std::memcpy(&x, &a, 8); return static_cast<float>(x); }
}

static unsigned char obj[1024] __attribute__((aligned(16)));
static void put32(int off, int v) { std::memcpy(obj + off, &v, 4); }
static void putf(int off, float v) { std::memcpy(obj + off, &v, 4); }
static int get32(int off) { int v; std::memcpy(&v, obj + off, 4); return v; }
static void reset(int flag, int c, int b) {
    std::memset(obj, 0, sizeof obj); calls.clear(); rollIndex = 0; particles = 0;
    putf(400, 2.0f); putf(444, 1.0f); putf(448, 0.5f); putf(452, 0.25f);
    put32(560, flag); put32(584, c); put32(588, b);
}

int main() {
    int bad = 0;
    // rolls: 0 passes the three-bit test; (32768, 32768, 32768) is the centre of the ball, then 40000 gives a magnitude
    rolls[0] = 0; rollCount = 1;
    reset(0, 10, 0);
    msg_run__5PhIce(obj);
    if (calls != "r;puff;parent;" || get32(584) != 11 || get32(588) != 0) { std::puts(calls.c_str()); std::puts("fail: puff only"); bad = 1; }
    rolls[0] = 3;
    reset(0, 10, 0);
    msg_run__5PhIce(obj);
    if (calls != "r;parent;") { std::puts(calls.c_str()); std::puts("fail: low bits non-zero"); bad = 1; }
    reset(0, 90, 0);
    msg_run__5PhIce(obj);
    if (calls != "parent;" || get32(584) != 91) { std::puts(calls.c_str()); std::puts("fail: counter at 90"); bad = 1; }
    // burst: all rolls 32768 -> the direction is (0, 0, 0), the magnitude 0.5 * 20 = 10; the velocity is (0 + V.x, 0 + V.y, 0)
    rolls[0] = 32768; rollCount = 1;
    reset(1, 90, 2);
    msg_run__5PhIce(obj);
    std::string want;
    for (int i = 0; i < 30; ++i) want += "r;r;r;r;hit;";
    want += "parent;";
    if (calls != want || particles != 30 || get32(588) != 3) { std::puts(calls.c_str()); std::puts("fail: burst"); bad = 1; }
    if (lastVelocity[0] != 2.0f || lastVelocity[1] != 1.0f || lastVelocity[2] != 0.0f) { std::puts("fail: burst velocity"); bad = 1; }
    reset(1, 90, 3);
    msg_run__5PhIce(obj);
    if (calls != "parent;" || get32(588) != 3) { std::puts("fail: third burst"); bad = 1; }
    reset(0, 90, 0);
    put32(560, 0);
    msg_run__5PhIce(obj);
    if (particles != 0) { std::puts("fail: flag clear"); bad = 1; }
    // rejection: first draw (65535, 65535, 65535) is outside the ball (each about 1), then the centre is accepted
    {
        static const int seq[] = {65535, 65535, 65535, 32768, 32768, 32768, 32768};
        for (int i = 0; i < 7; ++i) rolls[i] = seq[i];
        rollCount = 7;
        reset(1, 90, 2);
        msg_run__5PhIce(obj);
        if (calls.compare(0, 14, "r;r;r;r;r;r;r;") != 0) { std::puts(calls.c_str()); std::puts("fail: rejection"); bad = 1; }
    }
    // the third velocity component is never negative: rolls (32768, 32768, 0 -> z = -1) with magnitude roll 65535
    {
        static const int seq[] = {32768, 32768, 0, 65535};
        for (int i = 0; i < 4; ++i) rolls[i] = seq[i];
        rollCount = 4;
        reset(1, 90, 2);
        msg_run__5PhIce(obj);
        if (lastVelocity[2] <= 0.0f) { std::puts("fail: z sign"); bad = 1; }
    }
    return bad;
}
