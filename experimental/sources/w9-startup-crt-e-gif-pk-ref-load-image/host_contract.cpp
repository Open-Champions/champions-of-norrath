#include <cstdio>
#include <cstring>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "pk_ref_load_image.cpp"
struct Event { char kind; unsigned a; unsigned long long b; unsigned c; };
static std::vector<Event> events;
static unsigned long long slot_storage[2 * 8];
static int slots_used;
extern "C" void* sceGifPkCnt(GifPacketState*, unsigned a1, unsigned a2, unsigned a3) {
    events.push_back({'C', a1, a2, a3}); return nullptr;
}
extern "C" void norrath_open_gif_tag(GifPacketState*, const unsigned long long* tag) {
    events.push_back({'O', 0, tag[0], static_cast<unsigned>(tag[1])});
}
extern "C" void sceGifPkAddGsAD(GifPacketState*, unsigned reg, unsigned long long value) { events.push_back({'A', reg, value, 0}); }
extern "C" void sceGifPkCloseGifTag(GifPacketState*) { events.push_back({'X', 0, 0, 0}); }
extern "C" void* sceGifPkReserve(GifPacketState*, int words) {
    events.push_back({'R', static_cast<unsigned>(words), 0, 0});
    return slot_storage + 2 * slots_used++;
}
extern "C" void sceGifPkRef(GifPacketState*, const void* data, unsigned quadwords, unsigned a3, unsigned a4, unsigned a5) {
    events.push_back({'F', static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(data)), quadwords, a3 | a4 | a5});
}
int main() {
    GifPacketState packet = {};
    // Header: tag constant and the four AD writes with the field positions.
    sceGifPkRefLoadImage(&packet, 0x11234, 0x1ff, 0x30005, nullptr, 0, 0x7ff, 0x3, 640, 448);
    if (events.size() != 7) return 1;
    if (events[0].kind != 'C' || events[0].a || events[0].b || events[0].c) return 2;
    if (events[1].kind != 'O' || events[1].b != 0x1000000000000000ull || events[1].c != 0x0e) return 3;
    const unsigned long long bitblt = (0x1234ull << 32) | (0x0005ull << 48) | (0xffull << 56);
    if (events[2].kind != 'A' || events[2].a != 0x50 || events[2].b != bitblt) return 4;
    if (events[3].a != 0x51 || events[3].b != ((0x7ffull << 32) | (3ull << 48))) return 5;
    if (events[4].a != 0x52 || events[4].b != (640ull | (448ull << 32))) return 6;
    if (events[5].a != 0x53 || events[5].b != 0 || events[6].kind != 'X') return 7;
    // Chunking: 70000 quadwords is 32767 + 32767 + 4466; EOP only on the last.
    events.clear();
    unsigned char* data = reinterpret_cast<unsigned char*>(0x10000000);
    sceGifPkRefLoadImage(&packet, 0, 10, 10, data, 70000, 0, 0, 640, 448);
    if (events.size() != 7 + 3 * 3) return 8;
    const unsigned counts[3] = {32767, 32767, 4466};
    unsigned offset = 0;
    for (int i = 0; i < 3; ++i) {
        const Event& cnt = events[7 + i * 3];
        const Event& rsv = events[8 + i * 3];
        const Event& ref = events[9 + i * 3];
        if (cnt.kind != 'C' || rsv.kind != 'R' || rsv.a != 4 || ref.kind != 'F') return 9;
        if (ref.b != counts[i] || ref.a != 0x10000000u + offset || ref.c != 0) return 10;
        const unsigned long long header = slot_storage[i * 2], second = slot_storage[i * 2 + 1];
        const unsigned long long expect = counts[i] | (2ull << 58) | (i == 2 ? 0x8000ull : 0);
        if (header != expect || second != 0) return 11;
        offset += counts[i] * 16u;
    }
    // One chunk exactly at the limit is also the last.
    events.clear(); slots_used = 0; std::memset(slot_storage, 0xaa, sizeof slot_storage);
    sceGifPkRefLoadImage(&packet, 0, 0, 0, data, 32767, 0, 0, 1, 1);
    if (events.size() != 10 || slot_storage[0] != (32767ull | (2ull << 58) | 0x8000ull)) return 12;
    events.clear(); slots_used = 0;
    sceGifPkRefLoadImage(&packet, 0, 0, 0, data, 32768, 0, 0, 1, 1);
    if (events.size() != 13 || slot_storage[0] != (32767ull | (2ull << 58)) || slot_storage[2] != (1ull | (2ull << 58) | 0x8000ull)) return 13;
    std::puts("sceGifPkRefLoadImage contract passed");
    return 0;
}
