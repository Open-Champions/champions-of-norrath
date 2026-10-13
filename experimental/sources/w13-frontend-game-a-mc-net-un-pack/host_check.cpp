// Host check for MC_NetUnPack__Fv (source: ../w10-front-a-mc-net-un-pack). Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// A packed image is laid out by hand from the offsets in the source's header comment; the
// function must put the list back 71994 bytes later, the directory entries back at
// ramSlot + 0x1b610 (rest of the 648 bytes cleared) and the data at ramSlot + 0x1b898, and touch
// nothing else. Fails on a wrong offset, entry size, alignment, count, size-word offset or call order.
#include "../w10-front-a-mc-net-un-pack/mc_net_un_pack.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
alignas(16) unsigned char ramSlot[0xb29c8];
void* gameCam = reinterpret_cast<void*>(0x1234);
}
static std::string trace;
static int fp, jg;
extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    trace += camera == gameCam ? "c" : "?";
    return fp;
}
extern "C" int jumpGateSaveFootPrint__Fv() { trace += "j"; return jg; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static unsigned char b(int salt, int i) { return static_cast<unsigned char>((i * 37 + salt * 11 + 3) | 1); }
static unsigned align4(unsigned v) { return (v + 3) & ~3u; }

static int scenario(int fpv, int jgv, int nlist, const int* sizes, int ndir) {
    std::memset(ramSlot, 0xee, sizeof ramSlot);
    fp = fpv; jg = jgv; trace.clear();
    unsigned p0 = 112 + fpv + jgv + 28102;
    unsigned q = align4(p0 + 71994);
    unsigned p = align4(p0);
    // packed image: list (nlist entries plus terminator), int n, n directory entries, data
    for (int e = 0; e < nlist; ++e)
        for (int i = 0; i < 36; ++i) ramSlot[p + 36 * e + i] = b(e + 1, i);
    for (int i = 0; i < 36; ++i) ramSlot[p + 36 * nlist + i] = i == 0 ? 0 : static_cast<unsigned char>(0xc0 + i);
    unsigned char list[36 * 8];
    std::memcpy(list, ramSlot + p, 36 * (nlist + 1));
    p = align4(p + 36 * (nlist + 1));
    std::memcpy(ramSlot + p, &ndir, 4);
    p += 4;
    unsigned char dir[24 * 26];
    int total = 0;
    for (int e = 0; e < ndir; ++e) {
        for (int i = 0; i < 20; ++i) ramSlot[p + 24 * e + i] = b(50 + e, i);
        std::memcpy(ramSlot + p + 24 * e + 20, &sizes[e], 4);
        total += sizes[e];
    }
    std::memcpy(dir, ramSlot + p, 24 * ndir);
    p = align4(p + 24 * ndir);
    for (int i = 0; i < total; ++i) ramSlot[p + i] = static_cast<unsigned char>(i * 5 + 9);

    ramSlot[p + total] = 0x77;  // the byte after the packed data must not be copied

    MC_NetUnPack__Fv();

    CHECK(trace == "cj");
    for (int e = 0; e < ndir; ++e) CHECK(std::memcmp(ramSlot + 0x1b610 + 24 * e, dir + 24 * e, 24) == 0);
    for (int i = 24 * ndir; i < 648; ++i) CHECK(ramSlot[0x1b610 + i] == 0);
    for (int i = 0; i < total; ++i) CHECK(ramSlot[0x1b898 + i] == static_cast<unsigned char>(i * 5 + 9));
    CHECK(ramSlot[0x1b898 + total] == 0xee);
    CHECK(std::memcmp(ramSlot + q, list, 36 * (nlist + 1)) == 0);
    CHECK(ramSlot[q + 36 * (nlist + 1)] == 0xee);
    CHECK(ramSlot[q - 1] == 0xee);
    return 0;
}

int main() {
    const int three[3] = {8, 12, 4};
    const int one[1] = {20};
    const int none[1] = {0};
    int many[26];
    for (int i = 0; i < 26; ++i) many[i] = 4 * (i % 3 + 1);
    CHECK(scenario(5, 3, 3, three, 3) == 0);
    CHECK(scenario(0, 0, 0, none, 0) == 0);
    CHECK(scenario(1, 0, 1, one, 1) == 0);
    CHECK(scenario(2, 3, 5, three, 3) == 0);
    CHECK(scenario(9, 7, 2, many, 26) == 0);
    std::puts("net unpack passed");
    return 0;
}
