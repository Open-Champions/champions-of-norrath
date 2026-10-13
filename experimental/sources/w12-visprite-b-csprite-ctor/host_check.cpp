// Host check only; not a build input. Fills the object with 0x5a, runs the constructor with
// recording stubs and compares the call trace and every byte against an expectation built from
// the observed ranges (written as byte fills and overlays, not by repeating the constructor's
// loops). The same constructor was also compared with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$9VICSprite[8] __asm__("_vt$9VICSprite");
static unsigned char object[0x1400];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void __9VIHSprite(void* s) { trace += "hsprite:" + off(s) + ";"; }
void __t7VIArray1Z20VICSpritePartEmitter(void* s) { trace += "emitters:" + off(s) + ";"; }
}

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char e[sizeof object];
    std::memset(e, 0x5a, sizeof e);
    auto word = [&](int at, unsigned v) { std::memcpy(e + at, &v, 4); };
    auto flt = [&](int at, float v) { std::memcpy(e + at, &v, 4); };
    const unsigned N = 0xffffffffu;
    word(60, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VICSprite)));
    // 66 records of 52 bytes.
    unsigned char rec52[52];
    { unsigned w[13] = {N, N, 0x3f800000u, 0, 0, 0, N, N, 0x3f800000u, 0x3f800000u, 0, 0, 0}; std::memcpy(rec52, w, 52); }
    for (int i = 0; i < 66; ++i) std::memcpy(e + 424 + i * 52, rec52, 52);
    // 19 records of 24 bytes.
    unsigned char rec24[24];
    { unsigned w[6] = {0, N, N, N, 0, 0}; std::memcpy(rec24, w, 24); }
    for (int i = 0; i < 19; ++i) std::memcpy(e + 3896 + i * 24, rec24, 24);
    std::memset(e + 3856, 0xff, 40);
    word(4352, 0); word(4356, 2); word(4360, 1);
    std::memset(e + 4364, 0xff, 12 * 3);
    word(4368, 11); word(4380, 11); word(4392, 11);
    std::memset(e + 4400, 0xff, 24);
    std::memset(e + 4424, 0, 32);
    word(4456, 0);
    for (int g : {4460, 4476, 4508}) { word(g, 0); word(g + 4, N); word(g + 8, 0); word(g + 12, N); }
    word(4492, 0); word(4496, N); word(4500, 0); word(4504, N);
    std::memset(e + 4528, 0, 16);
    word(4544, 1);
    flt(4548, 0.25f); flt(4552, 0.25f); flt(4556, 0.25f); flt(4560, 1.0f);
    std::memset(e + 4568, 0, 12);
    word(4580, N); word(4584, N); word(4588, 0);
    word(4592, N); word(4596, 0); word(4600, N);
    std::memset(e + 4608, 0, 8);
    word(52, 5);
    word(400, 1); word(404, 0); word(408, 9); word(412, 0); word(416, N); word(420, N);
    fn_9VICSprite(object);
    const std::string want = "hsprite:0;emitters:4616;";
    bool ok = trace == want && std::memcmp(object, e, sizeof object) == 0;
    if (!ok) {
        std::printf("trace %s\n", trace.c_str());
        for (unsigned i = 0; i < sizeof object; ++i) if (object[i] != e[i]) { std::printf("byte %u: %02x want %02x\n", i, object[i], e[i]); break; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
