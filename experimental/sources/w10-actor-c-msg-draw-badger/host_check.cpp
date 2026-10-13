// Build with -DNORRATH_W10AC_HOST (the source then maps 32-bit pointer words to host addresses
// as offsets from norrath_w10ac_host_base).
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void msg_draw__6Badger(void*);
unsigned char* norrath_w10ac_host_base;
static int failures = 0;
static int calls = 0;
static void *seen_self, *seen_model, *seen_texture, *seen_extra;
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* c, void* m, void* t, void* x) {
    ++calls;
    seen_self = c; seen_model = m; seen_texture = t; seen_extra = x;
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void put(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float f) { std::memcpy(p, &f, 4); }
static unsigned char memory[0x1000];
static void run(float low, float high, bool expect_alt, const char* message) {
    unsigned char* object = memory + 0x10;
    unsigned char* shared = memory + 0x400;
    std::memset(memory, 0, sizeof memory);
    put(object + 36, 0x400);
    put(shared + 12, 0x500);
    put(shared + 20, 0x540);
    put(shared + 28, 0x580);
    putf(object + 560, low);
    putf(object + 564, high);
    calls = 0;
    msg_draw__6Badger(object);
    bool ok = calls == 1 && seen_self == object && seen_model == memory + 0x500 && seen_extra == nullptr &&
              seen_texture == memory + (expect_alt ? 0x580 : 0x540);
    expect(ok, message);
}
int main() {
    norrath_w10ac_host_base = memory;
    run(1.0f, 8.0f, true, "below a quarter uses +28");
    run(2.0f, 8.0f, false, "exactly a quarter uses +20");
    run(2.5f, 8.0f, false, "above a quarter uses +20");
    run(0.0f, 0.0f, false, "equal zeros use +20");
    run(-1.0f, 0.0f, true, "negative below zero uses +28");
    run(NAN, 8.0f, false, "NaN compares false");
    run(1.0f, NAN, false, "NaN limit compares false");
    run(0.99f, 4.0f, true, "0.99 < 1.0");
    run(1.01f, 4.0f, false, "1.01 > 1.0");
    return failures ? 1 : 0;
}
