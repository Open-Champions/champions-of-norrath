// Host check only; not a build input.
#include "dtor.cpp"
#include <cstdio>
#include <string>
unsigned char vt_VISprite[0x60] __asm__("_vt$8VISprite");
static unsigned char object[0x80];
static std::string trace;
extern "C" void __builtin_delete(void* p) { trace += p == object ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    const unsigned table = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VISprite));
    for (int flags : {0, 1, 2, 3, 4, 5, -1, -2}) {
        trace.clear();
        for (auto& b : object) b = 0x5a;
        _$_8VISprite(object, flags);
        if (trace != ((flags & 1) ? "delete;" : "")) ok = false;
        if (*reinterpret_cast<unsigned*>(object + 60) != table) ok = false;
        for (int i = 0; i < 0x80; ++i) if ((i < 60 || i >= 64) && object[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
