#include "pad_run_frame.cpp"
#include <cstring>
int padFilter;
signed char padIndexMap[4];
signed char netNetworkMode;
signed char tapCount[2];
unsigned rumphole[24];
unsigned char actSendData[8];
static int localMask;
int netPlayerIsLocal__Fi(int index) { return (localMask >> index) & 1; }
struct Call { int port, slot; unsigned char b0, b1; };
static Call calls[16];
static int ncalls;
int scePadSetActDirect(int port, int slot, unsigned char* data) {
    if (data != actSendData) return -1;
    calls[ncalls++] = Call{port, slot, data[0], data[1]};
    return 1;
}
static float f(unsigned u) { float v; std::memcpy(&v, &u, 4); return v; }
static unsigned u(float v) { unsigned r; std::memcpy(&r, &v, 4); return r; }
static void reset() {
    std::memset(rumphole, 0, sizeof rumphole);
    std::memset(actSendData, 0xaa, sizeof actSendData);
    ncalls = 0; netNetworkMode = 0; localMask = 0xf;
    tapCount[0] = 1; tapCount[1] = 4;
    for (int i = 0; i < 4; ++i) padIndexMap[i] = static_cast<signed char>(i);
}
#define CHECK(c, n) do { if (!(c)) return n; } while (0)
int main() {
    // idle: nothing to send, records already (0,0); filter floors at zero
    reset(); padFilter = 0;
    padRunFrame__Fv();
    CHECK(padFilter == 0 && ncalls == 0, 1);
    padFilter = 5; padRunFrame__Fv();
    CHECK(padFilter == 4, 2);
    // small motor ticks on pad 2 (port 1, slot 1); large motor idle
    reset();
    rumphole[2 * 6 + 0] = 2;
    padRunFrame__Fv();
    CHECK(rumphole[12] == 1 && rumphole[2 * 6 + 4] == 1 && rumphole[2 * 6 + 5] == 0, 3);
    CHECK(ncalls == 1 && calls[0].port == 1 && calls[0].slot == 1 && calls[0].b0 == 1 && calls[0].b1 == 0, 4);
    padRunFrame__Fv();  // counter 1 -> 0, still active this frame, same pair: no new call
    CHECK(rumphole[12] == 0 && ncalls == 1, 5);
    padRunFrame__Fv();  // now off: pair changes to (0,0): one more call
    CHECK(ncalls == 2 && calls[1].b0 == 0 && rumphole[2 * 6 + 4] == 0, 6);
    padRunFrame__Fv();
    CHECK(ncalls == 2, 7);
    // large motor: strength 0.5 decays by 0.25 before use, then x255 truncated
    reset();
    rumphole[1 * 6 + 1] = 3;
    rumphole[1 * 6 + 2] = u(0.5f);
    rumphole[1 * 6 + 3] = u(0.25f);
    padRunFrame__Fv();
    CHECK(f(rumphole[8]) == 0.25f && rumphole[7] == 2, 8);
    CHECK(ncalls == 1 && calls[0].port == 1 && calls[0].slot == 0 && calls[0].b1 == 63 && rumphole[11] == 63, 9);
    padRunFrame__Fv();  // strength 0.0; byte 0 sent
    CHECK(f(rumphole[8]) == 0.0f && ncalls == 2 && calls[1].b1 == 0, 10);
    // decay below zero clamps to 0.0, and the strength byte uses the clamped value
    reset();
    rumphole[0 * 6 + 1] = 1;
    rumphole[0 * 6 + 2] = u(0.5f);
    rumphole[0 * 6 + 3] = u(0.75f);
    padRunFrame__Fv();
    CHECK(f(rumphole[2]) == 0.0f && rumphole[1] == 0 && ncalls == 0, 11);  // pair already (0,0)
    // padIndexMap redirects the record but not the port/slot
    reset();
    padIndexMap[3] = 0;
    rumphole[0 * 6 + 0] = 1;
    padRunFrame__Fv();  // index 0 sends (1,0) for record 0; index 3 reuses record 0, now idle, and sends (0,0)
    CHECK(ncalls == 2 && calls[0].port == 0 && calls[0].slot == 0 && calls[0].b0 == 1, 12);
    CHECK(calls[1].port == 1 && calls[1].slot == -1 && calls[1].b0 == 0, 16);  // mapped 0 minus tapCount[0] 1
    // network mode: nonlocal indexes skipped, local ones use port 0 slot 0
    reset();
    netNetworkMode = 1; localMask = 0x4;
    rumphole[1 * 6 + 0] = 1;  // pad 1 not local: untouched
    rumphole[2 * 6 + 0] = 1;
    padRunFrame__Fv();
    CHECK(rumphole[6] == 1 && rumphole[12] == 0, 13);
    CHECK(ncalls == 1 && calls[0].port == 0 && calls[0].slot == 0 && calls[0].b0 == 1, 14);
    // slot at or beyond tapCount[port] is not sent but the cache is still updated
    reset();
    tapCount[1] = 1;  // port 1 has slots 0 only
    rumphole[3 * 6 + 0] = 1;  // index 3 -> port 1 slot 2: not below tapCount[1]
    padRunFrame__Fv();
    CHECK(ncalls == 0 && rumphole[3 * 6 + 4] == 1, 15);
    return 0;
}
