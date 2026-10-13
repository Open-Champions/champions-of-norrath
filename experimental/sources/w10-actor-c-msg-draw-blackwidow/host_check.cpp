// Build with -DNORRATH_W10AC_HOST (the source then maps 32-bit pointer words to host addresses
// as offsets from norrath_w10ac_host_base).
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void msg_draw__10BlackWidow(void*);
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
int main() {
    static unsigned char memory[0x1000];
    norrath_w10ac_host_base = memory;
    unsigned char* object = memory + 0x10;
    unsigned char* shared = memory + 0x200;
    put(object + 36, 0x200);
    put(shared + 12, 0x300);
    put(shared + 16, 0x340);
    put(shared + 20, 0x380);
    msg_draw__10BlackWidow(object);
    expect(calls == 1, "one call");
    expect(seen_self == object, "object first");
    expect(seen_model == memory + 0x300, "word at +12 second");
    expect(seen_texture == memory + 0x380, "word at +20 third");
    expect(seen_extra == nullptr, "fourth is null");
    return failures ? 1 : 0;
}
