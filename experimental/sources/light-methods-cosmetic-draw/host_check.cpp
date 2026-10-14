#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int norrath_vt_msg_draw_12CosmeticProp(void* object);

static int failures = 0;
static int yaw_calls = 0;
static int draw_calls = 0;
static short seen_yaw = 0;
static unsigned char seen_yaw_point[12];
static unsigned char seen_draw_point[12];
static void* yaw_matrix = nullptr;
static void* draw_matrix = nullptr;
static unsigned seen_model = 0;
static unsigned seen_texture = 0;
static int seen_mode = 0;
static unsigned seen_flags = 0;
static void* seen_animation = reinterpret_cast<void*>(1);
static void* seen_record = reinterpret_cast<void*>(1);
static int seen_last = 7;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    ++yaw_calls;
    yaw_matrix = matrix;
    seen_yaw = yaw;
    std::memcpy(seen_yaw_point, point, 12);
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last) {
    ++draw_calls;
    if (yaw_calls == 0) return -1;
    seen_model = pointer_word(model);
    seen_texture = pointer_word(texture);
    seen_mode = mode;
    draw_matrix = matrix;
    std::memcpy(seen_draw_point, point, 12);
    seen_animation = animation;
    seen_flags = flags;
    seen_record = draw_record;
    seen_last = last;
    return 44;
}

int main() {
    unsigned char object[96];
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
    write_half(object + 56, 0);
    write_word(object + 88, 0x11110088u);
    write_word(object + 92, 0x22220092u);
    int result = norrath_vt_msg_draw_12CosmeticProp(object);
    expect(result == 44 && yaw_calls == 1 && draw_calls == 1, "returns the model draw result after both calls");
    expect(seen_yaw == 16384, "a zero halfword becomes 16384");
    expect(std::memcmp(seen_yaw_point, object + 16, 12) == 0 && std::memcmp(seen_draw_point, object + 16, 12) == 0,
           "both calls receive the 12-byte point");
    expect(yaw_matrix == draw_matrix && yaw_matrix != nullptr, "the model draw receives the matrix just built");
    expect(seen_model == 0x11110088u && seen_texture == 0x22220092u && seen_mode == 1, "model, texture, and mode");
    expect(seen_animation == nullptr && seen_flags == 0xffffffffu && seen_record == nullptr && seen_last == 0,
           "animation, flags, record, and the final argument");

    write_half(object + 56, 0x4000u);
    norrath_vt_msg_draw_12CosmeticProp(object);
    expect(seen_yaw == static_cast<short>(-32768), "16384 plus 16384 sign-extends through 16 bits");

    write_half(object + 56, 0xc000u);
    norrath_vt_msg_draw_12CosmeticProp(object);
    expect(seen_yaw == 0, "0xc000 plus 16384 wraps to zero");
    return failures == 0 ? 0 : 1;
}
