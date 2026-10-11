// Host contract for __6Camera. Not an EE plan input.
#include "camera_ctor.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int modes = 0;
static int resets = 0;
static unsigned char* mode_camera = 0;
static unsigned char* reset_camera = 0;
static int mode_argument = -1;
static int reset_first = -1;
static int reset_second = -1;
static unsigned char at_mode[12];
static char order[8];
static int order_used = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::uint16_t load_u16(const unsigned char* address) {
    return static_cast<std::uint16_t>(address[0] | (address[1] << 8));
}

static void mark(char step) {
    if (order_used < 7) order[order_used++] = step;
    order[order_used] = 0;
}

extern "C" void SetMode__6Camerai(unsigned char* camera, int mode) {
    ++modes;
    mode_camera = camera;
    mode_argument = mode;
    std::memcpy(at_mode, camera + 126, 12);
    mark('M');
}

extern "C" void Reset__6Cameraii(unsigned char* camera, int first, int second) {
    ++resets;
    reset_camera = camera;
    reset_first = first;
    reset_second = second;
    mark('R');
}

int main() {
    unsigned char object[160];
    std::memset(object, 0x5a, sizeof object);
    __6Camera(object);
    expect(modes == 1 && resets == 1, "one mode call and one reset");
    expect(std::strcmp(order, "MR") == 0, "mode runs before reset");
    expect(mode_camera == object && mode_argument == 1, "mode receives the object and 1");
    expect(reset_camera == object && reset_first == 1 && reset_second == 1, "reset receives the object, 1, 1");
    expect(load_u16(at_mode + 0) == 100, "halfword 126 is 100 before mode");
    expect(load_u16(at_mode + 2) == 4000, "halfword 128 is 4000 before mode");
    expect(load_u16(at_mode + 4) == 6000, "halfword 130 is 6000 before mode");
    expect(load_u16(at_mode + 6) == 8000, "halfword 132 is 8000 before mode");
    expect(load_u16(at_mode + 8) == 9000, "halfword 134 is 9000 before mode");
    expect(load_u16(at_mode + 10) == 10000, "halfword 136 is 10000 before mode");
    expect(object[125] == 0x5a && object[138] == 0x5a, "bytes outside 126..137 stay");
    if (failures != 0) return 1;
    std::puts("__6Camera contract passed");
    return 0;
}
