#include "init.cpp"
#include <initializer_list>
#include <string.h>

static int size_result, alloc_result;
static int order[4], norder;
static unsigned char* seen_self[2];
static unsigned char snapshot_at_size[128];
extern "C" int CalcDataSize__12VICollBuffer(unsigned char* self) {
    order[norder++] = 1;
    seen_self[0] = self;
    memcpy(snapshot_at_size, self, 128);
    return size_result;
}
extern "C" int Allocate__12VICollBuffer(unsigned char* self) {
    order[norder++] = 2;
    seen_self[1] = self;
    return alloc_result;
}

int main() {
    for (int type : {0, 1, 2, 3}) {
        for (int sr : {-5, 0, 40}) {
            for (int ar : {-1, 0, 9}) {
                alignas(16) unsigned char o[128];
                memset(o, 0x61, sizeof o);
                unsigned p0 = 100, p1 = 200, p2 = 300, base = 1000;
                memcpy(o + 68, &p0, 4);
                memcpy(o + 72, &p1, 4);
                memcpy(o + 76, &p2, 4);
                memcpy(o + 88, &base, 4);
                unsigned char want[128];
                memcpy(want, o, sizeof o);
                int t = type == 2 ? 3 : type;
                int vals[4] = {t, 11, 22, 33};
                memcpy(want + 8, &vals[0], 4);
                memcpy(want + 48, &vals[1], 4);
                memcpy(want + 56, &vals[2], 4);
                memcpy(want + 64, &vals[3], 4);
                size_result = sr;
                alloc_result = ar;
                norder = 0;
                int expect_n = sr < 0 ? 1 : 2;
                bool ok = sr >= 0 && ar >= 0;
                if (ok) {
                    unsigned a = p0 + base, b = p1 + base, c = p2 + base;
                    memcpy(want + 68, &a, 4);
                    memcpy(want + 72, &b, 4);
                    memcpy(want + 76, &c, 4);
                }
                if (Init__12VICollBuffer16VICollBufferTypeiii(o, type, 11, 22, 33) != (ok ? 0 : -1)) return 1;
                if (norder != expect_n || order[0] != 1 || (expect_n == 2 && order[1] != 2)) return 2;
                if (seen_self[0] != o) return 3;
                if (memcmp(o, want, sizeof o) != 0) return 4;
                if (memcmp(snapshot_at_size + 8, want + 8, 4) != 0) return 5;
            }
        }
    }
    return 0;
}
