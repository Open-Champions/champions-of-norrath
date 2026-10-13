// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

static std::string calls;
static unsigned char* current;
static unsigned half_at_draw;
static void *seen_model, *seen_texture, *seen_shadow, *seen_self;
extern "C" {
void modelSetShadowScale__Ff(float scale) { calls += scale == 2.0f ? "scale2;" : "scale?;"; }
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* shadow) {
    calls += "draw;";
    seen_self = self; seen_model = model; seen_texture = texture; seen_shadow = shadow;
    half_at_draw = current[56] | (current[57] << 8);
}
void modelResetOptionalParams__Fv() { calls += "reset;"; }
}
#include "msg_draw.cpp"

static unsigned half(const unsigned char* o, unsigned off) { return o[off] | (o[off + 1] << 8); }
int main() {
    int failures = 0;
    alignas(8) unsigned char object[1024];
    alignas(8) unsigned char group[64];
    int model = 0, texture = 0;
    struct Case { unsigned angle, offset; };
    const Case cases[] = {{0, 0}, {100, 30}, {10, 30}, {0, 1}, {0xffff, 0xffff}, {0x8000, 0x7fff}, {5, 0x8000}};
    for (const Case& c : cases) {
        std::memset(object, 0x6b, sizeof object);
        std::memset(group, 0x77, sizeof group);
        const unsigned long long m = reinterpret_cast<unsigned long long>(&model);
        const unsigned long long t = reinterpret_cast<unsigned long long>(&texture);
        const unsigned mbits = static_cast<unsigned>(m), tbits = static_cast<unsigned>(t);
        rtti_c::host_note(&model);
        rtti_c::host_note(&texture);
        rtti_c::host_note(group);
        std::memcpy(group + 12, &mbits, 4);
        std::memcpy(group + 20, &tbits, 4);
        const unsigned gbits = static_cast<unsigned>(reinterpret_cast<unsigned long long>(group));
        std::memcpy(object + 36, &gbits, 4);
        object[56] = c.angle & 0xff; object[57] = c.angle >> 8;
        object[988] = c.offset & 0xff; object[989] = c.offset >> 8;
        current = object;
        calls.clear();
        msg_draw__8AntQueen(object);
        if (calls != "scale2;draw;reset;") { std::fprintf(stderr, "fail: order %s\n", calls.c_str()); ++failures; }
        if (half_at_draw != ((c.angle - c.offset) & 0xffffu)) { std::fprintf(stderr, "fail: angle during draw\n"); ++failures; }
        if (half(object, 56) != c.angle) { std::fprintf(stderr, "fail: angle restored\n"); ++failures; }
        if (seen_self != object || seen_model != &model || seen_texture != &texture || seen_shadow != nullptr) {
            std::fprintf(stderr, "fail: draw arguments\n"); ++failures;
        }
        if (half(object, 988) != c.offset || object[55] != 0x6b || object[58] != 0x6b || object[990] != 0x6b) {
            std::fprintf(stderr, "fail: neighbours\n"); ++failures;
        }
    }
    return failures ? 1 : 0;
}
