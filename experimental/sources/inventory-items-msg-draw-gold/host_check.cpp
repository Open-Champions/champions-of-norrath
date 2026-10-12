// Host check for msg_draw__4Gold. Build with -ffp-contract=off so the float
// expressions are not fused. Callees are stubs that log their arguments.
#include "msg_draw.cpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

int frameNm = 0;
void* haloTexture1 = reinterpret_cast<void*>(0x1111);
void* haloTexture2 = reinterpret_cast<void*>(0x2222);
unsigned char goldModel[16];
unsigned char goldTex[16];

struct Sprite {
    unsigned char point[12];
    float first, second;
    int red, green, blue, alpha;
    void* texture;
    int angle, mode;
};
static std::vector<Sprite> sprites;
static std::string log_text;
static unsigned char* yaw_matrix;
static unsigned char yaw_point[12];
static int yaw_value;
static void* draw_model; static void* draw_texture; static int draw_mode;
static unsigned char draw_point[12];
static void* draw_matrix; static void* draw_animation; static unsigned long long draw_flags;
static void* draw_record; static int draw_last = -1;


extern "C" void* get__11ResourceMap(void* map) {

    log_text += map == goldModel ? "m" : map == goldTex ? "t" : "?";
    return map == goldModel ? reinterpret_cast<void*>(0xaa01) : reinterpret_cast<void*>(0xaa02);
}
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    log_text += "Y"; yaw_matrix = static_cast<unsigned char*>(matrix); std::memcpy(yaw_point, point, 12); yaw_value = yaw;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* record, int last) {
    log_text += "D"; draw_model = model; draw_texture = texture; draw_mode = mode; std::memcpy(draw_point, point, 12);
    draw_matrix = matrix; draw_animation = animation; draw_flags = flags; draw_record = record; draw_last = last;
    return 0;
}
extern "C" void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int a, int b,
                                                          int c, int d, void* texture, short angle, int mode) {
    log_text += "S";
    Sprite s;
    std::memcpy(s.point, point, 12);
    s.first = first; s.second = second; s.red = a; s.green = b; s.blue = c; s.alpha = d;
    s.texture = texture; s.angle = angle; s.mode = mode;
    sprites.push_back(s);
}

static void word(unsigned char* o, int off, unsigned v) { std::memcpy(o + off, &v, 4); }
static unsigned get_word(const unsigned char* o, int off) { unsigned v; std::memcpy(&v, o + off, 4); return v; }
static float fbits(unsigned v) { float f; std::memcpy(&f, &v, 4); return f; }
static void fill(unsigned char* object, int count) {
    std::memset(object, 0, 200);
    word(object, 128, static_cast<unsigned>(count));
    float p[3] = {1.5f, -2.25f, 100.0f};
    std::memcpy(object + 16, p, 12);
    unsigned short yaw = 0xfff0;
    std::memcpy(object + 56, &yaw, 2);
    word(object, 68, 12345);
    for (int i = 0; i < 5; ++i) {
        float q[3] = {float(10 * i), float(20 * i), float(30 * i)};
        std::memcpy(object + 132 + 12 * i, q, 12);
    }
    sprites.clear(); log_text.clear();
}
#define CHECK(c, code) do { if (!(c)) return code; } while (0)

// The same generator, written independently for expectations.
static float next_unit(unsigned& seed) {
    seed = seed * 1664525u + 1013904223u;
    float u = static_cast<float>(seed >> 16) / 65536.0f;
    return u * 2.0f - 1.0f;
}

