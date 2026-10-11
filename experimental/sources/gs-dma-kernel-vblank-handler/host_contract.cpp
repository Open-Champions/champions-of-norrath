#define NORRATH_HOST_CONTRACT 1
#include "vblank_handler.cpp"

#include <cstdio>
#include <cstring>

extern "C" int engineDisplayDisable = 0;
extern "C" unsigned char specialMpegMode = 0;
extern "C" unsigned char draw[64] = {};
extern "C" unsigned displayBuffer1StartAddress = 0;
extern "C" unsigned displayBuffer2StartAddress = 0;
extern "C" int vblankCount = 0;
extern "C" unsigned char vblankField = 0;
extern "C" int vblankSemaphore = 0;
extern "C" int vblankSemaphore2 = 0;

static unsigned addresses[8];
static unsigned long long values[8];
static int stores;
static int signals[4];
static int signal_count;
static int syncs;
static int eis;
static unsigned long long csr;

extern "C" void norrath_host_gs_store(unsigned address, unsigned long long value) {
    addresses[stores] = address;
    values[stores] = value;
    stores += 1;
}
extern "C" unsigned long long norrath_host_gs_load(unsigned address) {
    if (address != 0x12001000u) return 0;
    return csr;
}
extern "C" int iSignalSema(int id) {
    signals[signal_count++] = id;
    return 0;
}
extern "C" void norrath_host_sync() { syncs += 1; }
extern "C" void norrath_host_ei() { eis += 1; }

static void reset() {
    std::memset(draw, 0, sizeof draw);
    stores = 0;
    signal_count = 0;
    syncs = 0;
    eis = 0;
    vblankCount = 4;
    vblankField = 0;
    vblankSemaphore = 11;
    vblankSemaphore2 = 22;
    engineDisplayDisable = 0;
    specialMpegMode = 0;
    displayBuffer1StartAddress = 0x230000u;
    displayBuffer2StartAddress = 0x2bc000u;
    csr = 0;
}

static void put_word(int offset, unsigned value) {
    draw[offset] = static_cast<unsigned char>(value);
    draw[offset + 1] = static_cast<unsigned char>(value >> 8);
    draw[offset + 2] = static_cast<unsigned char>(value >> 16);
    draw[offset + 3] = static_cast<unsigned char>(value >> 24);
}
static unsigned word_at(int offset) {
    return static_cast<unsigned>(draw[offset] | (draw[offset + 1] << 8)
        | (draw[offset + 2] << 16) | (draw[offset + 3] << 24));
}

int main() {
    reset();
    engineDisplayDisable = 1;
    specialMpegMode = 1;
    csr = 1ull << 13;
    if (vblankHandler__Fi(99) != 0) return 1;
    if (stores != 1 || addresses[0] != 0x12000000u || values[0] != 0ull) return 2;
    if (vblankField != 1 || vblankCount != 5) return 3;
    if (signal_count != 2 || signals[0] != 11 || signals[1] != 22) return 4;
    if (syncs != 1 || eis != 1) return 5;

    reset();
    csr = 0;
    put_word(56, 10);
    put_word(36, 9);
    put_word(52, 12);
    put_word(28, 0);
    put_word(44, 1);
    if (vblankHandler__Fi(0) != 0) return 6;
    if (values[0] != 2ull || addresses[1] != 0x12000020u || values[1] != 1ull) return 7;
    if (word_at(56) != 12u || word_at(60) != 1u) return 8;
    const unsigned frame = (0x230000u >> 13) | 0x1400u;
    const unsigned long long marked = static_cast<unsigned long long>(frame) | (0x8000ull << 28);
    if (values[2] != marked || addresses[2] != 0x12000070u) return 9;
    if (values[3] != frame || addresses[3] != 0x12000090u) return 10;
    if (values[4] != 0x1be9ff0183227cull || values[5] != 0x1be9ff0183227cull) return 11;
    if (addresses[4] != 0x12000080u || addresses[5] != 0x120000a0u) return 12;

    reset();
    put_word(56, 0);
    put_word(36, 4);
    put_word(52, 0x7fffffff);
    put_word(28, 0);
    if (vblankHandler__Fi(0) != 0) return 13;
    if (stores != 3 || values[1] != 3ull) return 14;
    const unsigned second = (0x2bc000u >> 13) | 0x1400u;
    if (word_at(60) != 0u || values[2] != ((0x230000u >> 13) | 0x1400u)) return 15;
    (void)second;
    put_word(44, 0);
    put_word(52, 8);
    put_word(36, 1);
    stores = 0;
    if (vblankHandler__Fi(0) != 0) return 16;
    if (values[1] != 3ull || values[2] != second || word_at(60) != 1u) return 17;

    reset();
    put_word(56, 0);
    put_word(36, 10);
    put_word(52, 30);
    put_word(28, 0);
    put_word(44, 1);
    if (vblankHandler__Fi(0) != 0) return 18;
    if (word_at(56) != 10u || word_at(60) != 0u) return 19;
    std::puts("vblank handler contract passed");
    return 0;
}
