#include "end_billboards.cpp"
#include <cstring>
#include <vector>

static std::vector<unsigned char*> slot_at_flush;
static unsigned char* g_raster;
extern "C" void FlushDMABufferIfFull__8VIRaster(void* r) {
    unsigned char* p;
    std::memcpy(&p, static_cast<unsigned char*>(r) + 19492, sizeof p);
    slot_at_flush.push_back(p);
}

int main() {
    alignas(16) static unsigned char raster[20000];
    alignas(16) static unsigned char packet[2048];
    for (int n : {-2, 0, 1, 2, 5}) {
        std::memset(raster, 0x44, sizeof raster);
        std::memset(packet, 0x99, sizeof packet);
        unsigned char* p = packet + 32;
        std::memcpy(raster + 19492, &p, sizeof p);
        std::memcpy(raster + 17860, &n, 4);
        alignas(16) static unsigned char want[2048];
        std::memcpy(want, packet, sizeof packet);
        slot_at_flush.clear();
        int r = EndBillboards__8VIRaster(raster);
        if (n <= 0) {
            if (r != -1 || !slot_at_flush.empty()) return 1;
            if (std::memcmp(want, packet, sizeof packet) != 0) return 2;
            unsigned char* now; std::memcpy(&now, raster + 19492, sizeof now);
            int cnt; std::memcpy(&cnt, raster + 17860, 4);
            if (now != p || cnt != n) return 3;
            continue;
        }
        if (r != 0 || slot_at_flush.size() != 1) return 4;
        unsigned c = 3 * n + 3;
        unsigned w;
        std::memcpy(&w, want + 32 + 48 * n + 12, 4); w |= 0x100; std::memcpy(want + 32 + 48 * n + 12, &w, 4);
        unsigned closing[4] = {0, 0, 0, 0x15000014u};
        std::memcpy(want + 32 + 48 * n + 48, closing, 16);
        want[32 + 3] = 16;
        unsigned zero = 0; std::memcpy(want + 32 + 4, &zero, 4);
        unsigned short h = static_cast<unsigned short>(c); std::memcpy(want + 32, &h, 2);
        if (std::memcmp(want, packet, sizeof packet) != 0) return 5;
        if (slot_at_flush[0] != p + 16 * c + 16) return 6;
        int cnt; std::memcpy(&cnt, raster + 17860, 4);
        if (cnt != 0) return 7;
    }
    return 0;
}
