// Host contract for patchSortedListWithLocalTime__Fv. Not an EE plan input.
// sceScfGetLocalTimefromRTC is replaced by a stand-in that records its input and adds 0x10
// to byte 3 so the converted image is distinguishable from the input image.
#include <cstdio>
#include <cstring>

extern "C" {
int norrath_world_c_view_4861e8;
unsigned char sortedDir[640];
unsigned char sortedTimes[160];
void patchSortedListWithLocalTime__Fv();
}
static int calls, lower_at, lower_to;
static unsigned char seen[32][8];
extern "C" void sceScfGetLocalTimefromRTC(unsigned char* clock) {
    std::memcpy(seen[calls++], clock, 8);
    clock[3] = (unsigned char)(clock[3] + 0x10);
    clock[4] = 0x77;
    if (lower_at != 0 && calls == lower_at) norrath_world_c_view_4861e8 = lower_to;
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    std::memset(sortedTimes, 0xee, sizeof sortedTimes);
    unsigned char* r0 = sortedDir;
    r0[2] = 5; r0[3] = 23; r0[4] = 31; r0[5] = 12; r0[6] = 0xd2; r0[7] = 0x07;      // year 2002
    r0[10] = 59; r0[11] = 9; r0[12] = 1; r0[13] = 7; r0[14] = 0xe9; r0[15] = 0x07;   // year 2025
    unsigned char* r1 = sortedDir + 64;
    r1[2] = 99; r1[3] = 10; r1[4] = 20; r1[5] = 30; r1[6] = 0x6c; r1[7] = 0x07;      // 1900 -> 00
    r1[10] = 0; r1[11] = 0; r1[12] = 0; r1[13] = 0; r1[14] = 0xe8; r1[15] = 0x03;    // 1000 -> 00

    norrath_world_c_view_4861e8 = 0;
    patchSortedListWithLocalTime__Fv();
    expect(calls == 0 && sortedTimes[0] == 0xee, "zero count: nothing happens");
    norrath_world_c_view_4861e8 = -3;
    patchSortedListWithLocalTime__Fv();
    expect(calls == 0, "negative count: nothing happens");

    norrath_world_c_view_4861e8 = 2;
    patchSortedListWithLocalTime__Fv();
    expect(calls == 4, "two calls per record");
    // First image of record 0 comes from bytes 10..14: minute 59, hour 9, day 1, month 7, year 25.
    const unsigned char a[8] = {0, 0, 0x59, 0x09, seen[0][4], 0x01, 0x07, 0x25};
    expect(std::memcmp(seen[0], a, 8) == 0 && seen[0][4] == 0, "record 0 image from bytes 10..14");
    // Second from bytes 2..6: 5, 23, 31, 12, year 02.
    const unsigned char b[8] = {0, 0, 0x05, 0x23, 0x77, 0x31, 0x12, 0x02};
    expect(std::memcmp(seen[1], b, 8) == 0, "record 0 image from bytes 2..6 (byte 4 left from the previous call)");
    const unsigned char out0[16] = {0, 0, 0x59, 0x19, 0x77, 0x01, 0x07, 0x25, 0, 0, 0x05, 0x33, 0x77, 0x31, 0x12, 0x02};
    expect(std::memcmp(sortedTimes, out0, 16) == 0, "record 0 times: modified stamp first, created stamp second");
    // Record 1: 99 -> 0x99, 10 -> 0x10, 20, 30, 1900 % 100 = 0; second image from zeros.
    const unsigned char c[8] = {0, 0, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00};
    expect(std::memcmp(seen[2], c, 8) == 0, "record 1 first image: zero stamp, year 1000 % 100 = 0");
    expect(seen[3][2] == 0x99 && seen[3][3] == 0x10 && seen[3][5] == 0x20 && seen[3][6] == 0x30 && seen[3][7] == 0x00, "record 1 second image");
    expect(sortedTimes[32] == 0xee && sortedTimes[16 + 3] == 0x10, "record 1 written, record 2 untouched");
    expect(sortedTimes[16 + 10] == 0x99 && sortedTimes[16 + 11] == 0x20, "record 1 second half");

    // The count is re-read after every entry: lowering it during the second call stops the loop after record 0.
    calls = 0; lower_at = 2; lower_to = 1;
    std::memset(sortedTimes, 0xee, sizeof sortedTimes);
    norrath_world_c_view_4861e8 = 5;
    patchSortedListWithLocalTime__Fv();
    expect(calls == 2 && sortedTimes[16] == 0xee, "count re-read: loop ends after record 0");
    if (failures) return 1;
    std::printf("patchSortedListWithLocalTime contract passed\n");
    return 0;
}
