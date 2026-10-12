#define NORRATH_HOST_ARENA
#include "monster_spawn.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
// ---- host stand-ins for the imports ----
int eRandState = 0;
extern "C" {
int norrath_factory_strncmp(const char* l, const char* r, unsigned n) { return std::strncmp(l, r, n); }
int norrath_factory_atoi(const char* t) { return std::atoi(t); }
char* norrath_factory_strstr(const char* t, const char* n) { return const_cast<char*>(std::strstr(t, n)); }
int norrath_factory_sscanf(const char* t, const char* f, ...) {
    // Only the "%d" form is used.
    if (std::strcmp(f, "%d") != 0) std::abort();
    int* out; __builtin_va_list ap; __builtin_va_start(ap, f); out = __builtin_va_arg(ap, int*); __builtin_va_end(ap);
    return std::sscanf(t, "%d", out);
}
unsigned long long norrath_player_atof(const char* t) {
    double d = std::strtod(t, nullptr); unsigned long long bits; std::memcpy(&bits, &d, 8); return bits;
}
float norrath_player_double_to_float(unsigned long long bits) {
    double d; std::memcpy(&d, &bits, 8); return static_cast<float>(d);
}
struct Call { unsigned char point[12]; int radius, line; short type; std::string found; bool has_found; bool filter; char** options; int clear; };
static std::vector<Call> calls;
static const char* g_base;   // option strings searched for a pointer's owner
void spawn__FG6Point3iisPcbPPci(const unsigned char* point, int radius, int line, short type_word, char* found,
                                bool filter, char** options, int clear_radius) {
    Call c; std::memcpy(c.point, point, 12); c.radius = radius; c.line = line; c.type = type_word;
    c.has_found = found != nullptr; c.found = found ? found : ""; c.filter = filter; c.options = options; c.clear = clear_radius;
    calls.push_back(c);
    (void)g_base;
}
}
// ---- an independent description of the random stream ----
static unsigned lcg(unsigned s) { return s * 0x19660du + 0x3c6ef35fu; }
static float frac(unsigned s) { return float(s >> 16) / 65536.0f; }
static int fail(const char* what, int a = 0, int b = 0) { std::printf("FAIL %s %d %d\n", what, a, b); return 1; }
int main() {
    unsigned char point[12]; for (int i = 0; i != 12; ++i) point[i] = 0x30 + i;
    // 1. Without chance= the routine never spawns, but consumes exactly one draw.
    {
        char type[] = "type=1,wolf"; char radius[] = "radius=77"; char* options[] = {type, radius, nullptr};
        calls.clear(); eRandState = 1234;
        norrath_monster_spawn(options, point, -5, true);
        if (!calls.empty()) return fail("no chance spawned");
        if (unsigned(eRandState) != lcg(1234)) return fail("state after gate");
    }
    // 2. chance gate: spawn exactly when chance > draw; the state advances once on a refusal
    //    and twice on an acceptance (plus the selection draw).
    for (unsigned seed = 1; seed < 400; seed += 7) {
        char type[] = "type=1,wolf"; char chance[] = "chance=0.5"; char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char* options[] = {type, chance, lo, hi, nullptr};
        calls.clear(); eRandState = int(seed);
        norrath_monster_spawn(options, point, 3, false);
        const float d1 = frac(lcg(seed));
        if (!(0.5f <= d1)) {
            // accepted: draw 2 (count), draw 3 (entry choice)
            if (calls.size() != 1) return fail("accepted count", int(seed), int(calls.size()));
            if (unsigned(eRandState) != lcg(lcg(lcg(seed)))) return fail("state accepted", int(seed));
        } else {
            if (!calls.empty()) return fail("refused spawned", int(seed));
            if (unsigned(eRandState) != lcg(seed)) return fail("state refused", int(seed));
        }
    }
    // 3. Every option is forwarded; the point is copied, the type word is sign-extended.
    {
        char type[] = "type=1,wolf"; char chance[] = "chance=2"; char line[] = "line=5"; char radius[] = "radius=77";
        char clear[] = "clearradius=9"; char lo[] = "countmin=3"; char hi[] = "countmax=3";
        char* options[] = {type, chance, line, radius, clear, lo, hi, nullptr};
        calls.clear(); eRandState = 99;
        norrath_monster_spawn(options, point, -5, true);
        // count = (int)(3 + draw * 1) = 3
        if (calls.size() != 3) return fail("count 3", int(calls.size()));
        for (const Call& c : calls) {
            if (std::memcmp(c.point, point, 12) || c.radius != 77 || c.line != 5 || c.type != -5 || !c.filter ||
                c.options != options || c.clear != 9) return fail("forwarded arguments");
            if (!c.has_found || c.found != "wolf") return fail("found text");
        }
    }
    // 4. Defaults: radius 2000, line 0, clearradius -1; later options override earlier ones.
    {
        char type[] = "type=1,wolf"; char chance[] = "chance=2"; char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char r1[] = "radius=5"; char r2[] = "radius=6";
        char* options[] = {type, chance, lo, hi, r1, r2, nullptr};
        calls.clear(); eRandState = 5;
        norrath_monster_spawn(options, point, 1, false);
        if (calls.size() != 1 || calls[0].radius != 6 || calls[0].line != 0 || calls[0].clear != -1 || calls[0].filter)
            return fail("defaults");
        char* options2[] = {type, chance, lo, hi, nullptr};
        calls.clear(); norrath_monster_spawn(options2, point, 1, false);
        if (calls.size() != 1 || calls[0].radius != 2000) return fail("default radius");
    }
    // 5. Count range: count = (int)(min + draw * (max - min + 1)).
    for (unsigned seed = 3; seed < 300; seed += 11) {
        char type[] = "type=1,wolf"; char chance[] = "chance=2"; char lo[] = "countmin=2"; char hi[] = "countmax=6";
        char* options[] = {type, chance, lo, hi, nullptr};
        calls.clear(); eRandState = int(seed);
        norrath_monster_spawn(options, point, 1, false);
        const unsigned s2 = lcg(lcg(seed));
        const int want = int(2.0f + frac(s2) * 5.0f);
        if (int(calls.size()) != want) return fail("count range", int(seed), want);
    }
    // 6. Entry parsing and always/once flags. Type text offsets: wolf 2, bat 7, rat group 13, spider 23.
    {
        char type[] = "type=4,wolf,bat-A,(rat-O;x),spider-AO"; char chance[] = "chance=2";
        char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char* options[] = {type, chance, lo, hi, nullptr};
        const char* text = type + 5;
        for (unsigned seed = 11; seed < 600; seed += 13) {
            calls.clear(); eRandState = int(seed);
            norrath_monster_spawn(options, point, 1, false);
            // After the two always-entries, spider (once) is unavailable and n is 3.
            const unsigned s3 = lcg(lcg(lcg(seed)));
            const int pick = int(3.0f * frac(s3));
            const int starts[] = {2, 7, 13, 23};
            if (calls.size() != 3) return fail("parse call count", int(seed), int(calls.size()));
            if (calls[0].found != std::string(text + 7) || calls[1].found != std::string(text + 23)) return fail("always found", int(seed));
            if (calls[2].found != std::string(text + starts[pick])) return fail("selection", int(seed), pick);
        }
    }
    // 7. multitype: each iteration draws its own entry; a once-entry leaves the pool.
    {
        char type[] = "type=3,a,b-O,c"; char chance[] = "chance=2"; char multi[] = "multitype=1";
        char lo[] = "countmin=4"; char hi[] = "countmax=4";
        char* options[] = {type, chance, multi, lo, hi, nullptr};
        const char* text = type + 5;
        const int starts[] = {2, 4, 8};
        for (unsigned seed = 21; seed < 500; seed += 17) {
            calls.clear(); eRandState = int(seed);
            norrath_monster_spawn(options, point, 1, false);
            if (calls.size() != 4) return fail("multi count", int(seed), int(calls.size()));
            // Independent model of the pool.
            unsigned s = lcg(lcg(seed));          // after the gate and the count draw
            bool available[3] = {true, true, true};
            int n = 3;
            for (int k = 0; k != 4; ++k) {
                s = lcg(s);
                int idx = int(float(n) * frac(s));
                int rounds = 0;
                do { bool a = idx < 3 ? available[idx] : true; ++rounds; if (!a) idx += 1; } while (!(idx < rounds));
                if (idx > 2) return fail("model ran off", int(seed));
                if (idx == 1) { available[1] = false; n -= 1; }
                if (calls[k].found != std::string(text + starts[idx])) return fail("multi entry", int(seed), k);
            }
            if (unsigned(eRandState) != s) return fail("multi state", int(seed));
        }
    }
    // 8. Lower-case flags, a nested group, and removal of a once-and-always entry from the pool.
    //    Offsets in "4,w-a,x-o,(y-ao;p(q)),z": w 2, x 6, group 10, z 22.
    {
        char type[] = "type=4,w-a,x-o,(y-ao;p(q)),z"; char chance[] = "chance=2";
        char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char* options[] = {type, chance, lo, hi, nullptr};
        const char* text = type + 5;
        const int starts[] = {2, 6, 10, 22};
        for (unsigned seed = 5; seed < 700; seed += 9) {
            calls.clear(); eRandState = int(seed);
            norrath_monster_spawn(options, point, 1, false);
            if (calls.size() != 3) return fail("nested call count", int(seed), int(calls.size()));
            if (calls[0].found != std::string(text + 2) || calls[1].found != std::string(text + 10)) return fail("nested always", int(seed));
            int idx = int(3.0f * frac(lcg(lcg(lcg(seed)))));
            const bool available[4] = {true, true, false, true};
            int rounds = 0;
            do { bool a = idx < 4 ? available[idx] : true; ++rounds; if (!a) idx += 1; } while (!(idx < rounds));
            if (calls[2].found != std::string(text + starts[idx])) return fail("nested pick", int(seed), idx);
        }
    }
    // 9. The gate compares chance <= draw: an exactly equal draw refuses.
    {
        unsigned found_seed = 0; bool found = false;
        for (unsigned seed = 0; seed < (1u << 22) && !found; ++seed)
            if ((lcg(seed) >> 16) == 0x8000u) { found_seed = seed; found = true; }
        if (!found) return fail("no equal-draw seed");
        char type[] = "type=1,wolf"; char chance[] = "chance=0.5"; char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char* options[] = {type, chance, lo, hi, nullptr};
        calls.clear(); eRandState = int(found_seed);
        norrath_monster_spawn(options, point, 1, false);
        if (!calls.empty()) return fail("equal draw spawned");
    }
    // 10. "multitype" without the equals sign is not the multitype option.
    {
        char type[] = "type=3,a,b-O,c"; char chance[] = "chance=2"; char multi[] = "multitype";
        char lo[] = "countmin=4"; char hi[] = "countmax=4";
        char* options[] = {type, chance, multi, lo, hi, nullptr};
        calls.clear(); eRandState = 21;
        norrath_monster_spawn(options, point, 1, false);
        // Single-choice mode: all four calls name the same entry.
        if (calls.size() != 4) return fail("single count");
        for (int k = 1; k != 4; ++k) if (calls[k].found != calls[0].found) return fail("single choice differs");
    }
    // 11. Groups without flags: "(a)" closes on its own and "(c;d)" skips through ';'.
    //     Offsets in "3,(a),b,(c;d)": group 2, b 6, group 8.
    {
        char type[] = "type=3,(a),b,(c;d)"; char chance[] = "chance=2";
        char lo[] = "countmin=1"; char hi[] = "countmax=1";
        char* options[] = {type, chance, lo, hi, nullptr};
        const char* text = type + 5;
        const int starts[] = {2, 6, 8};
        for (unsigned seed = 2; seed < 400; seed += 5) {
            calls.clear(); eRandState = int(seed);
            norrath_monster_spawn(options, point, 1, false);
            if (calls.size() != 1) return fail("group call count", int(seed), int(calls.size()));
            const int idx = int(3.0f * frac(lcg(lcg(lcg(seed)))));
            if (calls[0].found != std::string(text + starts[idx])) return fail("group pick", int(seed), idx);
        }
    }
    return 0;
}
