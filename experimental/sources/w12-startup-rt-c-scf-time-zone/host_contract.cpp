// Host check only; not an EE build input. Build the source with -DsceScfGetTimeZone=w12_tz.
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char gConfig[8];
static int t10k, osd_calls; static unsigned osd_word;
int IsT10K() { return t10k; }
void GetOsdConfigParam(unsigned* d) { ++osd_calls; *d = osd_word; }
int w12_tz();
}
int main() {
    t10k = 1; gConfig[0] = 0x34; gConfig[1] = 0x12; osd_word = 0xffffffffu;
    if (w12_tz() != 0x1234 || osd_calls) return 1;
    gConfig[0] = 0x00; gConfig[1] = 0x80;
    if (w12_tz() != -32768) return 2;            // sign-extended halfword
    gConfig[0] = 0x00; gConfig[1] = 0x00; gConfig[2] = 0xff;
    if (w12_tz() != 0) return 3;                 // only the halfword at 0
    t10k = 0;
    osd_word = 0x7fe0u << 16 | 0;                 // bits 13..15 zero
    if (w12_tz() != 540 || osd_calls != 1) return 4;
    osd_word = (0x0120u << 21) | (1u << 13);      // positive zone, field 1
    if (w12_tz() != 0x120) return 5;
    osd_word = (0x7e0u << 21) | (7u << 13);       // negative: top 11 bits 0x7e0
    if (w12_tz() != -32) return 6;
    osd_word = 0x80000000u | (4u << 13);
    if (w12_tz() != -1024) return 7;
    osd_word = 0x00001fffu;                       // field zero despite other bits
    if (w12_tz() != 540) return 8;
    osd_word = 0xffffe000u;                       // field 7
    if (w12_tz() != -1) return 9;
    std::puts("sceScfGetTimeZone ok");
    return 0;
}
