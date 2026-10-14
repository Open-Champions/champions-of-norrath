#include "init.cpp"

static void* seen[8];
static int seen_count[8];
static int count;
static int fail_at = -1;
static int reset_calls;
static void* reset_arg;
static int step(void* a, int n) {
    seen[count] = a; seen_count[count] = n;
    return count++ == fail_at ? -3 : 0;
}
extern "C" int Init__t7VIArray1Z11VIClipEntryi(void* a, int n) { return step(a, n); }
extern "C" int Init__t7VIArray1Z7VIVect3i(void* a, int n) { return step(a, n); }
extern "C" int Init__t7VIArray1Z7VIPlanei(void* a, int n) { return step(a, n); }
extern "C" int Reset__11VIClipStack(void* o) { ++reset_calls; reset_arg = o; return 0; }

int main() {
    unsigned char object[64];
    static const int offsets[5] = {4, 16, 28, 36, 44};
    static const int sizes[5] = {500, 2000, 2000, 32, 32};
    if (Init__11VIClipStack(object) != 0 || count != 5) return 1;
    for (int i = 0; i < 5; ++i) {
        if (seen[i] != object + offsets[i]) return 2 + i;
        if (seen_count[i] != sizes[i]) return 8 + i;
    }
    if (reset_calls != 1 || reset_arg != object) return 14;
    for (int f = 0; f < 5; ++f) {
        count = 0; reset_calls = 0; fail_at = f;
        if (Init__11VIClipStack(object) != -1) return 20 + f;
        if (count != f + 1 || reset_calls != 0) return 30 + f;
    }
    return 0;
}
