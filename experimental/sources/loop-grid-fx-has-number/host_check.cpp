// Host contract for norrath_has_light_or_particle_number. Not an EE plan input.
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {0};
extern "C" int norrath_has_light_or_particle_number(char** arguments, int number);

static int failures = 0;
static int sprintf_calls = 0, atoi_calls = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" unsigned norrath_factory_strlen(const char* text) { return static_cast<unsigned>(std::strlen(text)); }
extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) { return std::strncmp(left, right, count); }
extern "C" int norrath_factory_atoi(const char* text) { ++atoi_calls; return std::atoi(text); }
extern "C" int norrath_factory_sprintf(char* destination, const char* format, ...) {
    ++sprintf_calls;
    va_list list;
    va_start(list, format);
    const int n = std::vsnprintf(destination, 32, format, list);
    va_end(list);
    return n;
}

int main() {
    for (int ch = 'A'; ch <= 'Z'; ++ch) norrath_factory_ctype_minimum_view[ch + 128] = 1;
    char other[] = "other=4", zero[] = "particleemitter2=0", seven[] = "particleemitter2=7";
    char light[] = "dynamiclight2=3", halo[] = "halo2=5", no_equals[] = "halo2", prefix[] = "halo22=8";
    char halo_zero[] = "halo2=0";

    // First matching entry wins, even when its value is zero; the later 7 is not read.
    char* first_zero[] = {other, zero, seven, nullptr};
    expect(norrath_has_light_or_particle_number(first_zero, 2) == 0, "a zero first match hides later matches");
    char* first_seven[] = {other, seven, zero, nullptr};
    expect(norrath_has_light_or_particle_number(first_seven, 2) == 1, "nonzero particle tag");
    // Other numbers do not match "ParticleEmitter2" (strncmp over the formatted length only: 3 would not).
    expect(norrath_has_light_or_particle_number(first_seven, 3) == 0, "a different number does not match");
    // The light tag is checked second, the halo tag last.
    char* with_light[] = {other, light, nullptr};
    expect(norrath_has_light_or_particle_number(with_light, 2) == 1, "dynamic light tag");
    char* with_halo[] = {halo, nullptr};
    expect(norrath_has_light_or_particle_number(with_halo, 2) == 1, "halo tag");
    char* halo_off[] = {halo_zero, nullptr};
    expect(norrath_has_light_or_particle_number(halo_off, 2) == 0, "halo tag with zero value");
    // A name match without '=' right after it is skipped, so the next entry is considered.
    char* no_eq_then_value[] = {no_equals, halo, nullptr};
    expect(norrath_has_light_or_particle_number(no_eq_then_value, 2) == 1, "entry without equals is skipped");
    // "halo22=8" starts with "halo2" but has '2' where '=' must be.
    char* longer_name[] = {prefix, nullptr};
    expect(norrath_has_light_or_particle_number(longer_name, 2) == 0, "a longer name sharing the prefix is not a match");
    // Empty and null lists.
    char* empty[] = {nullptr};
    expect(norrath_has_light_or_particle_number(empty, 2) == 0, "empty list");
    sprintf_calls = 0;
    expect(norrath_has_light_or_particle_number(nullptr, 2) == 0, "null list");
    expect(sprintf_calls == 3, "the three names are formatted even for a null list");
    // atoi is only reached for the first match of each tag.
    atoi_calls = 0;
    char* dup[] = {seven, seven, nullptr};
    norrath_has_light_or_particle_number(dup, 2);
    expect(atoi_calls == 1, "returns after the first value");
    if (failures != 0) return 1;
    std::printf("has-number contract passed\n");
    return 0;
}
