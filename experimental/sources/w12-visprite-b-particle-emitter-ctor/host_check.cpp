// Host check only; not a build input. Fills the object with 0x5a, runs the constructor with
// recording stubs and compares the call trace (names and this-relative offsets) and every byte
// against an expectation built from the observed zero ranges. The stubs write a marker into the
// bytes they own, so a store that lands on a callee's bytes after the call would show as a
// difference (Identity__10VIMatrix33 and Identity__10VIMatrix44 stamp the bytes they were
// given). The same constructor was also compared with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x1000];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void __7VILNode(void* s) { trace += "node:" + off(s) + ";"; }
void __10VIMatrix33(void* s) { trace += "m33:" + off(s) + ";"; }
void __7VILList(void* s) { trace += "list:" + off(s) + ";"; }
void __20VIParticleAttributes(void* s) { trace += "attr:" + off(s) + ";"; }
void Identity__10VIMatrix33(void* s) { trace += "id33:" + off(s) + ";"; std::memset(s, 0x33, 36); }
void Identity__10VIMatrix44(void* s) { trace += "id44:" + off(s) + ";"; std::memset(s, 0x44, 64); }
}

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[sizeof object];
    std::memset(expect, 0x5a, sizeof expect);
    auto zero = [&](int at, int n) { std::memset(expect + at, 0, n); };
    zero(16, 24);            // two vectors
    zero(76, 12);            // float at +76, words at +80, +84
    zero(172, 4);
    std::memset(expect + 40, 0x33, 36);
    zero(16, 24);
    std::memset(expect + 96, 0x44, 64);
    zero(916, 60);
    zero(980, 8);
    // 40..76 is the 3x3 matrix stamped by the Identity stub; +76 is zeroed afterwards by the
    // constructor only before that call, and the stub covers 36 bytes (40..75).
    const std::string want = "node:0;m33:40;list:160;list:176;attr:188;id33:40;id44:96;";
    fn_17VIParticleEmitter(object);
    bool ok = trace == want && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) {
        std::printf("trace %s\n", trace.c_str());
        for (unsigned i = 0; i < sizeof object; ++i) if (object[i] != expect[i]) { std::printf("byte %u: %02x want %02x\n", i, object[i], expect[i]); break; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
