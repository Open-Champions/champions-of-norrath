#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" unsigned char modelClippingOn = 0;
#include "msg_draw.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

alignas(8) static unsigned char object[512];
static std::vector<std::string> log_calls;
static std::vector<std::string> tags;
static int shadow_token[8];
static int shadow_count;
static std::vector<void*> shadow_results;
static void* last_matrix;
static std::vector<void*> draw_records;
static std::vector<std::string> lump_names;

static std::string to_s(unsigned long long v) {
    char buf[40];
    std::snprintf(buf, sizeof buf, "%llx", v);
    return buf;
}
static std::string colour(const short* c) {
    return std::to_string(c[0]) + "," + std::to_string(c[1]) + "," + std::to_string(c[2]) + "," + std::to_string(c[3]);
}

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    last_matrix = matrix;
    float p[3];
    std::memcpy(p, point, 12);
    log_calls.push_back("matrix:" + std::to_string(static_cast<int>(p[0])) + ":" + std::to_string(yaw));
}
extern "C" void lightSetAmbientColor__FPs(short* c) { log_calls.push_back("ambient:" + colour(c)); }
extern "C" void lightSetAmbientOnly__Fv() { log_calls.push_back("ambient-only"); }
extern "C" void lightResetOptionalParams__Fv() { log_calls.push_back("light-reset"); }
extern "C" void modelSetShadowScale__Ff(float scale) { log_calls.push_back(scale == 2.0f ? "shadow-scale:2" : "shadow-scale:?"); }
extern "C" void modelResetOptionalParams__Fv() { log_calls.push_back("model-reset"); }
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag) {
    std::string tag = "shadow:" + to_s(pointer_word(model)) + ":" + std::to_string(mode) + ":" +
                      (matrix == last_matrix ? "m" : "?") + ":" + (animation == nullptr ? "noanim" : animation == object + 152 ? "anim" : "?") +
                      ":" + std::to_string(static_cast<int>(flag));
    log_calls.push_back(tag);
    return &shadow_token[shadow_count++ % 4];
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* record, int last) {
    float p[3];
    std::memcpy(p, point, 12);
    log_calls.push_back("draw:" + to_s(pointer_word(model)) + ":" + to_s(pointer_word(texture)) + ":" + std::to_string(mode) + ":" +
                        std::to_string(static_cast<int>(p[0])) + ":" + (matrix == last_matrix ? "m" : "?") + ":" +
                        (animation == nullptr ? "noanim" : animation == object + 152 ? "anim" : "?") + ":" + to_s(flags) + ":" +
                        std::to_string(last));
    draw_records.push_back(record);
    return 0;
}
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    log_calls.push_back(std::string("tag:") + tag + ":" + std::to_string(index));
    if (std::strcmp(tag, "multiwak") != 0 || index >= static_cast<int>(tags.size())) return nullptr;
    return const_cast<char*>(tags[index].c_str());
}
extern "C" int format_text(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const char* s = va_arg(args, const char*);
    int n = std::snprintf(buffer, 128, format, s);
    va_end(args);
    log_calls.push_back(std::string("format:") + buffer);
    return n;
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_calls.push_back(std::string("lump:") + lump + ":" + name);
    lump_names.push_back(name);
    // results are small integers carried as pointers
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x100 + lump_names.size()));
}

static void put(unsigned off, unsigned value) { std::memcpy(object + off, &value, 4); }

static void setup(unsigned flags, unsigned mode, int has_parts) {
    std::memset(object, 0, sizeof object);
    float pos[3] = {7.0f, 8.0f, 9.0f};
    std::memcpy(object + 16, pos, 12);
    short yaw = -2;
    std::memcpy(object + 56, &yaw, 2);
    put(12, flags);
    put(208, mode);
    put(144, 0x1111u);
    put(148, 0x2222u);
    object[108] = static_cast<unsigned char>(has_parts);
    std::strcpy(reinterpret_cast<char*>(object + 109), "lumpname");
    unsigned long long big = 0x1122334455667788ull;
    std::memcpy(object + 200, &big, 8);
    log_calls.clear();
    draw_records.clear();
    lump_names.clear();
    tags.clear();
    modelClippingOn = 0;
    shadow_count = 0;
}
static bool is(const std::vector<std::string>& want) { return log_calls == want; }

