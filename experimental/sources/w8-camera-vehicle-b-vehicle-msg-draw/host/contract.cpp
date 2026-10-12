// Host contract for msg_draw__7Vehicle. Not an EE plan input.
// Built with -DNORRATH_W8_HOST on the source file as well.
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
extern "C" void msg_draw__7Vehicle(void* vehicle);

extern "C" unsigned char modelClippingOn = 0;
static unsigned char vehicle[2400];
static unsigned char wheels[4][320];
static const unsigned char* tokens[8];

extern "C" const unsigned char* norrath_w8_host_resolve(unsigned bits) { return tokens[bits]; }

struct Draw {
    void* model; void* texture; int flag; Point3 point; float matrix[12]; void* animation;
    unsigned long long mask; void* record; int extra; int clipping; int order;
};
struct Rot { int axis; short angle; float matrix[12]; void* address; int order; };
struct Trans { void* out; void* matrix33; void* in; int order; };
static Draw draws[8]; static int draw_count = 0;
static Rot rots[8]; static int rot_count = 0;
static Trans trans[4]; static int trans_count = 0;
static int resets = 0, reset_after_draw[8], order = 0;
static float matrix_address_seen = 0; static void* matrix_ptrs[8];

extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int flag, Point3 point, void* matrix, void* animation, unsigned long long mask, void* record, int extra) {
    Draw& d = draws[draw_count < 8 ? draw_count : 7];
    d.model = model; d.texture = texture; d.flag = flag; d.point = point;
    std::memcpy(d.matrix, matrix, 48); d.animation = animation; d.mask = mask; d.record = record; d.extra = extra;
    d.clipping = modelClippingOn; d.order = ++order;
    matrix_ptrs[draw_count < 8 ? draw_count : 7] = matrix;
    ++draw_count;
}
extern "C" void modelResetOptionalParams__Fv() {
    reset_after_draw[resets < 8 ? resets : 7] = draw_count; ++resets; ++order;
}
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle) {
    Rot& r = rots[rot_count < 8 ? rot_count : 7];
    r.axis = axis; r.angle = angle; std::memcpy(r.matrix, matrix, 48); r.address = matrix; r.order = ++order;
    // distinguish the matrix state each later step sees
    static_cast<float*>(matrix)[11] += 1000.0f;
    ++rot_count;
}
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* m33, Point3* in) {
    Trans& t = trans[trans_count < 4 ? trans_count : 3];
    t.out = out; t.matrix33 = m33; t.in = in; t.order = ++order;
    out->x = in->x * 2.0f; out->y = in->y * 3.0f - 1.0f; out->z = in->z + 0.5f;
    ++trans_count;
}

static void putf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static void putw(unsigned char* base, int off, unsigned v) { std::memcpy(base + off, &v, 4); }
static void putl(unsigned char* base, int off, unsigned long long v) { std::memcpy(base + off, &v, 8); }
static float getf(const unsigned char* base, int off) { float v; std::memcpy(&v, base + off, 4); return v; }
static void reset() {
    std::memset(vehicle, 0, sizeof vehicle); std::memset(wheels, 0, sizeof wheels);
    draw_count = rot_count = trans_count = resets = order = 0; modelClippingOn = 0;
    std::memset(draws, 0, sizeof draws); std::memset(rots, 0, sizeof rots);
    for (int i = 0; i < 9; ++i) putf(vehicle, 1116 + i * 4, 0.5f + i);
    putf(vehicle, 16, 10.0f); putf(vehicle, 20, 20.0f); putf(vehicle, 24, 30.0f);
    putw(vehicle, 112, 0x1111u); putw(vehicle, 116, 0x2222u); putl(vehicle, 392, 0x0123456789abcdefull);
    for (int s = 0; s < 4; ++s) { putf(vehicle, 1932 + s * 12, 1.0f + s); putf(vehicle, 1936 + s * 12, 10.0f + s); putf(vehicle, 1940 + s * 12, 100.0f + s); }
    for (int w = 0; w < 4; ++w) {
        putw(wheels[w], 0, 0x3000u + w); putw(wheels[w], 4, 0x4000u + w); putl(wheels[w], 272, 0xaa00000000000000ull + w);
        putf(wheels[w], 280, 90.0f + w); putf(wheels[w], 284, 400.0f + w);
        tokens[w + 1] = wheels[w];
    }
}
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)

static short expect_angle(float v) {
    unsigned a = 0x3bb60b60u, b = 0x46fffe00u; float fa, fb; std::memcpy(&fa, &a, 4); std::memcpy(&fb, &b, 4);
    return static_cast<short>(static_cast<int>(v * fa * fb));
}

