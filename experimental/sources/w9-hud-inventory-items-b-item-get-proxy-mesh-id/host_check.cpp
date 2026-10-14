#include <initializer_list>
#include <cstdlib>
struct Desc { unsigned char before[52]; short base; };
struct Rec { const Desc* d; unsigned char gap[8 - sizeof(void*)]; long long flags; };
extern "C" int getProxyMeshId__4itemPQ24item11description(const Rec*);

int main() {
    Desc d = {};
    Rec r = {&d, {}, 0};
    for (int base : {-5, 0, 7, 300}) {
        d.base = (short)base;
        for (int field = 0; field < 32; ++field) {
            for (int bit36 = 0; bit36 < 2; ++bit36) {
                // other bits set around the field must not matter
                r.flags = ((long long)field << 11) | ((long long)bit36 << 36) | 0x7ff | 0xffe0000LL;
                int want = base + (bit36 ? field : 0);
                if (getProxyMeshId__4itemPQ24item11description(&r) != want) return 1;
            }
        }
    }
    d.base = 10;
    r.flags = (1LL << 35) | (1LL << 37) | (31LL << 11);
    if (getProxyMeshId__4itemPQ24item11description(&r) != 10) return 2;
    return 0;
}
