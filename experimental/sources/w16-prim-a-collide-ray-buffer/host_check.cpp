#include "ray_buffer.cpp"
#include <initializer_list>
#include <string.h>

static int which, args_ok;
static void *ws, *wh;
static unsigned char* wb;
static int tag(int t, void* s, void* h, unsigned char* b) {
    which = t;
    args_ok = s == ws && h == wh && b == wb;
    return 1000 + t;
}
extern "C" int CollideBufferV__9VICollideR9VICollRayR12VICollBuffer(void* s, void* h, unsigned char* b) { return tag(1, s, h, b); }
extern "C" int CollideBufferPackV__9VICollideR9VICollRayR12VICollBuffer(void* s, void* h, unsigned char* b) { return tag(2, s, h, b); }
extern "C" int CollideBufferPackVGF__9VICollideR9VICollRayR12VICollBuffer(void* s, void* h, unsigned char* b) { return tag(3, s, h, b); }

int main() {
    for (int type : {-1, 0, 1, 2, 3, 4, 100}) {
        alignas(16) unsigned char buf[32], a[4], c[4];
        memset(buf, 0x20, sizeof buf);
        memcpy(buf + 8, &type, 4);
        ws = a; wh = c; wb = buf;
        which = 0;
        int r = Collide__9VICollideR9VICollRayR12VICollBuffer(a, c, buf);
        int expect_which = type == 0 ? 1 : type == 1 ? 2 : type == 3 ? 3 : 0;
        if (which != expect_which) return 1;
        if (r != (expect_which ? 1000 + expect_which : -1)) return 2;
        if (expect_which && !args_ok) return 3;
    }
    return 0;
}
