#include "ctor.cpp"
#include <string.h>

static void* lists[8]; static int n;
extern "C" void __7VILList(void* l) { lists[n++] = l; }

int main() {
    static unsigned char object[4600], expect[4600];
    memset(object, 0x9d, sizeof object);
    memcpy(expect, object, sizeof expect);
    __12VIAtmosphere(object);
    if (n != 4 || lists[0] != object + 20 || lists[1] != object + 36 || lists[2] != object + 4448 || lists[3] != object + 4464) return 1;
    static const int ranges[][2] = {{60, 12}, {80, 24}, {104, 24}, {168, 768}, {1280, 64}, {1344, 64}, {1408, 64},
                                    {1568, 2048}, {3888, 12}, {3900, 12}, {3936, 512}, {1228, 4}, {4476, 4}, {12, 4}, {48, 4}};
    for (unsigned i = 0; i < sizeof ranges / sizeof ranges[0]; ++i) memset(expect + ranges[i][0], 0, ranges[i][1]);
    if (memcmp(object, expect, sizeof object) != 0) {
        for (int i = 0; i < 4600; ++i) if (object[i] != expect[i]) return 100 + (i % 100);
    }
    return 0;
}
