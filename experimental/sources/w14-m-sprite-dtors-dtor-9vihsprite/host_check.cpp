// Host contract for _$_9VIHSprite; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }
unsigned char d_vt_9VIHSprite[0x100];
void free_object(void* pointer) { trace += "delete@" + std::to_string(off(pointer)) + ";"; }
void d_Clear__9VIHSprite(void* self) { trace += "Clear__9VIHSprite@" + std::to_string(off(self)) + ";"; }
void d___10VIMatrix44(void* self, int flags) { trace += "_$_10VIMatrix44@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t6VIPool1Z17VIHSpritePlayNode(void* self, int flags) { trace += "_$_t6VIPool1Z17VIHSpritePlayNode@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t6VIPool1Z13VIHSpritePlay(void* self, int flags) { trace += "_$_t6VIPool1Z13VIHSpritePlay@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t6VIPool1Z19VIHSpriteAttachment(void* self, int flags) { trace += "_$_t6VIPool1Z19VIHSpriteAttachment@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t7VIArray1Z16VIHSpriteTrigger(void* self, int flags) { trace += "_$_t7VIArray1Z16VIHSpriteTrigger@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t8VIVector1Z10VIMatrix44(void* self, int flags) { trace += "_$_t8VIVector1Z10VIMatrix44@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___t8VIVector1Z13VIHSpriteNode(void* self, int flags) { trace += "_$_t8VIVector1Z13VIHSpriteNode@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void d___8VISprite(void* self, int flags) { trace += "_$_8VISprite@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }

int main() {
    int failures = 0;
    for (unsigned f : {0u, 1u, 2u, 3u, 0x80000001u, 0xfffffffeu}) {
        const int flags = static_cast<int>(f);
        std::memset(object, 0x3c, sizeof object);
        trace.clear();
        destructor_9VIHSprite(object, flags);
        std::string want;
        want += "Clear__9VIHSprite@0;";
        want += "_$_10VIMatrix44@320,2;";
        want += "_$_t6VIPool1Z17VIHSpritePlayNode@144,2;";
        want += "_$_t6VIPool1Z13VIHSpritePlay@120,2;";
        want += "_$_t6VIPool1Z19VIHSpriteAttachment@96,2;";
        want += "_$_t7VIArray1Z16VIHSpriteTrigger@88,2;";
        want += "_$_t8VIVector1Z10VIMatrix44@76,2;";
        want += "_$_t8VIVector1Z13VIHSpriteNode@64,2;";
        want += "_$_8VISprite@0,0;";
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", flags, trace.c_str(), want.c_str()); ++failures; }
        if (*reinterpret_cast<unsigned*>(object + 60) != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(d_vt_9VIHSprite))) { std::fprintf(stderr, "fail: table\n"); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-dtor-9vihsprite ok");
    return failures ? 1 : 0;
}
