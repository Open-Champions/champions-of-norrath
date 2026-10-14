// Host check for the AddParticle destructor: call order, arguments, vtable word and the flag-bit-0 release.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
extern "C" void add_particle_destructor(void* self, int flags) __asm__("_$_11AddParticle");
extern "C" { unsigned char add_particle_vtable[16] __asm__("_vt$11AddParticle"); }
static int fails;
static int step;
static unsigned char* object;
static int frees;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
extern "C" void light_destructor(void* light, int flags) __asm__("_$_5Light");
extern "C" void game_object_destructor(void* o, int flags) __asm__("_$_10GameObject");
extern "C" void blockFree__FPv(void* block);
extern "C" void light_destructor(void* light, int flags) {
    unsigned stored; memcpy(&stored, object + 72, 4);
    CHECK(step == 0 && light == object + 256 && flags == 2);
    CHECK(stored == (unsigned)(uintptr_t)add_particle_vtable);
    step = 1;
}
extern "C" void game_object_destructor(void* o, int flags) {
    CHECK(step == 1 && o == object && flags == 0);
    step = 2;
}
extern "C" void blockFree__FPv(void* block) {
    CHECK(step == 2 && block == object);
    ++frees;
}
int main() {
    alignas(16) static unsigned char storage[400];
    object = storage;
    for (int flags : {0, 2, 1, 5, 4, 3}) {
        step = 0;
        int before = frees;
        add_particle_destructor(object, flags);
        CHECK(step == 2);
        CHECK(frees - before == (flags & 1));
    }
    if (fails) return 1;
    puts("AddParticle destructor ok");
    return 0;
}
