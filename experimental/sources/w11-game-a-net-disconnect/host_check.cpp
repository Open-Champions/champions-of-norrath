// Host contract for netDisconnect; not an EE input.
#include "net_disconnect.cpp"
#include <cstdio>
int nmPartners;
unsigned char partners[4 * 17632];
signed char totallyDisconnected;
static int localMask; static int asked[8], nAsked;
int netPlayerIsLocal__Fi(int p) { asked[nAsked++] = p; return (localMask >> p) & 1; }
int main() {
    int bad = 0;
    for (int mask = 0; mask < 16; ++mask) for (int n = -1; n <= 5; ++n) {
        localMask = mask; nmPartners = n; nAsked = 0; totallyDisconnected = 0;
        for (auto& b : partners) b = 0;
        netDisconnect__Fv();
        if (nAsked != 4 || asked[0] != 0 || asked[3] != 3 || totallyDisconnected != 1) { std::puts("fail: calls"); bad = 1; }
        for (int i = 0; i < 4; ++i) {
            int want = !((mask >> i) & 1) && i < n;
            for (int o = 0; o < 17632; ++o) {
                int expect = (o == 17621) ? want : 0;
                if (partners[i * 17632 + o] != expect) { std::printf("fail: mask %d n %d i %d off %d\n", mask, n, i, o); bad = 1; o = 17632; i = 4; mask = 16; n = 6; }
            }
        }
    }
    if (!bad) std::puts("netDisconnect ok");
    return bad;
}
