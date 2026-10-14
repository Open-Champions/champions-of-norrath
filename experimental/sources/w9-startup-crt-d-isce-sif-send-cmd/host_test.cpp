// Host comparison only. Not an EE build input.
#include <cstdio>
static int seen[7];
extern "C" int _sceSifSendCmd(int a, int mode, int b, int c, int d, int e, int f) {
    seen[0] = a; seen[1] = mode; seen[2] = b; seen[3] = c; seen[4] = d; seen[5] = e; seen[6] = f;
    return a * 3 + f;
}
#include "isce_sif_send_cmd.cpp"
int main() {
    int r = isceSifSendCmd(10, 20, 30, 40, 50, 60);
    const int want[7] = {10, 1, 20, 30, 40, 50, 60};
    for (int i = 0; i < 7; ++i) if (seen[i] != want[i]) { std::printf("arg %d = %d\n", i, seen[i]); return 1; }
    if (r != 90) { std::printf("result must pass through\n"); return 2; }
    std::printf("isceSifSendCmd cases passed\n");
    return 0;
}
