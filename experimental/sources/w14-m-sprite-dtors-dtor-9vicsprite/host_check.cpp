// Host contract for _$_9VICSprite; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }
unsigned char d_vt_9VICSprite[0x100];
void free_object(void* pointer) { trace += "delete@" + std::to_string(off(pointer)) + ";"; }
void d_Clear__9VICSprite(void* self) { trace += "Clear__9VICSprite@" + std::to_string(off(self)) + ";"; }
void d___t7VIArray1Z20VICSpritePartEmitter(void* self, int flags) { trace += "_$_t7VIArray1Z20VICSpritePartEmitter@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___9VIHSprite(void* self, int flags) { trace += "_$_9VIHSprite@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }

int main() {
    int failures = 0;
    for (unsigned f : {0u, 1u, 2u, 3u, 0x80000001u, 0xfffffffeu}) {
        const int flags = static_cast<int>(f);
        std::memset(object, 0x3c, sizeof object);
        trace.clear();
        destructor_9VICSprite(object, flags);
        std::string want;
        want += "Clear__9VICSprite@0;";
        want += "_$_t7VIArray1Z20VICSpritePartEmitter@4616,2;";
        want += "_$_9VIHSprite@0,0;";
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", flags, trace.c_str(), want.c_str()); ++failures; }
        if (*reinterpret_cast<unsigned*>(object + 60) != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(d_vt_9VICSprite))) { std::fprintf(stderr, "fail: table\n"); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-dtor-9vicsprite ok");
    return failures ? 1 : 0;
}
