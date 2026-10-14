#include "init.cpp"
#include <initializer_list>
#include <string.h>

static int size_result, alloc_result;
static int order[4], norder;
static int got_args[3];
static unsigned char* seen_self;
extern "C" int CalcDataSize__12VIPrimBufferiii(unsigned char* self, int a, int b, int c) {
    order[norder++] = 1;
    seen_self = self;
    got_args[0] = a; got_args[1] = b; got_args[2] = c;
    return size_result;
}
extern "C" int Allocate__12VIPrimBuffer(unsigned char* self) {
    order[norder++] = 2;
    return seen_self == self ? alloc_result : -77;
}

int main() {
    for (int type : {0, 1, 5}) {
        for (int sr : {-5, 0, 40}) {
            for (int ar : {-1, 0, 9}) {
                alignas(16) unsigned char o[128];
                memset(o, 0x61, sizeof o);
                unsigned p0 = 100, p1 = 200, p2 = 300, base = 1000;
                memcpy(o + 88, &p0, 4);
                memcpy(o + 92, &p1, 4);
                memcpy(o + 96, &p2, 4);
                memcpy(o + 104, &base, 4);
                unsigned char want[128];
                memcpy(want, o, sizeof o);
                int v[4] = {type, 11, 22, 33};
                memcpy(want + 8, &v[0], 4);
                memcpy(want + 68, &v[1], 4);
                memcpy(want + 76, &v[2], 4);
                size_result = sr;
                alloc_result = ar;
                norder = 0;
                int expect_n = sr < 0 ? 1 : 2;
                bool ok = sr >= 0 && ar >= 0;
                if (ok) {
                    unsigned a = p0 + base, b = p1 + base, c = p2 + base;
                    memcpy(want + 88, &a, 4);
                    memcpy(want + 92, &b, 4);
                    memcpy(want + 96, &c, 4);
                }
                if (Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(o, 0, type, 11, 22, 33) != (ok ? 0 : -1)) return 1;
                if (norder != expect_n || order[0] != 1 || (expect_n == 2 && order[1] != 2)) return 2;
                if (got_args[0] != 11 || got_args[1] != 22 || got_args[2] != 33) return 3;
                if (memcmp(o, want, sizeof o) != 0) return 4;
            }
        }
    }
    return 0;
}