int main() {
    reset();
    putw(vehicle, 2088, 1); putw(vehicle, 2092, 0); putw(vehicle, 2096, 3); putw(vehicle, 2100, 0);  // wheels 0 and 2 only
    msg_draw__7Vehicle(vehicle);
    EXPECT(draw_count == 3 && resets == 3, "body plus two wheels");
    EXPECT(reset_after_draw[0] == 1 && reset_after_draw[1] == 2 && reset_after_draw[2] == 3, "reset after each draw");
    EXPECT(draws[0].clipping == 1 && modelClippingOn == 1, "clipping on before the body draw");
    // body
    EXPECT(draws[0].model == reinterpret_cast<void*>(0x1111) && draws[0].texture == reinterpret_cast<void*>(0x2222), "body model words");
    EXPECT(draws[0].flag == 1 && draws[0].animation == nullptr && draws[0].record == nullptr && draws[0].extra == 0, "body fixed args");
    EXPECT(draws[0].mask == 0x0123456789abcdefull, "body 64-bit word");
    EXPECT(draws[0].point.x == 10.0f && draws[0].point.y == 20.0f && draws[0].point.z == 30.0f, "body point");
    for (int i = 0; i < 9; ++i) EXPECT(draws[0].matrix[i] == 0.5f + i, "body matrix rows");
    EXPECT(draws[0].matrix[9] == 10.0f && draws[0].matrix[10] == 20.0f && draws[0].matrix[11] == 30.0f, "body matrix translation");
    // wheel 0 and wheel 2 rotations and transforms
    EXPECT(rot_count == 4 && trans_count == 2, "rotation and transform counts");
    EXPECT(rots[0].axis == 2 && rots[0].angle == expect_angle(90.0f), "wheel 0 first rot");
    EXPECT(rots[1].axis == 0 && rots[1].angle == expect_angle(400.0f), "wheel 0 second rot");
    EXPECT(rots[2].axis == 2 && rots[2].angle == expect_angle(92.0f), "wheel 2 first rot");
    EXPECT(rots[3].axis == 0 && rots[3].angle == expect_angle(402.0f), "wheel 2 second rot");
    EXPECT(expect_angle(400.0f) == 7279, "angle wraps to 16 bits");
    for (int r = 0; r < 4; ++r) for (int i = 0; i < 9; ++i) EXPECT(rots[r].matrix[i] == 0.5f + i, "rows recopied before rotation");
    EXPECT(rots[0].matrix[11] == 30.0f, "first wheel sees the body translation");
    EXPECT(rots[1].matrix[11] == 1030.0f, "matrix is shared between the two rotations");
    EXPECT(rots[2].matrix[11] != 30.0f, "translation persists to the next wheel");
    EXPECT(trans[0].matrix33 == vehicle + 1116 && trans[0].in == reinterpret_cast<void*>(vehicle + 1932), "wheel 0 transform inputs");
    EXPECT(trans[1].matrix33 == vehicle + 1116 && trans[1].in == reinterpret_cast<void*>(vehicle + 1932 + 24), "wheel 2 transform inputs");
    EXPECT(rots[0].order < rots[1].order && rots[1].order < trans[0].order && trans[0].order < draws[1].order, "per-wheel order");
    // wheel 0: rel = (1*2, 10*3-1, 100+0.5)
    EXPECT(draws[1].point.x == 10.0f + 2.0f && draws[1].point.y == 20.0f + 29.0f && draws[1].point.z == 30.0f + 100.5f, "wheel 0 position");
    EXPECT(draws[1].matrix[9] == 12.0f && draws[1].matrix[10] == 49.0f && draws[1].matrix[11] == 130.5f, "wheel 0 matrix translation");
    EXPECT(draws[1].model == reinterpret_cast<void*>(0x3000) && draws[1].texture == reinterpret_cast<void*>(0x4000), "wheel 0 words");
    EXPECT(draws[1].mask == 0xaa00000000000000ull && draws[1].flag == 1 && draws[1].animation == nullptr && draws[1].record == nullptr && draws[1].extra == 0, "wheel 0 args");
    for (int i = 0; i < 9; ++i) EXPECT(draws[1].matrix[i] == 0.5f + i, "wheel 0 rows");
    // wheel 2 (slot 2): in = (3, 12, 102); rel = (6, 35, 102.5)
    EXPECT(draws[2].point.x == 16.0f && draws[2].point.y == 55.0f && draws[2].point.z == 132.5f, "wheel 2 position");
    EXPECT(draws[2].model == reinterpret_cast<void*>(0x3002) && draws[2].texture == reinterpret_cast<void*>(0x4002), "wheel 2 words (slot 2 is token 3)");
    EXPECT(draws[2].mask == 0xaa00000000000002ull, "wheel 2 mask");
    // all four wheels, all slots used
    reset();
    for (int s = 0; s < 4; ++s) putw(vehicle, 2088 + s * 4, s + 1);
    msg_draw__7Vehicle(vehicle);
    EXPECT(draw_count == 5 && trans_count == 4, "four wheels");
    EXPECT(trans[3].in == reinterpret_cast<void*>(vehicle + 1932 + 36), "slot 3 point");
    // no wheels
    reset();
    msg_draw__7Vehicle(vehicle);
    EXPECT(draw_count == 1 && resets == 1 && rot_count == 0 && trans_count == 0, "no wheels");
    return 0;
}
