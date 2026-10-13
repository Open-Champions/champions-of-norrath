// Host check only; not a build input. Fills the object with a pattern, runs the constructor with a
// recording stub and compares the call trace and every byte against an expectation built from the
// observed ranges, so the untouched words of each record show as unchanged. The same constructor
// was also compared with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$13VIGroupSprite[8] __asm__("_vt$13VIGroupSprite");
static unsigned char object[0x400];
static std::string trace;
extern "C" void __8VISprite(void* s) { trace += "sprite:" + std::to_string(static_cast<unsigned char*>(s) - object) + ";"; }

int main() {
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 7 + 3);
    unsigned char e[sizeof object];
    std::memcpy(e, object, sizeof e);
    const unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13VIGroupSprite));
    std::memcpy(e + 60, &vt, 4);
    for (int i = 0; i < 5; ++i) {
        std::memset(e + 68 + 32 * i + 4, 0, 12);
        std::memset(e + 68 + 32 * i + 20, 0, 12);
    }
    const unsigned eleven = 11;
    std::memcpy(e + 52, &eleven, 4);
    fn_13VIGroupSprite(object);
    bool ok = trace == "sprite:0;" && std::memcmp(object, e, sizeof object) == 0;
    if (!ok) std::printf("trace %s\n", trace.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
