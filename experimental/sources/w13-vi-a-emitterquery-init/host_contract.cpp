#include "init.cpp"
#include <string.h>

static int map_ret; static void* map_arg; static int calls;
extern "C" int Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(void* m) { map_arg = m; ++calls; return map_ret; }

int main() {
    alignas(4) unsigned char q[64];
    int expect_ret[4] = {0, 5, -1, -9}, want[4] = {0, 0, -1, -1};
    for (int i = 0; i < 4; ++i) {
        memset(q, 0x77, sizeof q); map_ret = expect_ret[i];
        if (Init__14VIEmitterQueryi12VISpriteType(q, 33, 4) != want[i]) return 1 + i;
        if (map_arg != q + 16 || calls != i + 1) return 10;
        if (*(int*)q != 4 || *(int*)(q + 4) != 33 || *(int*)(q + 8) != -1 || *(int*)(q + 12) != -1) return 11;
        for (int k = 16; k < 64; ++k) if (q[k] != 0x77) return 12;
    }
    return 0;
}
