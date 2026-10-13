#include "init.cpp"

static int results[2], calls;
static void* args[2]; static int counts[2];
extern "C" int Init__t7VIArray1Z7VIVect3i(void* a, int c) {
    if (calls < 2) { args[calls] = a; counts[calls] = c; }
    return results[calls++ < 2 ? calls - 1 : 0];
}

static int run(int r0, int r1, unsigned char* obj, int count) {
    results[0] = r0; results[1] = r1; calls = 0;
    return Init__9VIClipperi(obj, count);
}

int main() {
    unsigned char obj[32];
    if (run(0, 0, obj, 5) != 0 || calls != 2 || args[0] != obj || args[1] != obj + 8 || counts[0] != 5 || counts[1] != 5) return 1;
    if (run(3, 7, obj, 9) != 0 || calls != 2) return 2;
    if (run(-1, 0, obj, 5) != -1 || calls != 1) return 3;
    if (run(-4, -4, obj, 5) != -1 || calls != 1) return 4;
    if (run(0, -1, obj, 5) != -1 || calls != 2) return 5;
    if (run(2, -9, obj, 5) != -1 || calls != 2) return 6;
    return 0;
}
