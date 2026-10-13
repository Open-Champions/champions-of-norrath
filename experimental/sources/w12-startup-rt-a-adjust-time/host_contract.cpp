// Host contract for AdjustTime. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The four helpers are scripted: the BCD converters really convert (so the record field order is
// exercised) and addhour/subhour count their calls and move the hour byte without carrying.
#include <cstdio>
#include <initializer_list>
static int adds, subs;
static unsigned char dec(unsigned char b) { return (b >> 4) * 10 + (b & 15); }
static unsigned char enc(unsigned char v) { return ((v / 10) << 4) | (v % 10); }
extern "C" {
void convertfrombcd(unsigned char* c) { for (int i : {1, 2, 3, 5, 6, 7}) c[i] = dec(c[i]); }
void converttobcd(unsigned char* c) { for (int i : {1, 2, 3, 5, 6, 7}) c[i] = enc(c[i]); }
void addhour(unsigned char* c) { ++adds; ++c[3]; }
void subhour(unsigned char* c) { ++subs; --c[3]; }
}
#include "adjust_time.cpp"
static int run(int minutes, unsigned char mi, int want_adds, int want_subs, unsigned char want_mi) {
    unsigned char c[8] = {0x77, 0x11, enc(mi), 0x10, 0x55, 0x04, 0x05, 0x06};
    adds = subs = 0;
    AdjustTime(c, minutes);
    if (adds != want_adds || subs != want_subs || c[2] != enc(want_mi) || c[0] != 0x77 || c[4] != 0x55) {
        std::printf("case %d/%d adds %d subs %d min %02x\n", minutes, mi, adds, subs, c[2]);
        return 1;
    }
    return c[1] == 0x11 && c[5] == 0x04 && c[6] == 0x05 && c[7] == 0x06 ? 0 : 1;
}
int main() {
    int bad = 0;
    bad += run(0, 30, 0, 0, 30);
    bad += run(29, 30, 0, 0, 59);
    bad += run(30, 30, 1, 0, 0);
    bad += run(-31, 30, 0, 1, 59);
    bad += run(-30, 30, 0, 0, 0);
    bad += run(150, 10, 2, 0, 40);
    bad += run(-541, 20, 0, 9, 19);
    bad += run(540, 0, 9, 0, 0);
    return bad;
}