int main() {
    // A: flash colour bit, animation present, shadow drawn, clipping byte 40 == 255
    setup(0x100, 0, 0);
    object[156] = 1;
    object[40] = 255;
    msg_draw__10AutoObject(object);
    expect(is({"matrix:7:-2", "ambient:255,255,255,0", "shadow-scale:2", "shadow:1111:1:m:anim:0",
               "draw:1111:2222:1:7:m:anim:1122334455667788:0", "model-reset", "light-reset"}),
           "case A sequence");
    expect(modelClippingOn == 1, "byte 40 == 255 sets modelClippingOn");
    expect(draw_records.size() == 1u && draw_records[0] == &shadow_token[0], "shadow result is the draw record");
    unsigned flags_after;
    std::memcpy(&flags_after, object + 12, 4);
    expect(flags_after == 0u, "bit 8 cleared");

    // B: no shadow (bit 0), ambient-only (bit 1), two extra parts, byte 41 == 255, no animation
    setup(0, 3, 1);
    object[41] = 255;
    object[156] = 0;
    tags = {"ab", "cd"};
    msg_draw__10AutoObject(object);
    expect(is({"matrix:7:-2", "shadow-scale:2", "ambient:128,128,128,0", "ambient-only",
               "draw:1111:2222:1:7:m:noanim:1122334455667788:0",
               "tag:multiwak:0", "format:ab.vif", "format:ab.tex", "lump:lumpname:ab.vif", "lump:lumpname:ab.tex",
               "draw:101:102:1:7:m:noanim:ffffffffffffffff:0",
               "tag:multiwak:1", "format:cd.vif", "format:cd.tex", "lump:lumpname:cd.vif", "lump:lumpname:cd.tex",
               "draw:103:104:1:7:m:noanim:ffffffffffffffff:0",
               "tag:multiwak:2", "model-reset", "light-reset"}),
           "case B sequence");
    expect(modelClippingOn == 1, "byte 41 == 255 sets modelClippingOn");
    expect(draw_records.size() == 3u && draw_records[0] == nullptr && draw_records[1] == nullptr && draw_records[2] == nullptr,
           "no shadow records when bit 0 is set");
    std::memcpy(&flags_after, object + 12, 4);
    expect(flags_after == 0u, "flag word unchanged apart from bit 8");

    // C: shadows per part, parts present, no light flags
    setup(0, 0, -1);
    tags = {"p"};
    msg_draw__10AutoObject(object);
    expect(is({"matrix:7:-2", "shadow-scale:2", "shadow:1111:1:m:noanim:0",
               "draw:1111:2222:1:7:m:noanim:1122334455667788:0",
               "tag:multiwak:0", "format:p.vif", "format:p.tex", "lump:lumpname:p.vif", "lump:lumpname:p.tex",
               "shadow:101:1:m:noanim:0", "draw:101:102:1:7:m:noanim:ffffffffffffffff:0",
               "tag:multiwak:1", "model-reset"}),
           "case C sequence (a negative byte at +108 counts as nonzero)");
    expect(draw_records.size() == 2u && draw_records[0] == &shadow_token[0] && draw_records[1] == &shadow_token[1],
           "each part gets its own shadow record");
    expect(modelClippingOn == 0, "clipping byte untouched");

    // D: only other flag bits -> no light reset, flag word kept
    setup(0xfffffeffu, 0, 0);
    msg_draw__10AutoObject(object);
    expect(is({"matrix:7:-2", "shadow-scale:2", "shadow:1111:1:m:noanim:0", "draw:1111:2222:1:7:m:noanim:1122334455667788:0",
               "model-reset"}),
           "case D sequence");
    std::memcpy(&flags_after, object + 12, 4);
    expect(flags_after == 0xfffffeffu, "flag word kept");
    return failures ? 1 : 0;
}
