// Host check for MC_NetUnPack__Fv (source: ../w10-front-a-mc-net-un-pack).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// A packed slot image is laid out by hand from the layout in the source notes, the expected
// unpacked image is derived independently (snapshot, then memmove / memset / copy in the order
// the notes give) and compared byte for byte. One scenario makes the data move overlap its
// destination, which only a backward copy handles; one has an empty directory.
#include "../w10-front-a-mc-net-un-pack/mc_net_un_pack.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
alignas(8) unsigned char ramSlot[0x40000];
int camTag;
void* gameCam = &camTag;
}

static int footCam, footJump, norder, order[4];
extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    if (camera != &camTag) return 1 << 20;
    order[norder++] = 1;
    return footCam;
}
extern "C" int jumpGateSaveFootPrint__Fv() { order[norder++] = 2; return footJump; }

namespace {
unsigned up(unsigned off) { return (off + 3u) & ~3u; }

struct Scenario {
    int foot_cam, foot_jump, script_entries, dir_entries;
    unsigned size_each;  // size word of every directory entry
};

int run(const Scenario& sc) {
    footCam = sc.foot_cam;
    footJump = sc.foot_jump;
    for (unsigned i = 0; i < sizeof ramSlot; ++i) ramSlot[i] = static_cast<unsigned char>(0x9d ^ (i * 3u));
    const unsigned dBase = 112u + sc.foot_cam + sc.foot_jump + 28102u;
    const unsigned qOff = up(dBase + 71994u);
    unsigned p = up(dBase);
    // packed script list
    unsigned listOff = p;
    for (int i = 0; i <= sc.script_entries; ++i) {
        unsigned char* e = ramSlot + p + 36u * i;
        for (int b = 0; b < 36; ++b) e[b] = static_cast<unsigned char>(1 + ((i * 9 + b * 5) % 250));
        if (i == sc.script_entries) e[0] = 0;
    }
    p = up(p + 36u * (sc.script_entries + 1));
    std::memcpy(ramSlot + p, &sc.dir_entries, 4);
    p += 4;
    const unsigned dirOff = p;
    for (int k = 0; k < sc.dir_entries; ++k) {
        unsigned char* e = ramSlot + p + 24u * k;
        for (int b = 0; b < 24; ++b) e[b] = static_cast<unsigned char>(1 + ((k * 7 + b * 11) % 250));
        std::memcpy(e + 20, &sc.size_each, 4);
    }
    p = up(p + 24u * sc.dir_entries);
    const unsigned dataOff = p;
    const unsigned total = sc.size_each * static_cast<unsigned>(sc.dir_entries);
    for (unsigned i = 0; i < total; ++i) ramSlot[dataOff + i] = static_cast<unsigned char>(0x21 + i * 7u);

    // expected image, from a snapshot
    std::vector<unsigned char> snap(ramSlot, ramSlot + sizeof ramSlot), want = snap;
    std::memmove(&want[0x1b898], &snap[dataOff], total);
    std::memset(&want[0x1b610], 0, 648);
    for (int k = 0; k < sc.dir_entries; ++k) std::memcpy(&want[0x1b610 + 24u * k], &snap[dirOff + 24u * k], 24);
    for (int i = 0; i <= sc.script_entries; ++i) std::memcpy(&want[qOff + 36u * i], &snap[listOff + 36u * i], 36);

    norder = 0;
    MC_NetUnPack__Fv();
    if (norder != 2 || order[0] != 1 || order[1] != 2) return 10;
    if (std::memcmp(want.data(), ramSlot, sizeof ramSlot) != 0) return 11;
    return 0;
}
}

int main() {
    const Scenario scenarios[] = {
        {100, 8, 2, 3, 40},        // aligned
        {101, 2, 1, 4, 33},        // misaligned footprints and odd sizes
        {103, 1, 0, 0, 0},         // terminator only, empty directory
        {97, 3, 5, 26, 30},        // 26 entries
        {96, 0, 2, 1, 85000},      // data move overlaps its destination (needs a backward copy)
    };
    for (unsigned i = 0; i < sizeof scenarios / sizeof scenarios[0]; ++i)
        if (int rc = run(scenarios[i])) { std::printf("scenario %u failed %d\n", i, rc); return rc; }
    std::puts("net unpack passed");
    return 0;
}
