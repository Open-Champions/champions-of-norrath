// Build with -DNORRATH_W10AC_HOST (32-bit pointer words become offsets from
// norrath_w10ac_host_base and the reciprocal square root uses the host square root).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void msg_draw__8Catapult(void*);
unsigned char* norrath_w10ac_host_base;
extern "C" float worldCameraPos[3] = {0.0f, 0.0f, 0.0f};
extern "C" float worldViewMatrix[16] = {};
static unsigned char memory[0x1000];
static unsigned char* object;
static int failures = 0;
static std::vector<std::string> events;
static void* last_matrix;
static int record_token;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void modelSetShadowScale__Ff(float scale) { events.push_back("scale:" + std::to_string(scale)); }
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    last_matrix = matrix;
    float p[3];
    std::memcpy(p, point, 12);
    events.push_back("matrix:" + std::to_string(p[0]) + ":" + std::to_string(p[2]) + ":" + std::to_string(yaw));
}
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int mode, void* matrix,
                                                                               void* animation, char flag) {
    events.push_back("shadow:" + std::to_string(static_cast<unsigned char*>(model) - memory) + ":" +
                     std::to_string(mode) + ":" + (matrix == last_matrix ? "m" : "?") + ":" +
                     (animation == object + 92 ? "anim" : "?") + ":" + std::to_string(flag));
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
                     std::to_string(p[0]) + ":" + (matrix == last_matrix ? "m" : "?") + ":" +
                     (animation == nullptr ? "noanim" : animation == object + 92 ? "anim" : "?") + ":" + buf + ":" +
                     (record == nullptr ? "norecord" : record == &record_token ? "record" : "?") + ":" + std::to_string(last));
    return 0;
}
extern "C" void modelResetOptionalParams__Fv() { events.push_back("reset"); }
static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(memory + off, &v, 4); }

static std::vector<std::string> run(float z, unsigned word84, unsigned char b85, unsigned short yaw) {
    std::memset(memory, 0, sizeof memory);
    events.clear();
    object = memory + 0x10;
    putf(0x10 + 16, 5.0f);
    putf(0x10 + 20, 6.0f);
    putf(0x10 + 24, z);
    put(0x10 + 36, 0x400);
    put(0x400 + 12, 0x500);
    put(0x400 + 20, 0x540);
    put(0x10 + 84, word84);
    memory[0x10 + 85] = b85;
    std::memcpy(memory + 0x10 + 56, &yaw, 2);
    msg_draw__8Catapult(object);
    return events;
}

int main() {
    norrath_w10ac_host_base = memory;
    worldCameraPos[0] = worldCameraPos[1] = worldCameraPos[2] = 0.0f;
    // View third column (2, 6, 10) = (0, 0, 4): normalised to (0, 0, 1) by the reciprocal root.
    worldViewMatrix[2] = 0.0f; worldViewMatrix[6] = 0.0f; worldViewMatrix[10] = 4.0f;
    const std::string scale = "scale:2.917964";
    // Far behind the threshold: shadow record, animation when +85 is nonzero.
    auto e = run(-500.0f, 0, 1, 0xc000);
    expect(e.size() == 5, "five calls with a shadow");
    if (e.size() == 5) {
        expect(e[0] == scale, "shadow scale constant");
        expect(e[1] == "matrix:5.000000:-500.000000:" + std::to_string(static_cast<short>(0xc000 + 16384)), "matrix point and wrapped yaw");
        expect(e[2] == "shadow:1280:1:m:anim:0", "shadow with the model, mode 1 and the animation");
        expect(e[3] == "draw:1280:1344:0:5.000000:m:anim:ffffffffffffffff:record:0", "draw with mode 0, all-ones flags and the record");
        expect(e[4] == "reset", "reset last");
    }
    // Exactly -244 is not below the threshold: no shadow, null record.
    e = run(-244.0f, 0, 0, 0);
    expect(e.size() == 4 && e[2] == "draw:1280:1344:0:5.000000:m:noanim:ffffffffffffffff:norecord:0", "threshold is strict; byte 85 zero means no animation");
    e = run(-244.5f, 0xffff, 0x80, 1);
    expect(e.size() == 5 && e[3].find("anim:ffff") != std::string::npos, "just below the threshold; signed byte 0x80 is nonzero; low halfword of +84 does not stop it");
    // The upper half of the +84 word stops everything.
    e = run(-500.0f, 0x10000, 1, 0);
    expect(e.empty(), "upper bits of +84 suppress the draw");
    e = run(-500.0f, 0x80000000u, 1, 0);
    expect(e.empty(), "top bit suppresses the draw");
    // Facing the other way: dot product positive, no shadow.
    e = run(500.0f, 0, 1, 0);
    expect(e.size() == 4, "positive dot product: no shadow");
    return failures ? 1 : 0;
}
