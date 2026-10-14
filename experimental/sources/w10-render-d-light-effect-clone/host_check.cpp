// Host check for LightEffect::clone: allocation failure, field copies, skipped bytes, light re-registration.
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" void* clone__11LightEffect(void* self);
extern "C" { unsigned char game_object_vtable[8] __asm__("_vt$10GameObject"); unsigned char light_effect_vtable[8] __asm__("_vt$11LightEffect"); }
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static unsigned char arena[200];
static bool fail_alloc;
static int alloc_size;
static unsigned char* heads[4][2];
static int head_count, light_adds;
static unsigned light_first, light_second;
static unsigned char* light_arg;
static unsigned vt_at_heads;
extern "C" void* blockAlloc__Fi(int size) {
    alloc_size = size;
    return fail_alloc ? 0 : arena;
}
extern "C" void __8ListHeadRC8ListHead(void* destination, const void* source) {
    heads[head_count][0] = (unsigned char*)destination;
    heads[head_count][1] = (unsigned char*)source;
    if (head_count == 0) memcpy(&vt_at_heads, arena + 72, 4);
    ++head_count;
}
extern "C" void lightAdd__FP5Light(void* light) {
    ++light_adds; light_arg = (unsigned char*)light;
    memcpy(&light_first, arena + 76, 4); memcpy(&light_second, arena + 80, 4);
}
int main() {
    alignas(16) unsigned char source[200];
    for (int i = 0; i < 200; ++i) source[i] = (unsigned char)(i * 7 + 3);
    fail_alloc = true;
    CHECK(clone__11LightEffect(source) == 0 && alloc_size == 188 && head_count == 0);
    fail_alloc = false;
    memset(arena, 0xee, sizeof arena);
    memset(source + 76, 0, 8);                       // links clear: no registration
    void* r = clone__11LightEffect(source);
    CHECK(r == arena && head_count == 4 && light_adds == 0);
    CHECK(heads[0][0] == arena && heads[0][1] == source && heads[1][0] == arena + 4 && heads[2][0] == arena + 8);
    CHECK(heads[3][0] == arena + 136 && heads[3][1] == source + 136);
    unsigned wanted_vt = (unsigned)(uintptr_t)light_effect_vtable, first_vt = (unsigned)(uintptr_t)game_object_vtable;
    CHECK(vt_at_heads == first_vt);
    unsigned vt; memcpy(&vt, arena + 72, 4);
    CHECK(vt == wanted_vt);
    CHECK(memcmp(arena + 12, source + 12, 4) == 0 && memcmp(arena + 16, source + 16, 24) == 0);
    CHECK(arena[40] == source[40] && arena[41] == source[41] && arena[42] == 0xee && arena[43] == 0xee);
    CHECK(memcmp(arena + 44, source + 44, 12) == 0 && memcmp(arena + 56, source + 56, 2) == 0);
    CHECK(arena[58] == 0xee && arena[59] == 0xee && memcmp(arena + 60, source + 60, 12) == 0);
    CHECK(memcmp(arena + 76, source + 76, 60) == 0);
    CHECK(memcmp(arena + 140, source + 140, 48) == 0);
    for (int which = 0; which < 2; ++which) {
        head_count = 0;
        memset(source + 76, 0, 8);
        source[which ? 80 : 76] = 9;
        memset(arena, 0xee, sizeof arena);
        clone__11LightEffect(source);
        CHECK(light_adds == which + 1 && light_arg == arena + 76 && light_first == 0 && light_second == 0);
        CHECK(memcmp(arena + 84, source + 84, 52) == 0);
    }
    if (fails) return 1;
    puts("LightEffect clone ok");
    return 0;
}
