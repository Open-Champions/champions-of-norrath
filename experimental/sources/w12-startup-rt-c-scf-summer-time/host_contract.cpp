// Host check only; not an EE build input. Build the source with -DsceScfGetSummerTime=w12_st.
#include <cstdio>
extern "C" {
unsigned char gConfig[8];
static int t10k, p2_calls, p2_a1, p2_a2; static unsigned osd_word; static unsigned char p2_byte;
int IsT10K() { return t10k; }
void GetOsdConfigParam(unsigned* d) { *d = osd_word; }
void GetOsdConfigParam2(unsigned char* d, int a, int b) { ++p2_calls; p2_a1 = a; p2_a2 = b; d[0] = p2_byte; }
int w12_st();
}
int main() {
    t10k = 1; gConfig[6] = 0xff; gConfig[5] = 0; gConfig[7] = 0;
    if (w12_st() != 255 || p2_calls) return 1;      // unsigned byte, not masked
    gConfig[6] = 0; gConfig[5] = 1; gConfig[7] = 1;
    if (w12_st() != 0) return 2;
    t10k = 0; osd_word = 0xffff1fffu; p2_byte = 0xff;
    if (w12_st() != 0 || p2_calls) return 3;        // field zero: no second query
    osd_word = 1u << 13; p2_byte = 0x10;
    if (w12_st() != 1 || p2_calls != 1 || p2_a1 != 1 || p2_a2 != 1) return 4;
    p2_byte = 0xef; if (w12_st() != 0) return 5;
    p2_byte = 0x20; if (w12_st() != 0) return 6;
    p2_byte = 0xff; if (w12_st() != 1) return 7;
    std::puts("sceScfGetSummerTime ok");
    return 0;
}
