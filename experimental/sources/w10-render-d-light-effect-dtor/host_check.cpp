// Host check for the LightEffect destructor: call order, arguments, vtable word and the flag-bit-0 release.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
extern "C" void light_effect_destructor(void* self, int flags) __asm__("_$_11LightEffect");
extern "C" { unsigned char light_effect_vtable[16] __asm__("_vt$11LightEffect"); }
static int fails;
static int step;
static unsigned char* object;
static int frees;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
extern "C" void listDestroy__FP8ListHead(void* list) {
    unsigned stored; memcpy(&stored, object + 72, 4);
    CHECK(step == 0 && list == object + 136 && stored == (unsigned)(uintptr_t)light_effect_vtable);
    step = 1;
}
extern "C" void light_destructor(void* light, int flags) __asm__("_$_5Light");
extern "C" void light_destructor(void* light, int flags) { CHECK(step == 1 && light == object + 76 && flags == 2); step = 2; }
extern "C" void game_object_destructor(void* o, int flags) __asm__("_$_10GameObject");
extern "C" void game_object_destructor(void* o, int flags) { CHECK(step == 2 && o == object && flags == 0); step = 3; }
extern "C" void blockFree__FPv(void* block) { CHECK(step == 3 && block == object); ++frees; }
int main() {
    alignas(16) static unsigned char storage[400];
    object = storage;
    for (int flags : {0, 2, 1, 5, 4, 3}) {
        step = 0;
        int before = frees;
        light_effect_destructor(object, flags);
        CHECK(step == 3);
        CHECK(frees - before == (flags & 1));
    }
    if (fails) return 1;
    puts("LightEffect destructor ok");
    return 0;
}
