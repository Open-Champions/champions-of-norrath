// Host contract for msg_run__5Torch; not an EE input. Includes the source. eRand returns 32768 unless
// a test overrides it, which makes every ball point (0, 0, 0); noise returns a value derived from the
// seed so the seeds are visible in the stored words.
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <initializer_list>

int frameNm;
unsigned char torch_msg_run_particle_7d7ba0[8], torch_msg_run_particle_7d7c40[8], torch_msg_run_particle_7d7c90[8];
static std::vector<unsigned> rolls;
static size_t roll_index;
static std::vector<int> seeds;
static std::string events;
struct Particle { const void* def; Point3 p, v; float age; };
static std::vector<Particle> particles;
static int removes;
unsigned eRand__10GameObject(void*) { events += 'r'; return roll_index < rolls.size() ? rolls[roll_index++] : 32768u; }
float ef1Noise__Fi(int seed) { seeds.push_back(seed); events += 'n'; return 0.25f * static_cast<float>(seeds.size()); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    events += 'P'; particles.push_back({d, *p, *v, age});
}
void objectRemoveFromRunList__FP10GameObject(void*) { ++removes; events += 'X'; }

static float read_f(const unsigned char* o, int off) { float f; std::memcpy(&f, o + off, 4); return f; }
static int fail(int code, const char* what) { std::printf("fail %d: %s\n", code, what); return code; }

int main() {
    alignas(8) unsigned char object[160];
    const float pos[3] = {1.5f, 2.0f, 100.0f};
    for (int mode : {0, 1, 2, 3, 4, 5, 9}) {
        for (int frame : {0, 8, 3}) {
            std::memset(object, 0, sizeof object);
            std::memcpy(object + 16, pos, 12);
            object[138] = static_cast<unsigned char>(mode);
            object[137] = 5;
            frameNm = frame;
            rolls.clear(); roll_index = 0; seeds.clear(); events.clear(); particles.clear(); removes = 0;
            msg_run__5Torch(object);
            const int n = mode == 4 ? 4 : (mode == 2 || mode == 5) ? 3 : 1;
            const bool heat = (frame & 7) == 0 && mode != 4 && mode != 5;
            std::string want;
            for (int i = 0; i < n; ++i) {
                want += "rrrrrrP";  // two ball points (six draws), one torch particle
                if (heat) want += "rrrrP";  // one draw for the z boost, one ball point, one heat particle
            }
            want += "nnnn";
            if (mode != 0 && mode != 4) want += "r";  // the single spark roll is 32768, which is not below 794
            if (events != want) return fail(1, events.c_str());
            if (static_cast<int>(particles.size()) != n * (heat ? 2 : 1)) return fail(2, "particle count");
            // first torch particle: v = {0, 0, 4.8}; p depends on the mode
            const Particle& t = particles[0];
            if (t.def != torch_msg_run_particle_7d7ba0) return fail(3, "torch definition");
            if (t.v.x != 0.0f || t.v.y != 0.0f || t.v.z != __builtin_bit_cast(float, 0x40999999u)) return fail(4, "velocity");
            float add = (mode == 2 || mode == 5) ? 5.0f : (mode == 3 || mode == 4) ? 10.0f : (mode == 1 ? 0.0f : 0.0f);
            if (t.p.x != 1.5f || t.p.y != 2.0f || t.p.z != (add + 100.0f) - 12.0f) return fail(5, "position");
            if (t.age != (mode == 3 ? 0.25f : 0.0f)) return fail(6, "age");
            if (heat) {
                const Particle& h = particles[1];
                if (h.def != torch_msg_run_particle_7d7c40 || h.age != 0.0f) return fail(7, "heat definition");
                if (h.p.x != 1.5f || h.p.y != 2.0f || h.p.z != 100.0f) return fail(8, "heat position");
                if (h.v.z != __builtin_bit_cast(float, 0x40999999u) + 10.0f) return fail(9, "heat boost");
            }
            // part 2
            if (seeds.size() != 4) return fail(10, "noise count");
            const int K = static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object)) * 0x19660du);
            if (seeds[0] != 0x7efe41 + K + frame * 16 || seeds[1] != K + frame * 2 || seeds[2] != K + frame * 2 + 0x19660d ||
                seeds[3] != K + frame * 2 + 0x32cc1a) return fail(11, "seeds");
            const float g = static_cast<float>(static_cast<int>(0.25f * 22.0f) + 160);
            if (read_f(object, 104) != g || read_f(object, 112) != g || read_f(object, 108) != 1.0f / (g * g) || read_f(object, 116) != 1.0f / (g * g)) return fail(12, "light words");
            if (read_f(object, 124) != 1.5f + 0.5f * 8.0f || read_f(object, 128) != 2.0f + 0.75f * 8.0f || read_f(object, 132) != 100.0f + 1.0f * 8.0f) return fail(13, "noise offsets");
            if (removes != 0 || object[137] != 6) return fail(14, "counter");
        }
    }
    // spark roll below 794 for a nonzero mode other than 4: one more ball point and one more particle
    for (unsigned roll : {793u, 794u}) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 16, pos, 12);
        object[138] = 1; frameNm = 3;
        rolls.assign(6 + 1 + 0, 32768u); rolls[6] = 32768u;
        // draws: 6 for the torch part, then the spark roll, then three for its ball point
        rolls = {32768u, 32768u, 32768u, 32768u, 32768u, 32768u, roll, 32768u, 32768u, 32768u};
        roll_index = 0; seeds.clear(); events.clear(); particles.clear(); removes = 0;
        msg_run__5Torch(object);
        const bool spark = roll < 794u;
        if (particles.size() != (spark ? 2u : 1u)) return fail(20, "spark presence");
        if (spark) {
            const Particle& s = particles[1];
            if (s.def != torch_msg_run_particle_7d7c90 || s.age != 0.0f) return fail(21, "spark definition");
            if (s.p.x != 1.5f || s.v.x != 0.0f || s.v.z != __builtin_bit_cast(float, 0x414a6666u)) return fail(22, "spark velocity");
        }
    }
    // run-list removal at the counter limit
    for (int counter : {29, 30, 31, 32, 127, 128, 255}) {
        std::memset(object, 0, sizeof object);
        object[136] = 7; object[137] = static_cast<unsigned char>(counter); object[138] = 0;
        frameNm = 3; roll_index = 0; rolls.clear(); seeds.clear(); events.clear(); particles.clear(); removes = 0;
        msg_run__5Torch(object);
        const bool remove = static_cast<signed char>(counter) >= 31;
        if (removes != (remove ? 1 : 0)) return fail(30, "remove call");
        if (object[136] != (remove ? 0 : 7)) return fail(31, "flag byte");
        if (object[137] != static_cast<unsigned char>(counter + 1)) return fail(32, "counter increment");
    }
    // rejection: a ball point outside the unit sphere is redrawn
    std::memset(object, 0, sizeof object);
    object[138] = 0; frameNm = 3;
    rolls = {0u, 0u, 0u, 32768u, 32768u, 32768u};  // first draw (-1, -1, -1) is rejected: 3 > 1
    roll_index = 0; seeds.clear(); events.clear(); particles.clear(); removes = 0;
    msg_run__5Torch(object);
    if (events.substr(0, 6) != "rrrrrr") return fail(40, "redraw");
    return 0;
}
