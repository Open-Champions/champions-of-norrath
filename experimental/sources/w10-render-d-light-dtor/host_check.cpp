// Host check for the Light destructor: unlinking in the middle, at either end, alone, and the delete flag.
#include <cstdio>
struct LightLinks { LightLinks* first; LightLinks* second; };
extern "C" void light_destructor(LightLinks* self, int flags) __asm__("_$_5Light");
static int fails;
static void* deleted;
static int deletes;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
extern "C" void builtin_delete(void* object) __asm__("__builtin_delete");
extern "C" void builtin_delete(void* object) { deleted = object; ++deletes; }
int main() {
    LightLinks a{}, b{}, c{};
    // chain: a.first = b, b.second = a, b.first = c, c.second = b
    a.first = &b; b.second = &a; b.first = &c; c.second = &b;
    light_destructor(&b, 0);
    CHECK(a.first == &c && c.second == &a && b.first == 0 && b.second == 0 && deletes == 0);
    light_destructor(&a, 2);                     // head: only the first link is used
    CHECK(c.second == 0 && a.first == 0 && a.second == 0 && deletes == 0);
    a.first = &c; c.second = &a;
    light_destructor(&c, 1);                     // tail with the delete flag
    CHECK(a.first == 0 && c.second == 0 && deletes == 1 && deleted == &c);
    LightLinks lone{};
    light_destructor(&lone, 3);
    CHECK(deletes == 2 && deleted == &lone);
    if (fails) return 1;
    puts("Light destructor ok");
    return 0;
}
