// Host contract for convertIndiciesToPointers__Fi. Not an EE plan input. Pointer words are the
// low 32 bits of the host address, so the check compares those.
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "convert_indicies_to_pointers.cpp"

extern "C" alignas(8) unsigned char player[23984 * 2];
alignas(8) unsigned char player[23984 * 2];
static std::vector<std::string> events;
static std::vector<unsigned char*> unpacked;
extern "C" void unpack__4itemPQ24item11description(void* d) { events.push_back("unpack"); unpacked.push_back((unsigned char*)d); }
extern "C" void playerPrintPaperDoll__Fi(int n) { events.push_back("paper " + std::to_string(n)); }

static void sw(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static unsigned lw(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return (unsigned)(uintptr_t)p; }

int main() {
    const unsigned R = 23984;
    for (int p = 0; p < 2; ++p) {
        unsigned char* rec = player + R * p;
        std::memset(player, 0x55, sizeof player);
        sw(rec + 4936, 3); sw(rec + 4936 + 4616, 0); sw(rec + 4936 + 2 * 4616, 2);
        unsigned in[19] = {0, 2, 3, 1000, 1002, 2001, 2002, 0xffffffffu, 1, 7, 0x80000000u, 2000, 999, 3000, 2, 1, 0xfffffffeu, 5, 2001};
        const unsigned off0 = 21896;
        for (int k = 0; k < 19; ++k) sw(rec + off0 + 4 * k, in[k]);
        events.clear(); unpacked.clear();
        convertIndiciesToPointers__Fi(p);
        // unpack: list 0 elements 0..2, list 2 elements 0..1, in order, then the paper-doll call last
        if (unpacked.size() != 5) return 1;
        if (unpacked[0] != rec + 328 || unpacked[1] != rec + 328 + 72 || unpacked[2] != rec + 328 + 144) return 2;
        if (unpacked[3] != rec + 328 + 2 * 4616 || unpacked[4] != rec + 328 + 2 * 4616 + 72) return 3;
        if (events.size() != 6 || events.back() != "paper " + std::to_string(p)) return 4;
        const unsigned c0 = addr(rec + 328), c1 = addr(rec + 328 + 72), c2 = addr(rec + 328 + 144);
        const unsigned e0 = addr(rec + 328 + 2 * 4616), e1 = addr(rec + 328 + 2 * 4616 + 72);
        unsigned want[19] = {c0, c2, 0, 0, 0, e1, 0, 0, c1, 0, 0, e0, 0, 0, c2, c1, 0, 0, e1};
        for (int k = 0; k < 19; ++k)
            if (lw(rec + off0 + 4 * k) != want[k]) return 10 + k;
        // everything else is untouched; the other record is untouched
        if (rec[off0 - 1] != 0x55 || rec[off0 + 76] != 0x55) return 40;
        if (player[(1 - p) * R + 100] != 0x55) return 41;
    }
    return 0;
}
