// Host check for msg_draw__8ItemProp. Every callee is a stub that logs its
// arguments, so the order, the stack copies, the float math and the gates are
// all compared with the expected values.
#define NORRATH_INVENTORY_HOST 1
#include "msg_draw.cpp"
#include <cstdint>
#include <cstring>
#include <string>

int frameNm = 100;
void* haloTexture1 = reinterpret_cast<void*>(0x1234);

static std::string log_text;
static unsigned char record_store[64];
namespace inventory_items { const void* host_resolve(unsigned word) { return word == 0x55 ? record_store : nullptr; } }

static float isin_result = -0.5f;
static int isin_arg;
static unsigned char yaw_point[12];
static void* yaw_matrix;
static int yaw_value;
static unsigned char halo_seen[8];
static void* halo_pointer;
static int halo_mode;
static void* setup_description;
static float setup_scale;
static unsigned char sprite_point[12];
static float sprite_first, sprite_second;
static int sprite_ints[4];
static void* sprite_texture;
static int sprite_angle, sprite_mode;
static void* draw_model;
static void* draw_texture;
static int draw_mode;
static unsigned char draw_point[12];
static void* draw_matrix;
static void* draw_animation;
static unsigned long long draw_flags;
static void* draw_record;
static int draw_last = -1;

extern "C" float isin__Fi(int angle) { log_text += "I"; isin_arg = angle; return isin_result; }
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    log_text += "Y"; yaw_matrix = matrix; std::memcpy(yaw_point, point, 12); yaw_value = yaw;
}
extern "C" void modelSetHalo__FP7HaloDefi(void* halo_record, int mode) {
    log_text += "H"; halo_pointer = halo_record; std::memcpy(halo_seen, halo_record, 8); halo_mode = mode;
}
extern "C" void setupHalo__4itemPQ24item11descriptionf(void* description, float scale) {
    log_text += "h"; setup_description = description; setup_scale = scale;
}
extern "C" void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int a, int b,
                                                          int c, int d, void* texture, short angle, int mode) {
    log_text += "S"; std::memcpy(sprite_point, point, 12);
    sprite_first = first; sprite_second = second;
    sprite_ints[0] = a; sprite_ints[1] = b; sprite_ints[2] = c; sprite_ints[3] = d;
    sprite_texture = texture; sprite_angle = angle; sprite_mode = mode;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* record, int last) {
    log_text += "D"; draw_model = model; draw_texture = texture; draw_mode = mode; std::memcpy(draw_point, point, 12);
    draw_matrix = matrix; draw_animation = animation; draw_flags = flags; draw_record = record; draw_last = last;
    return 0;
}
extern "C" void modelResetOptionalParams__Fv() { log_text += "R"; }

static void word(unsigned char* o, int off, unsigned v) { std::memcpy(o + off, &v, 4); }
static void half(unsigned char* o, int off, unsigned short v) { std::memcpy(o + off, &v, 2); }
static void reset(unsigned char* object) {
    std::memset(object, 0, 400);
    std::memset(record_store, 0, sizeof record_store);
    log_text.clear();
    word(object, 328, 0x55);
    for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x30 + i);
    word(object, 76, 0xaaaa00);
    word(object, 80, 0xbbbb00);
    word(object, 232, 0x11223344u);
    word(object, 236, 0x55667788u);
}
#define CHECK(c, code) do { if (!(c)) return code; } while (0)

int main() {
    alignas(8) unsigned char object[400];

    // Step 1: bit 0 of the halfword at +320 clear runs the yaw matrix.
    reset(object);
    half(object, 56, 0x7f00);
    word(object, 260, 0);                       // frameNm - 3 < 0 is false: setupHalo path
    frameNm = 100;
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YhDR", 1);
    CHECK(yaw_matrix == object + 272 && std::memcmp(yaw_point, object + 16, 12) == 0, 2);
    CHECK(yaw_value == static_cast<short>(0x7f00 + 16384), 3);
    CHECK(setup_description == object + 328 && setup_scale == 5.0f, 4);
    CHECK(draw_model == reinterpret_cast<void*>(0xaaaa00) && draw_texture == reinterpret_cast<void*>(0xbbbb00), 5);
    CHECK(draw_mode == 1 && std::memcmp(draw_point, object + 16, 12) == 0 && draw_matrix == object + 272, 6);
    CHECK(draw_animation == nullptr && draw_record == nullptr && draw_last == 0, 7);
    CHECK(draw_flags == 0x5566778811223344ull, 8);
    half(object, 320, 1);
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "hDR", 9);

    // Step 2: the three halo conditions.
    reset(object);
    word(object, 260, 98);                      // 100 - 3 < 98
    isin_result = -0.5f;
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 10);
    CHECK(isin_arg == (100 << 9) && halo_mode == 1, 11);
    CHECK(halo_seen[0] == 255 && halo_seen[1] == 255 && halo_seen[2] == 255 && halo_seen[3] == 18, 12);
    float size;
    std::memcpy(&size, halo_seen + 4, 4);
    CHECK(size == 409.0f, 13);
    reset(object);
    word(object, 260, 97);                      // 97 < 97 is false
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YhDR", 14);
    reset(object);
    word(object, 260, 97);
    record_store[44] = 0x80;                    // bit 39 of the 64-bit value at record+40
    isin_result = 1.0f;
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR" && halo_seen[3] == 24, 15);
    reset(object);
    word(object, 260, 97);
    record_store[44] = 0x7f;
    record_store[40] = 0xff;                    // other bytes do not count
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YhDR", 16);

    // Step 3: the sprite needs bit 28 of the value at +336 and a low byte below 30.
    const float c1 = __builtin_bit_cast(float, 0x3d888888u);
    const float c2 = __builtin_bit_cast(float, 0x45bb8000u);
    reset(object);
    word(object, 260, 1000);
    object[339] = 0x10;
    frameNm = 0x100 + 20;                        // low byte 20, low six bits 20
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHSDR", 20);
    float scale = static_cast<float>(10) * c1;
    scale = scale * scale;
    CHECK(sprite_first == 10.0f && sprite_second == scale * c2, 21);
    CHECK(sprite_ints[0] == 128 && sprite_ints[1] == 128 && sprite_ints[2] == 128 && sprite_ints[3] == static_cast<int>(scale * 64.0f), 22);
    CHECK(sprite_texture == reinterpret_cast<void*>(0x1234) && sprite_mode == 3, 23);
    CHECK(sprite_angle == static_cast<short>(20 << 10), 24);
    CHECK(std::memcmp(sprite_point, object + 16, 12) == 0, 25);
    frameNm = 0x100 + 40;                        // low byte 40: skipped
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 26);
    frameNm = 0x100 + 29;                        // low byte 29: distance 14, n = 1, angle bit 5 set
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHSDR", 27);
    CHECK(sprite_angle == static_cast<short>((29 & 63) << 10), 28);
    scale = static_cast<float>(1) * c1;
    scale = scale * scale;
    CHECK(sprite_second == scale * c2, 29);
    frameNm = 0x100 + 0x3f;                      // low byte 63: skipped
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 30);
    frameNm = 0x200;                             // low byte 0: n = 0, size 0
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHSDR" && sprite_second == 0.0f && sprite_ints[3] == 0, 31);
    frameNm = 0x7e;                              // low byte 126: skipped
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 32);
    frameNm = 15;
    object[339] = 0x0f;                          // other bits of that byte do not count
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 33);
    object[339] = 0xe0;
    log_text.clear();
    msg_draw__8ItemProp(object);
    CHECK(log_text == "YIHDR", 34);
    frameNm = 100;
    return 0;
}
