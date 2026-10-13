// Host check only; not a build input. Fills an object with a pattern, runs the routine with
// recording stubs for the callees, then compares the call trace (names, this-relative offsets,
// extra arguments) and every byte of the object against an expectation built from the observed
// stores. For destructors flags 0 to 3 and 0xffffffff are tried.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$16VIParticleSprite[8] __asm__("_vt$16VIParticleSprite");
static unsigned char object[0x2000];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void __8VISprite(void* self) { trace += "__8VISprite:" + off(self) + ";"; }
void __17VIParticleEmitter(void* self) { trace += "__17VIParticleEmitter:" + off(self) + ";"; }
}

static bool run(int flags) {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[sizeof object];
    std::memset(expect, 0x5a, sizeof expect);
    std::string want;
    want += "__8VISprite:0;";
    { unsigned v = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16VIParticleSprite)); std::memcpy(expect + 60, &v, 4); }
    want += "__17VIParticleEmitter:80;";
    { unsigned v = 10u; std::memcpy(expect + 52, &v, 4); }
    { unsigned v = 4294967295u; std::memcpy(expect + 64, &v, 4); }
    { unsigned v = 0u; std::memcpy(expect + 1072, &v, 4); }
    (void)flags;
    trace.clear();
    fn_16VIParticleSprite(object);
    bool ok = trace == want && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    return ok;
}

int main() {
    bool ok = true;
    for (int flags : {0, 1, 2, 3, -1}) ok = run(flags) && ok;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
