// Host contract for msg_draw__10FireBeetle. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int frameNm;
static std::string order;
static std::vector<int> cos_args, sin_args;
struct Glow { void* object; void* model; void* animation; float point[3]; short heading; void* glow; float f[3]; short a[4], b[4]; int selector; float fourth; int last; };
static std::vector<Glow> glows;
static short ambient_seen[4];
static float pos_at_draw[3], pos_at_reset[3];
static void *draw_self, *draw_model, *draw_texture, *draw_extra;
static unsigned char* g_object;
static int failures = 0;
extern "C" float icos__Fi(int a) { cos_args.push_back(a); order += 'c'; return 0.25f + static_cast<float>(a % 97) * 0.01f; }
extern "C" float isin__Fi(int a) { sin_args.push_back(a); order += 's'; return -0.5f + static_cast<float>(a % 89) * 0.02f; }
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    order += 'D'; draw_self = self; draw_model = model; draw_texture = texture; draw_extra = extra;
    std::memcpy(pos_at_draw, g_object + 16, 12); return nullptr;
}
extern "C" void lightResetOptionalParams__Fv() { order += 'R'; std::memcpy(pos_at_reset, g_object + 16, 12); }
extern "C" void lightSetAmbientOffset__FPs(short* v) { order += 'A'; std::memcpy(ambient_seen, v, 8); }
extern "C" void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    void* object, void* model, void* animation, const void* point, short heading, void* glow, float first,
    float second, float third, short* a, short* b, int selector, float fourth, int last) {
    order += 'G';
    Glow g; g.object = object; g.model = model; g.animation = animation; std::memcpy(g.point, point, 12); g.heading = heading;
    g.glow = glow; g.f[0] = first; g.f[1] = second; g.f[2] = third; std::memcpy(g.a, a, 8); std::memcpy(g.b, b, 8);
    g.selector = selector; g.fourth = fourth; g.last = last; glows.push_back(g);
}
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    using namespace vslots_b;
    static unsigned char object[1200];
    static unsigned char group[64];
    static unsigned char model[4], texture[4];
    host_note(group); host_note(model); host_note(texture);
    g_object = object;
    write_word(address(group) + 12u, address_bits(model));
    write_word(address(group) + 20u, address_bits(texture));
    for (int state : {13, 14, 0, 12, 15, -1}) for (int t : {0, 1, -3, 12345}) for (int elapsed : {0, 15, 16, 40, 41, 1000, -5}) {
        std::memset(object, 0x41, sizeof object);
        write_word(address(object) + 36u, address_bits(group));
        float pos[3] = {10.5f, -20.25f, 300.0f};
        std::memcpy(object + 16, pos, 12);
        const float amb[4] = {10.7f, -3.9f, 300.5f, 0.0f};
        std::memcpy(object + 108, amb, 16);
        write_u16(address(object) + 572u, static_cast<unsigned>(state) & 0xffffu);
        write_word(address(object) + 808u, static_cast<unsigned>(t));
        write_u16(address(object) + 56u, 0xfff0u);
        frameNm = 1000;
        write_word(address(object) + 248u, static_cast<unsigned>(1000 - elapsed));
        order.clear(); cos_args.clear(); sin_args.clear(); glows.clear();
        msg_draw__10FireBeetle(object);
        std::string want;
        if (state == 13) want += "csc";
        if (state == 14) want += "GA";
        want += "DR";
        expect(order == want, "call order");
        expect(draw_self == object && draw_model == model && draw_texture == texture && draw_extra == nullptr, "creature draw arguments");
        float after[3]; std::memcpy(after, object + 16, 12);
        expect(std::memcmp(after, pos, 12) == 0, "position restored");
        float want_pos[3] = {pos[0], pos[1], pos[2]};
        if (state == 13) {
            expect(cos_args.size() == 2 && sin_args.size() == 1 && cos_args[0] == t * 1500 && cos_args[1] == t * 1500 && sin_args[0] == t * 1500, "clock angle");
            want_pos[0] += (0.25f + float(cos_args[0] % 97) * 0.01f) * 3.0f;
            want_pos[1] += (-0.5f + float(sin_args[0] % 89) * 0.02f) * 3.0f;
            want_pos[2] += (0.25f + float(cos_args[1] % 97) * 0.01f) * 5.0f;
        }
        expect(std::memcmp(pos_at_draw, want_pos, 12) == 0 && std::memcmp(pos_at_reset, want_pos, 12) == 0, "moved position at draw");
        if (state == 14) {
            expect(glows.size() == 1, "one glow");
            const Glow& g = glows[0];
            expect(g.object == object && g.model == model && g.animation == object + 288 && g.glow == object + 824 && g.heading == -16, "glow pointers");
            expect(std::memcmp(g.point, pos, 12) == 0 && g.f[0] == 0.0f && g.f[1] == 1.0f && g.f[2] == 0.0f && g.last == -2000, "glow constants");
            expect(g.a[0] == 250 && g.a[1] == 221 && g.a[2] == 61 && g.a[3] == 96 && g.b[0] == 150 && g.b[1] == 132 && g.b[2] == 37 && g.b[3] == 23, "tables");
            const int sel = elapsed < 16 ? 0 : elapsed < 41 ? 1 : 2;
            expect(g.selector == sel && g.fourth == (sel == 1 ? 1.0f : 0.25f), "selector and fourth float");
            expect(ambient_seen[0] == 10 && ambient_seen[1] == -3 && ambient_seen[2] == 300 && ambient_seen[3] == 0, "ambient truncation");
        }
        for (unsigned i = 0; i < sizeof object; ++i) {
            const bool known = (i >= 16 && i < 28);
            if (!known && object[i] != (i >= 108 && i < 124 ? object[i] : 0x41) && !(i >= 248 && i < 252) && !(i >= 808 && i < 812) && !(i >= 572 && i < 574) && !(i >= 56 && i < 58) && !(i >= 36 && i < 40)) { expect(false, "stray write"); break; }
        }
    }
    return failures ? 1 : 0;
}
