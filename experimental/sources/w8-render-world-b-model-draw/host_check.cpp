#include <cstring>
#include <vector>

struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
unsigned char* glistp;
signed char modelDrawFade;
int modelOption;
int nmHalos;
const void* halos;
unsigned long long zwritesOff;
unsigned long long zwritesOn;
int scx1, scy1, scx2, scy2;
int engineScissory1, engineScissory2;
unsigned char worldCharViewMatrix[64];
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    unsigned char* model, void* texture, int value, const float* position, void* matrix, void* anim,
    unsigned long long flags, void* record, int level);
}

alignas(16) static unsigned char list[4096];
alignas(16) static unsigned char model[512];
static unsigned char matrix34[48];

static int mat_calls, setup_calls, anim_calls, halo_calls, glow_calls, add_calls;
static void *mat_out, *mat_a, *mat_b;
static float setup_point[3];
static signed char setup_fade;
static const void* setup_model;
static void* setup_matrix;
static Packet* setup_packet;
static unsigned char* setup_start;
static void* anim_arg;
static int halo_count; static const void* halo_ptr; static signed char halo_flag; static const void* halo_model;
static const void* glow_model; static void* glow_anim; static void* glow_matrix;
static void* add_list; static int add_value; static void* add_texture; static void* add_record; static int add_extra;
static unsigned char* anim_start;

extern "C" void matMul__FP8Matrix44P8Matrix34T0(void* out, const void* a, const void* b) {
    ++mat_calls; mat_out = out; mat_a = const_cast<void*>(a); mat_b = const_cast<void*>(b);
}
extern "C" void modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc(Packet* p, void* m, const float* pt, const void* mdl, signed char fade) {
    ++setup_calls; setup_packet = p; setup_matrix = m; setup_model = mdl; setup_fade = fade;
    std::memcpy(setup_point, pt, sizeof setup_point);
    setup_start = p->cursor;
    std::memset(p->cursor, 0xab, 32);
    p->cursor += 32;
}
extern "C" void animLoadBoneMatricies__FP6PacketP14AnimationState(Packet* p, void* a) {
    ++anim_calls; anim_arg = a; anim_start = p->cursor;
    std::memset(p->cursor, 0xcd, 16);
    p->cursor += 16;
}
extern "C" void modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi(Packet*, const void* m, signed char f, const void* h, int n) {
    ++halo_calls; halo_model = m; halo_flag = f; halo_ptr = h; halo_count = n;
}
extern "C" void modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34(const void* m, void* a, void* mat) {
    ++glow_calls; glow_model = m; glow_anim = a; glow_matrix = mat;
}
extern "C" void drawAddToList__FPviP8_textureP11_drawRecordi(void* l, int v, void* t, void* r, int e) {
    ++add_calls; add_list = l; add_value = v; add_texture = t; add_record = r; add_extra = e;
}

static unsigned long long Q(int off) { unsigned long long v; std::memcpy(&v, list + off, 8); return v; }
static unsigned D(int off) { unsigned v; std::memcpy(&v, list + off, 4); return v; }

static void reset(int start_offset) {
    std::memset(list, 0x77, sizeof list);
    std::memset(model, 0, sizeof model);
    glistp = list + start_offset;
    modelDrawFade = 5; modelOption = 0; nmHalos = 0; halos = nullptr;
    zwritesOff = 0x1111222233334444ull; zwritesOn = 0x5555666677778888ull;
    scx1 = 10; scy1 = 20; scx2 = 30; scy2 = 40; engineScissory1 = 7; engineScissory2 = 9;
    mat_calls = setup_calls = anim_calls = halo_calls = glow_calls = add_calls = 0;
}
// Model with `segments` segments per level and `levels` levels; each level's
// boundary words are base + level*0x1000 + i*0x100.
static void make_model(int segments, int levels, unsigned base) {
    model[18] = static_cast<unsigned char>(segments);
    model[38] = static_cast<unsigned char>(levels);
    for (int l = 0; l < levels; ++l)
        for (int i = 0; i <= segments; ++i) {
            const unsigned v = base + l * 0x1000 + i * 0x100;
            std::memcpy(model + 40 + l * (segments * 4 + 4) + i * 4, &v, 4);
        }
}

