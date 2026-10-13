// Build with -DNORRATH_W10AC_HOST (the source then maps 32-bit pointer words to host addresses
// as offsets from norrath_w10ac_host_base).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void msg_draw__5Chest(void*);
unsigned char* norrath_w10ac_host_base;
static unsigned char memory[0x1000];
static int failures = 0;
static std::vector<std::string> events;
static void* last_matrix;
static int record_token;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned char* object;
static std::string hex(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    last_matrix = matrix;
    float p[3];
    std::memcpy(p, point, 12);
    events.push_back("matrix:" + hex(p[0]) + "," + hex(p[1]) + "," + hex(p[2]) + ":" + std::to_string(yaw) +
                     (point == object + 16 ? ":inplace" : ""));
}
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int mode, void* matrix,
                                                                               void* animation, char flag) {
    events.push_back("shadow:" + std::to_string(static_cast<unsigned char*>(model) - memory) + ":" + std::to_string(mode) +
                     ":" + (matrix == last_matrix ? "m" : "?") + ":" +
                     (animation == nullptr ? "noanim" : animation == object + 92 ? "anim" : "?") + ":" + std::to_string(flag));
    return &record_token;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation, unsigned long long flags,
    void* record, int last) {
    float p[3];
    std::memcpy(p, point, 12);
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llx", flags);
    events.push_back("draw:" + std::to_string(static_cast<unsigned char*>(model) - memory) + ":" +
                     std::to_string(static_cast<unsigned char*>(texture) - memory) + ":" + std::to_string(mode) + ":" +
                     hex(p[0]) + "," + hex(p[1]) + "," + hex(p[2]) + (point == object + 16 ? "(inplace)" : "") + ":" +
                     (matrix == last_matrix ? "m" : "?") + ":" +
                     (animation == nullptr ? "noanim" : animation == object + 92 ? "anim" : "?") + ":" + buf + ":" +
                     (record == &record_token ? "record" : "?") + ":" + std::to_string(last));
    return 0;
}
static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
int main() {
    norrath_w10ac_host_base = memory;
    object = memory + 0x10;
    struct Case { unsigned short yaw; signed char flag; unsigned word84; };
    const Case cases[] = {{0, 0, 0}, {1, 1, 0xffffffffu}, {0xc000, -1, 0x80000000u}, {0x4000, 2, 0x7fffffffu}, {0x1234, -128, 5u}};
    for (const Case& c : cases) {
        std::memset(memory, 0, sizeof memory);
        events.clear();
        const float point[3] = {1.5f, -2.0f, 3.25f};
        std::memcpy(object + 16, point, 12);
        std::memcpy(object + 56, &c.yaw, 2);
        object[96] = static_cast<unsigned char>(c.flag);
        put(0x10 + 84, c.word84);
        put(0x10 + 36, 0x400);
        put(0x400 + 12, 0x500);
        put(0x400 + 16, 0x580);
        put(0x400 + 20, 0x540);
        unsigned char before[200];
        std::memcpy(before, object, sizeof before);
        msg_draw__5Chest(object);
        short want_yaw = static_cast<short>(static_cast<unsigned short>(c.yaw + 16384));
        const char* anim = c.flag != 0 ? "anim" : "noanim";
        char flags[32];
        std::snprintf(flags, sizeof flags, "%llx", static_cast<unsigned long long>(static_cast<long long>(static_cast<int>(c.word84))));
        std::vector<std::string> want = {
            "matrix:1.5,-2,3.25:" + std::to_string(want_yaw),
            "shadow:" + std::to_string(0x500) + ":1:m:" + anim + ":0",
            "draw:" + std::to_string(0x500) + ":" + std::to_string(0x540) + ":1:1.5,-2,3.25:m:" + anim + ":" + flags + ":record:0"};
        expect(events == want, "call sequence and arguments");
        if (events != want) {
            for (auto& e : events) std::fprintf(stderr, "  got  %s\n", e.c_str());
            for (auto& e : want) std::fprintf(stderr, "  want %s\n", e.c_str());
        }
        expect(std::memcmp(before, object, sizeof before) == 0, "object not written");
    }
    return failures ? 1 : 0;
}
