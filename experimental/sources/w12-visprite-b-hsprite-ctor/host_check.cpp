// Host check only; not a build input. Fills the object with 0x5a, runs the constructor with
// recording stubs and compares the call trace and every byte against an expectation built from the
// observed ranges. The same constructor was also compared with the original in a private
// interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$9VIHSprite[8] __asm__("_vt$9VIHSprite");
static unsigned char object[0x1000];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void __8VISprite(void* s) { trace += "sprite:" + off(s) + ";"; }
void __t8VIVector1Z13VIHSpriteNode(void* s) { trace += "nodes:" + off(s) + ";"; }
void __t8VIVector1Z10VIMatrix44(void* s) { trace += "mats:" + off(s) + ";"; }
void __t7VIArray1Z16VIHSpriteTrigger(void* s) { trace += "trig:" + off(s) + ";"; }
void __t6VIPool1Z19VIHSpriteAttachment(void* s) { trace += "attach:" + off(s) + ";"; }
void __t6VIPool1Z13VIHSpritePlay(void* s) { trace += "play:" + off(s) + ";"; }
void __t6VIPool1Z17VIHSpritePlayNode(void* s) { trace += "playnode:" + off(s) + ";"; }
}

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[sizeof object];
    std::memset(expect, 0x5a, sizeof expect);
    auto word = [&](int at, unsigned v) { std::memcpy(expect + at, &v, 4); };
    auto flt = [&](int at, float v) { std::memcpy(expect + at, &v, 4); };
    word(60, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIHSprite)));
    word(168, 0);
    word(52, 4);
    flt(208, 1.0f);
    for (int at = 184; at < 208; at += 4) word(at, 0xffffffffu);
    for (int at = 212; at < 236; at += 4) flt(at, 0.0f);
    const float rec[8] = {0, 0, 0, 1, 1, 0, 0, 0};
    std::memcpy(expect + 236, rec, 32);
    std::memcpy(expect + 268, rec, 32);
    word(300, 0);
    std::memset(expect + 304, 0, 8);
    std::memset(expect + 320, 0, 64);
    word(384, 0xffffffffu);
    word(388, 0xffffffffu);
    const std::string want = "sprite:0;nodes:64;mats:76;trig:88;attach:96;play:120;playnode:144;";
    fn_9VIHSprite(object);
    bool ok = trace == want && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) {
        std::printf("trace %s\n", trace.c_str());
        for (unsigned i = 0; i < sizeof object; ++i) if (object[i] != expect[i]) { std::printf("byte %u: %02x want %02x\n", i, object[i], expect[i]); break; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
