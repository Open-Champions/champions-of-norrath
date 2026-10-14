// Host contract for norrath_vehicle_reset. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_vehicle_reset(void* object, int value);
extern "C" void norrath_vehicle_host_note(void* full);
extern "C" unsigned char norrath_vehicle_specs_bytes[64];
extern "C" {
unsigned norrath_actor_view_4844a4;
}

static int failures = 0;
static unsigned char object[2300];
static unsigned char alternate[64];
static unsigned char wheel_a[400];
static unsigned char wheel_b[400];
static const char* calls[8];
static int step = 0;
static int wheel_calls = 0;
static void* wheel_seen[2];
static short seen_yaw = 1;
static unsigned seen_point[3];
static int shrink = 0;
static unsigned installed_half = 100u;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned bits_of(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}
static float float_from(unsigned bits) {
    float value = 0;
    std::memcpy(&value, &bits, 4);
    return value;
}
static unsigned low(void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static void note(const char* name) {
    if (step < 8) calls[step] = name;
    ++step;
}

extern "C" void refreshSettings__7Vehicle(void* storage) {
    note("refresh");
    expect(storage == object, "refresh object");
    expect(read_word(object + 2052) == 0x5a5a5a5au, "spec stored after refresh");
    write_word(object + 1632, 4u);
    write_word(object + 1636, 5u);
    write_word(object + 1640, 6u);
    object[1048] = static_cast<unsigned char>(installed_half);
    object[1049] = static_cast<unsigned char>(installed_half >> 8);
}
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    note("make");
    seen_yaw = yaw;
    std::memcpy(seen_point, point, 12);
    std::memset(matrix, 0x11, 16);
    expect(read_word(object + 2052) == low(norrath_vehicle_specs_bytes), "stored spec");
}
extern "C" void matToQuaternion__FP4QuatP8Matrix34(void* quat, void* matrix) {
    note("quat");
    expect(quat == object + 1084, "quaternion slot");
    expect(static_cast<unsigned char*>(matrix)[0] == 0x11, "matrix from make");
    std::memset(quat, 0x22, 16);
}
extern "C" void quatToMat__FP8Matrix33P4Quat(void* matrix, void* quat) {
    note("tomat");
    expect(matrix == object + 1116, "orientation slot");
    expect(static_cast<unsigned char*>(quat)[0] == 0x22, "quaternion bytes");
    write_word(object + 2052, low(alternate));
}
extern "C" void Reset__5Wheel(void* wheel) {
    if (wheel_calls < 2) wheel_seen[wheel_calls] = wheel;
    ++wheel_calls;
    if (shrink && wheel_calls == 1) write_word(object + 1000, 1u);
    write_word(alternate + 0, bits_of(16.0f));
    write_word(alternate + 4, bits_of(4.0f));
}

static void prepare() {
    std::memset(object, 0x5a, sizeof object);
    std::memset(alternate, 0, sizeof alternate);
    std::memset(wheel_a, 0, sizeof wheel_a);
    std::memset(wheel_b, 0, sizeof wheel_b);
    write_word(alternate + 48, bits_of(8.0f));
    write_word(alternate, bits_of(8.0f));
    write_word(alternate + 4, bits_of(2.0f));
    write_word(norrath_vehicle_specs_bytes + 48, bits_of(4.0f));
    write_word(object + 820, bits_of(2.0f));
    write_word(object + 912, bits_of(4.0f));
    write_word(wheel_b + 380, bits_of(5.0f));
    write_word(wheel_a + 380, bits_of(9.0f));
    norrath_vehicle_host_note(alternate);
    norrath_vehicle_host_note(wheel_a);
    norrath_vehicle_host_note(wheel_b);
    norrath_actor_view_4844a4 = 0x1234u;
    step = 0;
    wheel_calls = 0;
    shrink = 0;
    installed_half = 100u;
    for (int i = 0; i < 8; ++i) calls[i] = "";
}

int main() {
    prepare();
    write_word(object + 1000, 2u);
    write_word(object + 1052, low(wheel_a));
    write_word(object + 1056, low(wheel_b));
    write_word(wheel_a + 388, 0u);
    write_word(wheel_b + 388, 1u);
    const unsigned result = norrath_vehicle_reset(object, 0);
    expect(result == low(wheel_b), "selected wheel word");
    expect(step == 4 && std::strcmp(calls[0], "refresh") == 0 && std::strcmp(calls[1], "make") == 0, "call order");
    expect(std::strcmp(calls[2], "quat") == 0 && std::strcmp(calls[3], "tomat") == 0, "matrix order");
    expect(wheel_calls == 2 && wheel_seen[0] == wheel_a && wheel_seen[1] == wheel_b, "both wheels");
    expect(seen_yaw == 16484 && seen_point[0] == 4u && seen_point[1] == 5u && seen_point[2] == 6u, "yaw and point");
    expect(read_word(object + 16) == 4u && read_word(object + 24) == 6u, "copied position");
    expect(read_word(object + 2160) == 0x1234u, "frame word");
    expect(read_word(object + 1188) == 0u && read_word(object + 1200) == 0u, "cleared anchors");
    expect(read_word(object + 2152) == 0u && read_word(object + 1028) == 0u && read_word(object + 1040) == 0u, "cleared words");
    expect(read_word(object + 848) == 0u && read_word(object + 752) == 0u && read_word(object + 748) == 0u, "cleared floats");
    expect(read_word(object + 740) == 0u && read_word(object + 808) == 0u, "copies of the cleared word");
    expect(read_word(object + 760) == bits_of(0.5f), "reciprocal");
    expect(read_word(object + 756) == bits_of(8.0f * float_from(0x3da3d70au)), "scaled spec");
    expect(object[108] == 0 && object[109] == 0 && object[110] == 0, "cleared bytes");
    expect(read_word(object + 676) == 0u && read_word(object + 680) == 0u, "first zero pair");
    expect(read_word(object + 708) == 0u && read_word(object + 712) == 0u, "second zero pair");
    expect(read_word(object + 684) == 0x5a5a5a5au && read_word(object + 716) == 0x5a5a5a5au, "zero loop stops at the count");
    const float ratio = (4.0f / (16.0f / 4.0f)) * float_from(0x3d888888u) * 5.0f;
    expect(read_word(object + 832) == bits_of(ratio), "selected ratio");

    prepare();
    write_word(object + 1000, 0u);
    installed_half = 0xc000u;
    const unsigned skipped = norrath_vehicle_reset(object, 0);
    expect(skipped == 0xffffffffu && wheel_calls == 0, "no wheel returns -1");
    expect(seen_yaw == 0, "half 0xC000 narrows to zero");
    expect(read_word(object + 676) == 0x5a5a5a5au, "nonpositive count does not clear");
    expect(read_word(object + 756) == bits_of(8.0f * float_from(0x3da3d70au)), "height scale still runs");

    prepare();
    shrink = 1;
    write_word(object + 1000, 2u);
    write_word(object + 1052, low(wheel_a));
    write_word(object + 1056, low(wheel_b));
    write_word(wheel_a + 388, 1u);
    const unsigned shortened = norrath_vehicle_reset(object, 0);
    expect(wheel_calls == 1 && wheel_seen[0] == wheel_a, "reloaded count stops the walk");
    expect(shortened == low(wheel_a), "first marked wheel");
    expect(read_word(object + 680) == 0u, "zero loop used the original count");

    if (failures != 0) return 1;
    std::printf("vehicle reset passed\n");
    return 0;
}
