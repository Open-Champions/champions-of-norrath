#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
#include "msg_draw.cpp"
using namespace vslots_b;
static int failures = 0;
// events: 1 draw, 2 attach, 3 ident, 4 trans, 5 rot, 6 modelDraw
static std::vector<int> events;
struct Attach { void* model; unsigned char point[12]; short yaw; void* state; int index; void* first; void* second; float scale; };
static std::vector<Attach> attaches;
static std::vector<float> trans;  // x y z triples
static std::vector<std::pair<int, short>> rots;
struct Draw { void* model; void* texture; int mode; unsigned char point[12]; void* matrix; void* anim; unsigned flags; void* record; int last; };
static std::vector<Draw> draws;
static std::vector<void*> ident_matrices, trans_matrices, rot_matrices;
static void *draw_model, *draw_texture, *draw_extra;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    (void)self; events.push_back(1); draw_model = model; draw_texture = texture; draw_extra = extra;
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short yaw, void* state, int index, void* first, void* second, float scale) {
    events.push_back(2);
    Attach a; a.model = model; std::memcpy(a.point, point, 12); a.yaw = yaw; a.state = state; a.index = index; a.first = first; a.second = second; a.scale = scale;
    attaches.push_back(a);
    const float values[3] = {10.0f * (index + 1), 20.0f * (index + 1), 30.0f * (index + 1)};
    std::memcpy(first, values, 12);
    const float junk[3] = {-1.0f, -2.0f, -3.0f};
    std::memcpy(second, junk, 12);
}
extern "C" void matIdent__FP8Matrix34(void* matrix) { events.push_back(3); ident_matrices.push_back(matrix); }
extern "C" void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z) {
    events.push_back(4); trans_matrices.push_back(matrix); trans.push_back(x); trans.push_back(y); trans.push_back(z);
}
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle) { events.push_back(5); rot_matrices.push_back(matrix); rots.push_back({axis, angle}); }
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last) {
    events.push_back(6);
    Draw d; d.model = model; d.texture = texture; d.mode = mode; std::memcpy(d.point, point, 12); d.matrix = matrix; d.anim = animation; d.flags = flags; d.record = draw_record; d.last = last;
    draws.push_back(d);
    return 0;
}
static unsigned char object[900], group[128], models[6][4], textures[6][4], main_model[4], main_texture[4];
static void reset() { events.clear(); attaches.clear(); trans.clear(); rots.clear(); draws.clear(); ident_matrices.clear(); trans_matrices.clear(); rot_matrices.clear(); }
static void run(unsigned half) {
    reset();
    write_u16(address(object) + 808u, half);
    msg_draw__7Natasla(object);
}
int main() {
    host_note(group); host_note(main_model); host_note(main_texture);
    for (int i = 0; i < 6; ++i) { host_note(models[i]); host_note(textures[i]); }
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x50 + i);
    write_u16(address(object) + 56u, 0xff00u);
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(group) + 12u, address_bits(main_model));
    write_word(address(group) + 20u, address_bits(main_texture));
    for (int i = 0; i < 5; ++i) {
        write_word(address(group) + 36u + 16u * i, address_bits(models[i]));
        write_word(address(group) + 44u + 16u * i, address_bits(textures[i]));
    }
    run(0x0123u);
    std::vector<int> want = {1};
    for (int i = 0; i < 5; ++i) { want.push_back(2); want.push_back(3); want.push_back(4); want.push_back(5); want.push_back(6); }
    expect(events == want, "creature draw, then five (attach, ident, trans, rot, draw) groups");
    expect(draw_model == main_model && draw_texture == main_texture && draw_extra == nullptr, "creature draw arguments");
    expect(attaches.size() == 5 && draws.size() == 5 && rots.size() == 5 && trans.size() == 15, "five of each");
    if (attaches.size() == 5 && draws.size() == 5 && rots.size() == 5 && trans.size() == 15) {
        for (int i = 0; i < 5; ++i) {
            expect(attaches[i].model == main_model && attaches[i].index == i && attaches[i].scale == 1.0f, "attachment model, index and scale");
            expect(std::memcmp(attaches[i].point, object + 16, 12) == 0 && attaches[i].yaw == static_cast<short>(0xff00) && attaches[i].state == object + 288, "attachment point, signed yaw, state");
            expect(attaches[i].first != attaches[i].second, "two result buffers");
            expect(trans[3 * i] == 10.0f * (i + 1) && trans[3 * i + 1] == 20.0f * (i + 1) && trans[3 * i + 2] == 30.0f * (i + 1), "matTrans uses the first result");
            expect(ident_matrices[i] == trans_matrices[i] && trans_matrices[i] == rot_matrices[i] && rot_matrices[i] == draws[i].matrix, "one matrix per part");
            expect(draws[i].model == models[i] && draws[i].texture == textures[i] && draws[i].mode == 1, "model/texture 36+16i, 44+16i");
            float p[3];
            std::memcpy(p, draws[i].point, 12);
            expect(p[0] == 10.0f * (i + 1) && p[1] == 20.0f * (i + 1) && p[2] == 30.0f * (i + 1), "draw point is the first result");
            expect(draws[i].anim == nullptr && draws[i].flags == 0xffffffffu && draws[i].record == nullptr && draws[i].last == 0, "draw tail arguments");
        }
        expect(rots[0].first == 2 && rots[1].first == 1 && rots[2].first == 0 && rots[3].first == 2 && rots[4].first == 2, "axes");
        expect(rots[0].second == static_cast<short>(0x0123u << 7) && rots[1].second == static_cast<short>(0x0123u << 12) &&
               rots[2].second == static_cast<short>(0x0123u << 8) && rots[3].second == static_cast<short>(0x0123u << 10) &&
               rots[4].second == static_cast<short>((0u - 0x0123u) << 9), "angles for 0x0123");
    }
    // High bits and sign of the narrowing.
    run(0xfedcu);
    expect(rots.size() == 5 && rots[0].second == static_cast<short>(0xfedcu << 7) && rots[1].second == static_cast<short>(0xfedcu << 12) &&
           rots[2].second == static_cast<short>(0xfedcu << 8) && rots[3].second == static_cast<short>(0xfedcu << 10) &&
           rots[4].second == static_cast<short>((0u - 0xfedcu) << 9), "angles for 0xfedc");
    run(0x0001u);
    expect(rots.size() == 5 && rots[0].second == 128 && rots[1].second == 4096 && rots[2].second == 256 && rots[3].second == 1024 && rots[4].second == static_cast<short>(0xfffffe00u), "angles for 1");
    run(0u);
    expect(rots.size() == 5 && rots[0].second == 0 && rots[4].second == 0, "angles for 0");
    run(0x4000u);
    expect(rots.size() == 5 && rots[0].second == 0 && rots[1].second == 0 && rots[2].second == 0 && rots[4].second == 0, "wrapped-out shifts");
    run(0x0100u);
    expect(rots.size() == 5 && rots[0].second == static_cast<short>(0x8000) && rots[2].second == 0, "bit 8 shifted by 7 is the sign bit");
    // Halfword read: a word with garbage above bit 15 must not leak in.
    write_word(address(object) + 808u, 0xabcd0003u);
    reset();
    msg_draw__7Natasla(object);
    expect(rots.size() == 5 && rots[0].second == 3 * 128 && rots[4].second == static_cast<short>((0u - 3u) << 9), "only the low halfword is used");
    // The group is re-read for every part.
    return failures == 0 ? 0 : 1;
}
