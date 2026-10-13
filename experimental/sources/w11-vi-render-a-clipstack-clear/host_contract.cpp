#include "clear.cpp"

static void* seen[8];
static int count;
static int results[8];
static int fail_at = -1;
static int step() {
    int index = count - 1;
    return index == fail_at ? -5 : 0;
}
extern "C" int Clear__t7VIArray1Z11VIClipEntry(void* a) { seen[count++] = a; return step(); }
extern "C" int Clear__t7VIArray1Z7VIVect3(void* a) { seen[count++] = a; return step(); }
extern "C" int Clear__t7VIArray1Z7VIPlane(void* a) { seen[count++] = a; return step(); }

int main() {
    unsigned char object[64];
    static const int offsets[5] = {4, 16, 28, 36, 44};
    count = 0; fail_at = -1;
    if (Clear__11VIClipStack(object) != 0 || count != 5) return 1;
    for (int i = 0; i < 5; ++i) if (seen[i] != object + offsets[i]) return 2 + i;
    for (int f = 0; f < 5; ++f) {
        count = 0; fail_at = f;
        if (Clear__11VIClipStack(object) != -1) return 10 + f;
        if (count != f + 1) return 20 + f;
    }
    (void)results;
    return 0;
}
