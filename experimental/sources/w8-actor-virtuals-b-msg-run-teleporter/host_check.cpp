#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#include "msg_run.cpp"

extern "C" const unsigned char teleportParticle1[4] = {1, 0, 0, 0};
extern "C" const unsigned char teleportParticle2[4] = {2, 0, 0, 0};

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

struct Call {
    const void* definition;
    float position[3];
    float velocity[3];
    float extra;
};
static std::vector<Call> particles;
static std::vector<int> log_calls;  // 1 activation, 2 rand, 3 icos, 4 isin, 5 particle
static std::vector<int> rands;
static unsigned next_rand;
static int check_value;
static void* check_object;
static std::vector<int> yaws;

extern "C" int objectActivationCheck__8Creaturei(void* object, int value) {
    log_calls.push_back(1);
    check_object = object;
    check_value = value;
    return 0;
}
extern "C" int eRand__10GameObject(void*) {
    log_calls.push_back(2);
    return rands[next_rand++];
}
extern "C" float icos__Fi(int angle) {
    log_calls.push_back(3);
    yaws.push_back(angle);
    return static_cast<float>(std::cos(angle * 3.14159265358979 / 32768.0));
}
extern "C" float isin__Fi(int angle) {
    log_calls.push_back(4);
    yaws.push_back(angle);
    return static_cast<float>(std::sin(angle * 3.14159265358979 / 32768.0));
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, float* position, float* velocity,
                                                          float extra) {
    log_calls.push_back(5);
    Call c;
    c.definition = definition;
    std::memcpy(c.position, position, sizeof c.position);
    std::memcpy(c.velocity, velocity, sizeof c.velocity);
    c.extra = extra;
    particles.push_back(c);
}

static bool near(double a, double b) { return std::fabs(a - b) <= 1e-3 + 1e-5 * std::fabs(b); }

int main() {
    alignas(8) unsigned char object[128];
    std::memset(object, 0, sizeof object);
    float origin[3] = {100.0f, -250.5f, 12.25f};
    std::memcpy(object + 16, origin, sizeof origin);
    short yaw = -9000;
    std::memcpy(object + 56, &yaw, sizeof yaw);
    rands = {49152, 16384, 0, 65535};
    next_rand = 0;
    msg_run__10Teleporter(object);

    expect(check_object == object && check_value == 600, "activation check gets the object and 600");
    const std::vector<int> expected_order = {1, 2, 3, 4, 2, 5, 2, 3, 4, 2, 5};
    expect(log_calls == expected_order, "call order");
    expect(particles.size() == 2u, "two particles");
    expect(next_rand == 4u, "four random draws");
    if (particles.size() == 2u) {
        expect(particles[0].definition == teleportParticle1, "first definition");
        expect(particles[1].definition == teleportParticle2, "second definition");
        for (int p = 0; p < 2; ++p) {
            const double a = rands[p * 2] / 65536.0 - 0.5;
            const double b = rands[p * 2 + 1] / 65536.0 - 0.5;
            const double c = std::cos(yaw * 3.14159265358979 / 32768.0);
            const double s = std::sin(yaw * 3.14159265358979 / 32768.0);
            expect(near(particles[p].position[0], c * a * 54.0 + origin[0]), "x");
            expect(near(particles[p].position[1], s * a * 54.0 + origin[1]), "y");
            expect(near(particles[p].position[2], b * 130.0 + 65.0 + origin[2]), "z");
            expect(particles[p].velocity[0] == 0.0f && particles[p].velocity[1] == 0.0f &&
                       particles[p].velocity[2] == 0.0f,
                   "zero velocity");
            expect(particles[p].extra == 0.0f, "float argument is zero");
        }
    }
    for (int angle : yaws) expect(angle == -9000, "yaw is the sign-extended halfword");
    return failures ? 1 : 0;
}
