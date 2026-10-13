// Host check for MC_NetPack__Fv (source: ../w10-front-a-mc-net-pack).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// Builds a slot image, writes the expected packed image by hand from the layout in the source
// notes (not by calling the function under test) and compares every byte of the image plus the
// returned size. Covers a misaligned footprint, an empty directory, 26 directory entries and
// a script list of one entry. A 27th used entry is not tested: its walk would run into the data area.
#include "../w10-front-a-mc-net-pack/mc_net_pack.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
alignas(8) unsigned char ramSlot[0x40000];
int camTag;
void* gameCam = &camTag;
}

static int footCam, footJump, order[4], norder, checksum;
extern "C" int getPackChecksum__Fv() { order[norder++] = 0; return checksum; }
extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    if (camera != &camTag) return 1 << 20;
    order[norder++] = 1;
    return footCam;
}
extern "C" int jumpGateSaveFootPrint__Fv() { order[norder++] = 2; return footJump; }

namespace {
unsigned up(unsigned off) { return (off + 3u) & ~3u; }

struct Scenario {
    int foot_cam, foot_jump;
    int script_entries;   // entries before the terminator
    int dir_entries;      // used directory entries
    unsigned sizes_seed;
};

int run(const Scenario& sc) {
    footCam = sc.foot_cam;
    footJump = sc.foot_jump;
    checksum = 0x1234beef;
    for (unsigned i = 0; i < sizeof ramSlot; ++i) ramSlot[i] = static_cast<unsigned char>(0xc3 ^ (i * 5u));
    const unsigned dBase = 112u + sc.foot_cam + sc.foot_jump + 28102u;
    const unsigned sOff = up(dBase + 71994u);
    // script list: entries of 36 bytes with nonzero first byte, then a terminator entry
    for (int i = 0; i <= sc.script_entries; ++i) {
        unsigned char* e = ramSlot + sOff + 36u * i;
        for (int b = 0; b < 36; ++b) e[b] = static_cast<unsigned char>(1 + ((i * 13 + b * 7) % 250));
        if (i == sc.script_entries) e[0] = 0;
    }
    // directory
    unsigned totalSize = 0;
    unsigned sizes[27];
    for (int k = 0; k < 27; ++k) {
        unsigned char* e = ramSlot + 0x1b610 + 24u * k;
        for (int b = 0; b < 24; ++b) e[b] = static_cast<unsigned char>(1 + ((k * 11 + b * 3) % 250));
        sizes[k] = 8u + ((sc.sizes_seed + 5u * k) % 23u);
        std::memcpy(e + 20, &sizes[k], 4);
        if (k >= sc.dir_entries) e[0] = 0;
    }
    for (int k = 0; k < sc.dir_entries; ++k) totalSize += sizes[k];
    for (unsigned i = 0; i < totalSize; ++i) ramSlot[0x1b898 + i] = static_cast<unsigned char>(0x40 + i * 3);

    // expected image, written from the notes
    std::vector<unsigned char> want(ramSlot, ramSlot + sizeof ramSlot);
    want[110] = 0xef; want[111] = 0xbe;
    unsigned d = up(dBase);
    for (int i = 0; i <= sc.script_entries; ++i) std::memcpy(&want[d + 36u * i], ramSlot + sOff + 36u * i, 36);
    d = up(d + 36u * (sc.script_entries + 1));
    int n = sc.dir_entries < 26 ? sc.dir_entries : 26;
    std::memcpy(&want[d], &n, 4);
    d += 4;
    for (int k = 0; k < sc.dir_entries; ++k) {
        std::memcpy(&want[d], ramSlot + 0x1b610 + 24u * k, 24);
        d += 24;
    }
    d = up(d);
    std::memcpy(&want[d], ramSlot + 0x1b898, totalSize);
    d += totalSize;

    norder = 0;
    int got = MC_NetPack__Fv();
    if (order[0] != 0 || order[1] != 1 || order[2] != 2 || norder != 3) return 10;
    if (got != static_cast<int>(d)) return 11;
    if (std::memcmp(want.data(), ramSlot, sizeof ramSlot) != 0) return 12;
    return 0;
}
}

int main() {
    const Scenario scenarios[] = {
        {100, 8, 2, 3, 0},       // aligned footprint
        {101, 2, 1, 5, 7},       // misaligned offsets
        {103, 1, 0, 0, 3},       // terminator only, empty directory
        {96, 4, 3, 26, 11},      // 26 entries: stop at 26
        {97, 3, 2, 1, 2},        // one directory entry
    };
    for (unsigned i = 0; i < sizeof scenarios / sizeof scenarios[0]; ++i)
        if (int rc = run(scenarios[i])) { std::printf("scenario %u failed %d\n", i, rc); return rc; }
    std::puts("net pack passed");
    return 0;
}
