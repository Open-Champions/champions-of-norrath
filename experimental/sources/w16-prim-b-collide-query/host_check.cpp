#include <initializer_list>
#include <string.h>
static int g_which;
static const void* g_args[5];
static void rec(int which, const void* a, const void* b, const void* c, const void* d, const void* e) {
    g_which = which;
    g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d; g_args[4] = e;
}
extern "C" int QueryBufferV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void* a, const void* b, const void* c, void* d, void* e) { rec(10, a, b, c, d, e); return 111; }
extern "C" int QueryBufferPackV__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void* a, const void* b, const void* c, void* d, void* e) { rec(11, a, b, c, d, e); return 222; }
extern "C" int QueryBufferPackVGF__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(void* a, const void* b, const void* c, void* d, void* e) { rec(13, a, b, c, d, e); return 333; }
#include "query.cpp"

int main() {
    char self, b1, b2, list;
    for (int type : {0, 1, 2, 3, 4, -1}) {
        alignas(16) unsigned char buffer[32];
        memset(buffer, 0x44, sizeof buffer);
        memcpy(buffer + 8, &type, 4);
        g_which = 0;
        int r = Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(&self, &b1, &b2, &list, buffer);
        int want_which = type == 0 ? 10 : type == 1 ? 11 : type == 3 ? 13 : 0;
        int want_ret = type == 0 ? 111 : type == 1 ? 222 : type == 3 ? 333 : -1;
        if (g_which != want_which || r != want_ret) return 1;
        if (want_which && (g_args[0] != &self || g_args[1] != &b1 || g_args[2] != &b2 || g_args[3] != &list || g_args[4] != buffer)) return 2;
    }
    return 0;
}
