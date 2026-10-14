// Host contract for __12VIPointLight. Not an EE plan input. Build with -DNORRATH_HOST_ARENA.
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
unsigned char vt_stub[16] __asm__("_vt$12VIPointLight");
static int baseCalls; static unsigned char* baseSelf;
void __8VISprite(void* self) {
    ++baseCalls; baseSelf = static_cast<unsigned char*>(self);
    // the base constructor fills the object with a recognisable pattern
    std::memset(self, 0x5a, 80);
}
void __12VIPointLight(void* self);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* obj = arena_at(0x400);
    std::memset(obj, 0, 96);
    __12VIPointLight(obj);
    expect(baseCalls == 1 && baseSelf == obj, "base constructor called once on the object");
    expect(hw(obj + 60) == norrath_host_address(vt_stub), "vtable word");
    expect(hw(obj + 52) == 8, "type field");
    for (int off : {64, 68, 72, 76}) expect(hw(obj + off) == 0, "zero float");
    for (int off : {0, 4, 40, 48, 56}) expect(hw(obj + off) == 0x5a5a5a5au, "base fields untouched");
    for (int off = 80; off < 96; ++off) expect(obj[off] == 0, "nothing past 80 bytes");
    if (failures) return 1;
    std::printf("__12VIPointLight contract passed\n");
    return 0;
}
