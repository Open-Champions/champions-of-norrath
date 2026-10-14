// Host contract for checkYawReset__6Camera. Not an EE plan input.
#include "check_yaw_reset.cpp"

#include <cstdio>
#include <cstring>

unsigned levelFlags;
unsigned char pad[88 * 4];
unsigned char player[23984 * 4];

static int calls, got_from, got_to;
static int* got_amount;
int fastestRotDirection__FssPi(short from, short to, int* amount) {
    ++calls; got_from = from; got_to = to; got_amount = amount;
    return 777;
}

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static void put32(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }
static void putf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static int get32(const unsigned char* base, int off) { int v; std::memcpy(&v, base + off, 4); return v; }
static short get16(const unsigned char* base, int off) { short v; std::memcpy(&v, base + off, 2); return v; }

static unsigned char camera[512];
static unsigned char targetA[64], targetB[64];

static void reset() {
    std::memset(camera, 0, sizeof camera);
    std::memset(pad, 0, sizeof pad);
    std::memset(player, 0, sizeof player);
    levelFlags = 0;
    put32(camera, 64, 1);
    put32(pad + 88, 12, 0x200);
    putf(pad + 88, 20, 0.1f);
    putf(pad + 88, 24, 0.1f);
    camera[353] = 0xff;
    short yaw = 1234; std::memcpy(camera + 104, &yaw, 2);
    unsigned short half = 5000; std::memcpy(targetA + 56, &half, 2);
    half = 0xf000; std::memcpy(targetB + 56, &half, 2);
    calls = 0;
}

static void run() { checkYawReset__6Camera(camera); }

int main() {
    reset(); run();
    expect(calls == 1 && get32(camera, 56) == 777, "baseline reaches the call");
    expect(get16(camera, 138) == -8192 && got_to == -8192 && got_from == 1234 && got_amount == nullptr, "null target and negative byte give -8192");

    reset(); levelFlags = 0x2000000; run(); expect(calls == 0, "level flag blocks");
    reset(); levelFlags = 0xfdffffff; run(); expect(calls == 1, "other level flags do not block");
    reset(); camera[352] = 1; run(); expect(calls == 0, "byte 352 blocks");
    reset(); put32(camera, 100, 1); run(); expect(calls == 0, "word 100 blocks");
    reset(); put32(camera, 56, 5); run(); expect(calls == 0 && get32(camera, 56) == 5, "word 56 blocks");
    reset(); put32(pad + 88, 12, 0x1ff); run(); expect(calls == 0, "pad flag bit 0x200 needed");
    reset(); putf(pad + 88, 20, 0.2f); putf(pad + 88, 24, 0.0f); run(); expect(calls == 0, "0.04 is not below 0.032");
    reset(); putf(pad + 88, 20, 0.17f); putf(pad + 88, 24, 0.0f); run(); expect(calls == 1, "0.0289 is below 0.032");
    reset(); putf(pad + 88, 20, 0.0f); putf(pad + 88, 24, -0.17f); run(); expect(calls == 1, "negative component squared");

    reset(); { unsigned char* p = targetA; std::memcpy(player + 23984 + 21872, &p, sizeof p); }
    run(); expect(get16(camera, 138) == 16384 - 5000 && got_to == 16384 - 5000, "own target");
    reset(); { unsigned char* p = targetB; std::memcpy(player + 23984 + 21872, &p, sizeof p); }
    run(); expect(get16(camera, 138) == static_cast<short>(16384 - 0xf000), "own target halfword is unsigned and result truncated");
    reset(); { unsigned char* p = targetA; std::memcpy(player + 2 * 23984 + 21872, &p, sizeof p); }
    camera[353] = 2; run(); expect(get16(camera, 138) == 16384 - 5000, "fallback player from byte 353");
    reset(); camera[353] = 2; run(); expect(get16(camera, 138) == -8192 && calls == 1, "fallback player without target");
    reset(); { unsigned char* p = targetA; std::memcpy(player + 23984 + 21872, &p, sizeof p); }
    camera[353] = 2; run(); expect(get16(camera, 138) == 16384 - 5000, "own target wins over byte 353");
    return failures ? 1 : 0;
}
