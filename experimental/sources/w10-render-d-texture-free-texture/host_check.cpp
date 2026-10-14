// Host check for textureFreeTexture: clears the pointer and walks the record chain.
#include <cstdio>
#include <cstring>
struct UncompRecord { int owner; UncompRecord* next; };
char* uncompCache;
UncompRecord uncompMap[16];
extern "C" void textureFreeTexture__FP8_texture(unsigned char* texture);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main() {
    static char cache[16 * 4096];
    uncompCache = cache;
    unsigned char tex[32];
    memset(tex, 0, sizeof tex);
    for (int i = 0; i < 16; ++i) { uncompMap[i].owner = 100 + i; uncompMap[i].next = 0; }
    textureFreeTexture__FP8_texture(tex);  // no cache pointer: nothing changes
    for (int i = 0; i < 16; ++i) CHECK(uncompMap[i].owner == 100 + i);
    *reinterpret_cast<char**>(tex + 20) = cache + 3 * 4096 + 77;
    uncompMap[3].next = &uncompMap[4];
    uncompMap[4].next = &uncompMap[9];
    textureFreeTexture__FP8_texture(tex);
    CHECK(*reinterpret_cast<char**>(tex + 20) == 0);
    CHECK(uncompMap[3].owner == 0 && uncompMap[4].owner == 0 && uncompMap[9].owner == 0);
    CHECK(uncompMap[2].owner == 102 && uncompMap[5].owner == 105 && uncompMap[10].owner == 110);
    if (fails) return 1;
    puts("textureFreeTexture ok");
    return 0;
}
