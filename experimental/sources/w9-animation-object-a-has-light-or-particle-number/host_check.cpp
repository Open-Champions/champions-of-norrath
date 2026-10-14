// Host contract for hasLightOrParticleNumber__FPPci. Not an EE plan input.
// Build: c++ -std=c++17 host_check.cpp
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> events;
// Upper-case letters (ctype bit 0) at table[c + 128]: 193 zeros, 26 ones for 'A'..'Z', 165 zeros.
#define Z8 0, 0, 0, 0, 0, 0, 0, 0
#define Z64 Z8, Z8, Z8, Z8, Z8, Z8, Z8, Z8
#define O8 1, 1, 1, 1, 1, 1, 1, 1
extern "C" const unsigned char norrath_factory_ctype_minimum_view[384] = {
    Z64, Z64, Z64, 0, O8, O8, O8, 1, 1, Z64, Z64, Z8, Z8, Z8, Z8, 0, 0, 0, 0, 0};
extern "C" int host_sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int n = std::vsnprintf(d, 32, f, ap); va_end(ap);
    events.push_back(std::string("sprintf ") + d);
    return n;
}
extern "C" unsigned norrath_factory_strlen(const char* t) { return (unsigned)std::strlen(t); }
extern "C" int norrath_factory_strncmp(const char* a, const char* b, unsigned n) { return std::strncmp(a, b, n); }
extern "C" int norrath_factory_atoi(const char* t) { events.push_back(std::string("atoi ") + t); return std::atoi(t); }
#define sprintf host_sprintf
#include "has_light_or_particle_number.cpp"
#undef sprintf
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
static int run(std::vector<const char*> tags, int number, bool null_list = false) {
    std::vector<char*> storage;
    for (const char* t : tags) storage.push_back(strdup(t));
    storage.push_back(nullptr);
    events.clear();
    int r = hasLightOrParticleNumber__FPPci(null_list ? nullptr : storage.data(), number);
    for (size_t i = 0; i + 1 < storage.size(); ++i) free(storage[i]);
    return r;
}
int main() {
    // The names are built in order, lowercased in place, and tried until one is nonzero.
    check(run({"particleemitter3=1"}, 3) == 1, "emitter hit");
    check(events == std::vector<std::string>{"sprintf ParticleEmitter3", "atoi 1"}, "emitter order");
    check(run({"dynamiclight3=2"}, 3) == 1, "light hit");
    check(events == std::vector<std::string>{"sprintf ParticleEmitter3", "sprintf DynamicLight3", "atoi 2"}, "light order");
    check(run({"halo3=7"}, 3) == 1, "halo hit");
    check(events == std::vector<std::string>{"sprintf ParticleEmitter3", "sprintf DynamicLight3", "sprintf Halo3", "atoi 7"}, "halo order");
    // Zero values and missing tags give 0 after all three names were tried.
    check(run({"halo3=0", "particleemitter3=0", "dynamiclight3=0"}, 3) == 0, "all zero");
    check(events.size() == 3 + 3, "three sprintf and three atoi");
    check(run({"other=1", "halo4=1"}, 3) == 0, "other number");
    // '=' is required right after the name; a longer name with the same prefix is not a hit.
    check(run({"halo30=1"}, 3) == 0, "prefix is not a hit");
    check(run({"halo3:1"}, 3) == 0, "separator must be '='");
    // Only the first matching entry counts; its value decides (a later nonzero one is ignored).
    check(run({"halo3=0", "halo3=5"}, 3) == 0, "first match decides");
    check(run({"halo3=x"}, 3) == 0, "non-numeric is 0");
    check(run({"halo3=-4"}, 3) == 1, "negative is nonzero");
    check(run({"halo-1=1"}, -1) == 1, "negative number");
    // Null array and empty array give 0; the names are still built.
    check(run({}, 3, true) == 0 && events.size() == 3, "null array");
    check(run({}, 3) == 0 && events.size() == 3, "empty array");
    if (failures) return 1;
    std::printf("hasLightOrParticleNumber check passed\n");
    return 0;
}
