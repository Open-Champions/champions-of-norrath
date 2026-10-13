// Host check for MC_NetPack__Fv (source: ../w10-front-a-mc-net-pack). Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The expected layout is computed here by hand from the offsets in the source's header comment
// (not by calling the function twice), so a wrong offset, entry size, alignment, count cap,
// size-word offset or call order fails.
#include "../w10-front-a-mc-net-pack/mc_net_pack.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(16) unsigned char ramSlot[0xb29c8];
void* gameCam = reinterpret_cast<void*>(0x1234);
}
static std::string trace;
static int fp, jg, checksum;
extern "C" int getPackChecksum__Fv() { trace += "k"; return checksum; }
extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    trace += camera == gameCam ? "c" : "?";
    return fp;
}
extern "C" int jumpGateSaveFootPrint__Fv() { trace += "j"; return jg; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static unsigned char b(int salt, int i) { return static_cast<unsigned char>((i * 37 + salt * 11 + 3) | 1); }  // never zero

static unsigned align4(unsigned v) { return (v + 3) & ~3u; }

// nlist: number of nonzero 36-byte entries before the terminating entry; sizes: directory size words
static int scenario(int fpv, int jgv, int nlist, const int* sizes, int ndir, int chk, int expect_n) {
    std::memset(ramSlot, 0xee, sizeof ramSlot);
    fp = fpv; jg = jgv; checksum = chk; trace.clear();
    unsigned d0 = 112 + fpv + jgv + 28102;
    unsigned s0 = align4(d0 + 71994);
    for (int e = 0; e < nlist; ++e)
        for (int i = 0; i < 36; ++i) ramSlot[s0 + 36 * e + i] = b(e + 1, i);
    for (int i = 0; i < 36; ++i) ramSlot[s0 + 36 * nlist + i] = i == 0 ? 0 : static_cast<unsigned char>(0xc0 + i);
    unsigned char* table = ramSlot + 0x1b610;
    int total = 0;
    for (int e = 0; e < ndir; ++e) {
        for (int i = 0; i < 24; ++i) table[24 * e + i] = i < 20 ? b(50 + e, i) : 0;
        std::memcpy(table + 24 * e + 20, &sizes[e], 4);
        total += sizes[e];
    }
    if (ndir < 27) std::memset(table + 24 * ndir, 0, 24);
    for (int i = 0; i < total; ++i) ramSlot[0x1b898 + i] = static_cast<unsigned char>(i * 5 + 9);
    for (int i = total; i < total + 64; ++i) ramSlot[0x1b898 + i] = 0x77;  // bytes past the data must not be copied
    if (ndir >= 27) ramSlot[0x1b898] = 0;  // the data area's first byte ends the scan of an unterminated directory
    unsigned char snapshot_table[24 * 27];
    std::memcpy(snapshot_table, table, sizeof snapshot_table);

    int result = MC_NetPack__Fv();

    CHECK(trace == "kcj");  // checksum, then camera footprint, then jump-gate footprint
    CHECK(ramSlot[110] == (chk & 0xff) && ramSlot[111] == ((chk >> 8) & 0xff));
    unsigned d = align4(d0);
    for (int e = 0; e <= nlist; ++e)
        CHECK(std::memcmp(ramSlot + d + 36 * e, ramSlot + s0 + 36 * e, 36) == 0);
    d = align4(d + 36 * (nlist + 1));
    int stored;
    std::memcpy(&stored, ramSlot + d, 4);
    CHECK(stored == expect_n);
    d += 4;
    for (int e = 0; e < ndir; ++e) CHECK(std::memcmp(ramSlot + d + 24 * e, snapshot_table + 24 * e, 24) == 0);
    d = align4(d + 24 * ndir);
    for (int i = 0; i < total; ++i) CHECK(ramSlot[d + i] == (i == 0 && ndir >= 27 ? 0 : static_cast<unsigned char>(i * 5 + 9)));
    d += total;
    CHECK(ramSlot[d] == 0xee);  // nothing is written past the copied data
    CHECK(result == static_cast<int>(d));
    return 0;
}

int main() {
    const int none[1] = {0};
    const int three[3] = {8, 12, 4};
    const int one[1] = {20};
    int many[27];
    for (int i = 0; i < 27; ++i) many[i] = 4;
    // d0 = 112 + fp + jg + 28102 (unaligned cases move d up to a multiple of four)
    CHECK(scenario(5, 3, 3, three, 3, 0x1a2b, 3) == 0);
    CHECK(scenario(0, 0, 0, none, 0, 0x7, 0) == 0);
    CHECK(scenario(1, 0, 1, one, 1, -2, 1) == 0);
    CHECK(scenario(2, 3, 5, three, 3, 0x12345, 3) == 0);
    // 27 used directory entries: the stored count stops at 26 while 27 entries are copied
    CHECK(scenario(9, 7, 2, many, 27, 0x55aa, 26) == 0);
    std::puts("net pack passed");
    return 0;
}