int main() {
    float pt[3] = {1.0f, 2.0f, 3.0f};
    int texture, record, anim;

    // 1. No options, no animation, no chunk flags.
    reset(0); make_model(2, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 123, pt, matrix34, nullptr, 0, &record, 0);
    if (mat_calls != 1 || mat_a != matrix34 || mat_b != worldCharViewMatrix || setup_calls != 1) return 1;
    if (setup_point[0] != 1.0f || setup_point[1] != 2.0f || setup_point[2] != 3.0f || setup_fade != 5 || setup_model != model) return 2;
    if (setup_matrix != mat_out || setup_start != list) return 3;
    // MSCAL packet at 32 becomes a return tag; list ends at 48.
    if (Q(32) != 0x60000000ull || Q(40) != 0x1400001eull) return 4;
    if (glistp != list + 48) return 5;
    if (add_calls != 1 || add_list != list || add_value != 123 || add_texture != &texture || add_record != &record || add_extra != 0) return 6;
    if (anim_calls != 0 || halo_calls != 0 || glow_calls != 0) return 7;

    // 2. Animation: bone matrices loaded only when the draw record is null.
    reset(0); make_model(2, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, &anim, 0, nullptr, 0);
    if (anim_calls != 1 || anim_arg != &anim || anim_start != list + 32) return 8;
    if (Q(48) != 0x60000000ull || Q(56) != 0x1400000cull || glistp != list + 64) return 9;
    reset(0); make_model(2, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, &anim, 0, &record, 0);
    if (anim_calls != 0 || Q(32) != 0x60000000ull || Q(40) != 0x1400000cull || glistp != list + 48) return 10;

    // 3. Halos and glows follow the model flag bits.
    reset(0); make_model(2, 1, 0x10000);
    model[16] = 0x20; nmHalos = 3; halos = &texture;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, &anim, 0, &record, 0);
    if (halo_calls != 1 || halo_model != model || halo_flag != 1 || halo_ptr != &texture || halo_count != 3 || glow_calls != 0) return 11;
    reset(0); make_model(2, 1, 0x10000);
    model[16] = 0x20; nmHalos = 0;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, &anim, 0, &record, 0);
    if (halo_calls != 0) return 12;
    reset(0); make_model(2, 1, 0x10000);
    model[16] = 0x40;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, &anim, 0, &record, 0);
    if (glow_calls != 1 || glow_model != model || glow_anim != &anim || glow_matrix != matrix34 || halo_calls != 0) return 13;

    // 4. Chunk runs. Boundaries 0x10000 + i*0x100, three segments.
    reset(0); make_model(3, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0x3, &record, 0);   // chunks 0 and 1
    // setup 0..32, run tag at 32, MSCAL packet follows? (MSCAL is built before the runs)
    // MSCAL at 32 (return tag), then one REF tag at 48.
    if (Q(32) != 0x60000000ull) return 14;
    if (Q(48) != ((0x10000ull << 32) | 0x20ull | 0x30000000ull) || Q(56) != 0) return 15;
    if (glistp != list + 64) return 16;
    // Non-adjacent runs, the high flag bit (above the segment count) ignored, 64-bit flags.
    reset(0); make_model(3, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0x1ull | 0x4ull | 0x8ull | (1ull << 40), &record, 0);
    if (Q(48) != ((0x10000ull << 32) | 0x10ull | 0x30000000ull)) return 17;   // chunk 0
    if (Q(64) != ((0x10200ull << 32) | 0x10ull | 0x30000000ull)) return 18;   // chunk 2
    if (glistp != list + 80) return 19;
    // Everything set: one run through the last boundary.
    reset(0); make_model(3, 1, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0x7, &record, 0);
    if (Q(48) != ((0x10000ull << 32) | 0x30ull | 0x30000000ull) || glistp != list + 64) return 20;
    // Level index clamps to [0, count-1].
    reset(0); make_model(2, 2, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0x1, &record, 9);
    if (Q(48) != ((0x11000ull << 32) | 0x10ull | 0x30000000ull)) return 21;
    reset(0); make_model(2, 2, 0x10000);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0x1, &record, -4);
    if (Q(48) != ((0x10000ull << 32) | 0x10ull | 0x30000000ull)) return 22;

    // 5. Option packets, aligned list. Options 1|8|2|4|0x10.
    reset(0); make_model(2, 1, 0x10000);
    modelOption = 1 | 8 | 2 | 4 | 0x10;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0, &record, 0);
    // packet 1 at 32: 2 zbuf + 2 test + dimx + 2 scissor + 2 prmode = 9 entries
    {
        const int n1 = 9;
        const int p1 = 32;
        const int size1 = 32 + 16 * n1;
        if (D(p1 + 8) != 0 || D(p1 + 12) != (0x50000000u | (unsigned)(n1 + 1))) return 23;
        if (Q(p1 + 16) != (0x1000000000008000ull | (unsigned)n1) || Q(p1 + 24) != 14) return 24;
        if (Q(p1) != (unsigned)(n1 + 1) + 0x10000000ull - 0 && Q(p1) != (0x10000000ull | (unsigned)(n1 + 1))) return 25;
        const int e = p1 + 32;
        if (Q(e) != zwritesOff || Q(e + 8) != 78 || Q(e + 16) != zwritesOff || Q(e + 24) != 79) return 26;
        const unsigned long long test = 0x10010ull | 0x40001ull | 0x2000ull;   // ATST never, AFAIL 2, ZTST 2
        if (Q(e + 32) != test || Q(e + 40) != 71 || Q(e + 48) != test || Q(e + 56) != 72) return 27;
        if (Q(e + 64) != 0x1212303012123030ull || Q(e + 72) != 68) return 28;
        const unsigned long long sc = 10ull | (40ull << 48) | (20ull << 32) | (30ull << 16);
        if (Q(e + 80) != sc || Q(e + 88) != 64 || Q(e + 96) != sc || Q(e + 104) != 65) return 29;
        if (Q(e + 112) != 0 || Q(e + 120) != 26 || Q(e + 128) != 128 || Q(e + 136) != 27) return 30;
        // MSCAL packet follows, then the second (restoring) packet.
        const int m = p1 + size1;
        if (Q(m) != 0x10000000ull || Q(m + 8) != 0x1400001eull) return 31;
        const int p2 = m + 16;
        const int n2 = 2 + 1 + 1 + 2 + 1;
        if (D(p2 + 8) != 0x11000000u || D(p2 + 12) != (0x50000000u | (unsigned)(n2 + 1))) return 32;
        if (Q(p2 + 16) != (0x1000000000008000ull | (unsigned)n2) || Q(p2 + 24) != 14) return 33;
        const int f = p2 + 32;
        if (Q(f) != zwritesOn || Q(f + 8) != 78 || Q(f + 16) != zwritesOn || Q(f + 24) != 79) return 34;
        if (Q(f + 32) != 0x50013ull || Q(f + 40) != 72) return 35;
        if (Q(f + 48) != 0x3131757531317575ull || Q(f + 56) != 68) return 36;
        const unsigned long long sc2 = (9ull << 48) | (7ull << 32) | 0x04ff0000ull;
        if (Q(f + 64) != sc2 || Q(f + 72) != 64 || Q(f + 80) != sc2 || Q(f + 88) != 65) return 37;
        if (Q(f + 96) != 1 || Q(f + 104) != 26) return 38;
        // Last tag keeps its count but becomes a return tag.
        if (Q(p2) != (0x60000000ull | (unsigned)(n2 + 1))) return 39;
        if (glistp != list + p2 + 32 + 16 * n2) return 40;
        // The first packet's tag keeps the continue id.
        if ((Q(p1) >> 28 & 7) != 1) return 41;
    }

    // 6. Test register variants: only ZTST bit and bit 0x20.
    reset(0); make_model(2, 1, 0x10000);
    modelOption = 0x20;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0, &record, 0);
    {
        const unsigned long long test = 0x10010ull | (1ull << 17) | 1ull | 2ull;   // ATST always, ZTST 1
        if (Q(64) != test || Q(72) != 71 || Q(80) != test || Q(88) != 72) return 42;
    }

    // 7. Unaligned list: padding of zero bytes up to offset 12 modulo 16.
    reset(8); make_model(2, 1, 0x10000);
    modelOption = 2;
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, &texture, 0, pt, matrix34, nullptr, 0, &record, 0);
    {
        // setup at 8..40; packet at 40; pad 40+8=48..59, VIF at 60? 48 is 0 mod 16, so pad to 60.
        for (int i = 48; i < 60; ++i) if (list[i] != 0) return 43;
        if (D(60) != (0x50000000u | 2u)) return 44;
        if (Q(64) != (0x1000000000008000ull | 1u)) return 45;
        if (Q(80) != 0x1212303012123030ull || Q(88) != 68) return 46;
        if ((reinterpret_cast<unsigned long>(list + 60) & 0xf) != 12) return 47;
    }
    return 0;
}