int main() {
    alignas(8) unsigned char object[200];

    // Zero count: nothing at all.
    fill(object, 0);
    msg_draw__4Gold(object);
    CHECK(log_text.empty(), 1);

    // Count 3: model, main sprite, no sparkles.
    fill(object, 3);
    frameNm = 1000;
    word(object, 80, 900);
    msg_draw__4Gold(object);
    CHECK(log_text == "YmtDS", 2);
    CHECK(yaw_matrix == draw_matrix && std::memcmp(yaw_point, object + 16, 12) == 0 && yaw_value == static_cast<short>(0xfff0), 3);
    CHECK(draw_model == reinterpret_cast<void*>(0xaa01) && draw_texture == reinterpret_cast<void*>(0xaa02) && draw_mode == 1, 4);
    CHECK(std::memcmp(draw_point, object + 16, 12) == 0 && draw_animation == object + 88, 5);
    CHECK(draw_flags == 0xfull && draw_record == nullptr && draw_last == 0, 6);   // (1 << 4) - 1
    CHECK(sprites.size() == 1, 7);
    CHECK(sprites[0].first == 12.0f && sprites[0].second == float(114 * 3 + 6900), 8);
    CHECK(sprites[0].red == 255 && sprites[0].green == 200 && sprites[0].blue == 50 && sprites[0].alpha == 7, 9);
    CHECK(sprites[0].texture == haloTexture2 && sprites[0].angle == 0 && sprites[0].mode == 2, 10);
    CHECK(std::memcmp(sprites[0].point, object + 16, 12) == 0, 11);

    // The mask and size saturate: 62 and 50.
    fill(object, 200);
    frameNm = 5000; word(object, 80, 1);
    msg_draw__4Gold(object);
    CHECK(draw_flags == ((1ull << 63) - 1ull), 12);
    CHECK(sprites[0].second == float(114 * 50 + 6900) && sprites[0].alpha == 25, 13);
    fill(object, 62);
    msg_draw__4Gold(object);
    CHECK(draw_flags == ((1ull << 63) - 1ull), 14);
    fill(object, 61);
    msg_draw__4Gold(object);
    CHECK(draw_flags == ((1ull << 62) - 1ull), 15);
    fill(object, -5);                            // the shift count wraps modulo 64 like dsllv
    frameNm = 5000; word(object, 80, 1);
    msg_draw__4Gold(object);
    CHECK(log_text == "YmtDS" && draw_flags == ((1ull << 60) - 1ull), 16);

    // Sparkles: none at count 10, five at count 11.
    fill(object, 10);
    frameNm = 0; word(object, 80, 0);
    msg_draw__4Gold(object);
    CHECK(sprites.size() == 1, 20);
    const float scale = fbits(0x3d088888u);
    fill(object, 11);
    frameNm = 4000; word(object, 80, 0);          // 4000 % 60 = 40: ages 40, 52, 4, 16, 28
    msg_draw__4Gold(object);
    CHECK(sprites.size() == 6 && log_text == "YmtDSSSSSS", 21);
    CHECK(sprites[0].alpha == 15, 22);              // min(count + 4, 25) = 15
    for (int i = 0; i < 5; ++i) {
        const Sprite& s = sprites[1 + i];
        int age = (4000 + 12 * i) % 60;
        int d = age - 30; if (d < 0) d = -d;
        float want = float(int((1.0f - float(d) * scale) * 2000.0f));
        CHECK(s.first == 12.0f && s.second == want, 30 + i);
        CHECK(s.red == 255 && s.green == 200 && s.blue == 50 && s.alpha == 30, 40 + i);
        CHECK(s.texture == haloTexture1 && s.mode == 2, 50 + i);
        CHECK(s.angle == static_cast<short>((4000 << 12) + (i << 15)), 60 + i);
        CHECK(std::memcmp(s.point, object + 132 + 12 * i, 12) == 0, 70 + i);
        float q[3] = {float(10 * i), float(20 * i), float(30 * i)};
        CHECK(std::memcmp(object + 132 + 12 * i, q, 12) == 0, 80 + i);       // not respawned
    }
    CHECK(get_word(object, 68) == 12345, 90);

    // Respawn: choose a seed whose first attempt is rejected, so the loop is exercised.
    unsigned seed = 1;
    int attempts = 0;
    for (;; ++seed) {
        unsigned t = seed;
        attempts = 0;
        float x, y, z;
        do { x = next_unit(t); y = next_unit(t); z = next_unit(t); ++attempts; } while (1.0f < x * x + y * y + z * z);
        if (attempts > 1) break;
    }
    unsigned t = seed;
    float x, y, z;
    do { x = next_unit(t); y = next_unit(t); z = next_unit(t); } while (1.0f < x * x + y * y + z * z);
    fill(object, 11);
    word(object, 68, seed);
    frameNm = 35;                                  // ages 35, 47, 59, 11, 23
    msg_draw__4Gold(object);
    CHECK(log_text == "YmtDSSSSSS", 100);
    CHECK(get_word(object, 68) == t, 101);
    const float ox = 1.5f, oy = -2.25f, oz = 100.0f;
    float spawn[3] = {ox + x * 8.0f + 0.0f, oy + y * 8.0f + 0.0f, oz + z * 8.0f + 8.0f};
    CHECK(std::memcmp(object + 132 + 24, spawn, 12) == 0, 102);
    CHECK(std::memcmp(sprites[3].point, spawn, 12) == 0, 103);
    CHECK(sprites[3].second == float(int((1.0f - 30.0f * scale) * 2000.0f)), 104);   // age counts as 0
    float untouched[3] = {10.0f, 20.0f, 30.0f};
    CHECK(std::memcmp(object + 132 + 12, untouched, 12) == 0, 105);
    CHECK(sprites[1].angle == static_cast<short>((35 << 12) + 0), 106);
    CHECK(sprites[3].angle == static_cast<short>((35 << 12) + (2 << 15)), 107);

    // Respawn of slot 0 at frameNm 59 ((59 + 0) % 60 == 59).
    fill(object, 11);
    word(object, 68, 777);
    frameNm = 59;
    msg_draw__4Gold(object);
    CHECK(std::memcmp(sprites[1].point, object + 132, 12) == 0 && get_word(object, 68) != 777, 110);
    // Alpha limit from the stamp: frameNm - stamp - 20 can be below count + 4.
    fill(object, 20);
    frameNm = 100; word(object, 80, 90);          // 100 - 90 - 20 = -10: clamps to 0
    msg_draw__4Gold(object);
    CHECK(sprites[0].alpha == 0 && sprites[1].alpha == 0, 120);
    fill(object, 20);
    frameNm = 130; word(object, 80, 100);         // 130 - 100 - 20 = 10 < 24
    msg_draw__4Gold(object);
    CHECK(sprites[0].alpha == 10 && sprites[1].alpha == 20, 121);
    return 0;
}
