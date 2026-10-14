#include <cstdio>
#include <cstring>
#include <vector>

#include "run.cpp"

unsigned char candleParticle[80];

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static unsigned rng_state;
static std::vector<int> draws;
static int next_draw() {
    rng_state = rng_state * 1103515245u + 12345u;
    int value = static_cast<int>((rng_state >> 4) & 0xffff);
    draws.push_back(value);
    return value;
}
static int particle_calls, remove_calls;
static Point3 got_position, got_velocity;
static void* got_def;
static float got_age;
static void* remove_self;
extern "C" int eRand__10GameObject(void* object) {
    (void)object;
    return next_draw();
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age) {
    ++particle_calls;
    got_def = definition;
    got_position = *position;
    got_velocity = *velocity;
    got_age = age;
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) {
    ++remove_calls;
    remove_self = object;
}

// Independent model of the observed arithmetic, fed from the recorded draws.
struct Model {
    const std::vector<int>& d;
    unsigned at;
    float unit() {
        float t = static_cast<float>(d[at++]) * (1.0f / 65536.0f);
        return t + t - 1.0f;
    }
    Point3 ball() {
        Point3 p;
        do {
            p.x = unit();
            p.y = unit();
            p.z = unit();
        } while (p.x * p.x + p.y * p.y + p.z * p.z > 1.0f);
        return p;
    }
};

int main() {
    alignas(8) unsigned char object[256];
    for (unsigned seed = 1; seed <= 40; ++seed) {
        for (int flag : {0, 1, 30, 31, 100, 127, 128, 255}) {
            std::memset(object, 0x22, sizeof object);
            const float x = 3.5f * static_cast<float>(seed), y = -1.25f, z = 40.0f;
            std::memcpy(object + 16, &x, 4);
            std::memcpy(object + 20, &y, 4);
            std::memcpy(object + 24, &z, 4);
            object[140] = 1;
            object[141] = static_cast<unsigned char>(flag);
            const int counter = static_cast<int>(seed % 6u), mode = static_cast<int>(seed % 3u);
            std::memcpy(object + 136, &counter, 4);
            std::memcpy(object + 144, &mode, 4);
            rng_state = seed * 2654435761u;
            draws.clear();
            particle_calls = remove_calls = 0;
            msg_run__6Candle(object);

            Model m{draws, 0};
            Point3 vel = m.ball();
            vel.z = vel.z * 0.3f + 3.5f;
            Point3 pos = m.ball();
            if (pos.z < 0.0f) pos.z = -pos.z;
            pos.z = pos.z + z + (mode == 1 ? 8.0f : -4.0f);
            expect(particle_calls == 1 && got_def == candleParticle && got_age == 0.0f, "one particle with the definition and age 0");
            expect(got_velocity.x == 0.0f && got_velocity.y == 0.0f, "velocity x and y are zero");
            float dv = got_velocity.z - vel.z;
            expect((dv < 0 ? -dv : dv) <= 1e-6f * (vel.z < 0 ? -vel.z : vel.z) + 1e-7f, "velocity z");
            expect(got_position.x == x && got_position.y == y, "position x and y come from the object");
            float dp = got_position.z - pos.z;
            expect((dp < 0 ? -dp : dp) <= 1e-6f * (pos.z < 0 ? -pos.z : pos.z) + 1e-6f, "position z");
            {
                const int expected_counter = counter + 1;
                int got_counter;
                std::memcpy(&got_counter, object + 136, 4);
                if (expected_counter == 5) {
                    expect(got_counter == 0 && draws.size() == m.at + 1u, "counter reset after one more draw");
                    const int n = ((draws[m.at] - 8) & 15) + (mode == 1 ? 84 : 48);
                    float f = static_cast<float>(n), inv = 1.0f / (f * f), g104, g108, g112, g116;
                    std::memcpy(&g104, object + 104, 4);
                    std::memcpy(&g108, object + 108, 4);
                    std::memcpy(&g112, object + 112, 4);
                    std::memcpy(&g116, object + 116, 4);
                    expect(g104 == f && g112 == f && g108 == inv && g116 == inv, "scale floats");
                } else {
                    expect(got_counter == expected_counter && draws.size() == m.at, "counter incremented, no extra draw");
                }
            }
            const signed char sflag = static_cast<signed char>(flag);
            if (sflag >= 31) {
                expect(remove_calls == 1 && remove_self == object && object[140] == 0, "removed from the run list, byte cleared");
            } else {
                expect(remove_calls == 0 && object[140] == 1, "stays on the run list");
            }
            expect(object[141] == static_cast<unsigned char>(flag + 1), "counter byte incremented");
        }
    }
    return failures ? 1 : 0;
}
