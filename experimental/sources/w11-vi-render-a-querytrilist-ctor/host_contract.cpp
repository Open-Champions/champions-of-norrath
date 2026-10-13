#include "ctor.cpp"
#include <string.h>

static void* calls[4];
static int n;
extern "C" void __t7VIArray1Z7VIVect3(void* a) { calls[n++] = a; }

int main() {
    unsigned char object[200];
    unsigned char expect[200];
    memset(object, 0x6b, sizeof object);
    memcpy(expect, object, sizeof expect);
    __14VIQueryTriList(object);
    if (n != 2 || calls[0] != object + 12 || calls[1] != object + 20) return 1;
    memset(expect + 0, 0, 12);
    memset(expect + 28, 0, 12);
    memset(expect + 44, 0, 96);
    if (memcmp(object, expect, sizeof object) != 0) return 2;
    return 0;
}
