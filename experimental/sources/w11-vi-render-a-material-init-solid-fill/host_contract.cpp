#include "init_solid_fill.cpp"
#include <string.h>

static void* matrix_arg; static int calls;
extern "C" void Identity__10VIMatrix33(void* m) { matrix_arg = m; ++calls; }

static void put(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }

int main() {
    unsigned char object[160], expect[160];
    memset(object, 0x3c, sizeof object);
    memcpy(expect, object, sizeof object);
    int r = InitSolidFill__10VIMaterial(object);
    if (r != 0 || calls != 1 || matrix_arg != object + 64) return 1;
    put(expect, 8, 1); put(expect, 12, 19); put(expect, 16, 0);
    put(expect, 52, 1); put(expect, 32, 1); put(expect, 48, 0xffffffffu);
    put(expect, 56, 0xffffffffu); put(expect, 60, 0);
    put(expect, 100, 0x3f4cccccu);
    put(expect, 104, 0); put(expect, 108, 0); put(expect, 112, 0); put(expect, 116, 0);
    put(expect, 120, 0xffffffffu);
    if (memcmp(object, expect, sizeof object) != 0) return 2;
    return 0;
}
