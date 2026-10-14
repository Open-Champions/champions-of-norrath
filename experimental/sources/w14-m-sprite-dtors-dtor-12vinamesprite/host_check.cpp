// Host contract for _$_12VINameSprite; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }
unsigned char d_vt_12VINameSprite[0x100];
void free_object(void* pointer) { trace += "delete@" + std::to_string(off(pointer)) + ";"; }
void d_Clear__12VINameSprite(void* self) { trace += "Clear__12VINameSprite@" + std::to_string(off(self)) + ";"; }
void d___8VISprite(void* self, int flags) { trace += "_$_8VISprite@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }

int main() {
    int failures = 0;
    for (unsigned f : {0u, 1u, 2u, 3u, 0x80000001u, 0xfffffffeu}) {
        const int flags = static_cast<int>(f);
        std::memset(object, 0x3c, sizeof object);
        trace.clear();
        destructor_12VINameSprite(object, flags);
        std::string want;
        want += "Clear__12VINameSprite@0;";
        want += "_$_8VISprite@0,0;";
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", flags, trace.c_str(), want.c_str()); ++failures; }
        if (*reinterpret_cast<unsigned*>(object + 60) != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(d_vt_12VINameSprite))) { std::fprintf(stderr, "fail: table\n"); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-dtor-12vinamesprite ok");
    return failures ? 1 : 0;
}
