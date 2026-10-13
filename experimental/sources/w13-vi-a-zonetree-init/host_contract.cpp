#include "init.cpp"

static int node_ret, leaf_ret, node_calls, leaf_calls, node_cnt, leaf_cnt; static void *node_p, *leaf_p;
extern "C" int Init__t7VIArray1Z10VIZoneNodei(void* a, int c) { ++node_calls; node_p = a; node_cnt = c; return node_ret; }
extern "C" int Init__t7VIArray1Z10VIZoneLeafi(void* a, int c) { ++leaf_calls; leaf_p = a; leaf_cnt = c; return leaf_ret; }

int main() {
    unsigned char t[32];
    node_ret = -2; leaf_ret = 0;
    if (Init__10VIZoneTreeii(t, 3, 9) != -1 || node_calls != 1 || leaf_calls != 0) return 1;
    if (node_p != t || node_cnt != 3) return 2;
    node_ret = 0; leaf_ret = -1;
    if (Init__10VIZoneTreeii(t, 3, 9) != -1 || leaf_calls != 1) return 3;
    if (leaf_p != t + 8 || leaf_cnt != 9) return 4;
    node_ret = 4; leaf_ret = 6;
    if (Init__10VIZoneTreeii(t, 5, 7) != 0 || node_cnt != 5 || leaf_cnt != 7) return 5;
    return 0;
}
