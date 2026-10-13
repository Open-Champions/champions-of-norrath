// Host comparison only. Not an EE build input. Callees are scripted; the 32-bit request object
// word is mapped to a host buffer through the two host-contract functions.
#include <cstdio>
#include <cstring>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
extern "C" {
alignas(8) unsigned char norrath_libpad_port_info_view[0x200];
alignas(4) unsigned char norrath_libpad_is_warning_word[4];
}
static std::vector<int> events;
static int stat_result;
static unsigned set_dma_result;
static unsigned packet_seen[4];
static unsigned char* sync_begin;
static bool sync_range_ok;
static unsigned char request_storage[64];
extern "C" {
int scePrintf(const char*, ...) { events.push_back(3); return 0; }
int sceSifDmaStat(unsigned id) { events.push_back(1000 + static_cast<int>(id)); return stat_result; }
unsigned sceSifSetDma(void* packet, int count) {
    events.push_back(2);
    std::memcpy(packet_seen, packet, 16);
    return count == 1 ? set_dma_result : 99;
}
void SyncDCache(void* begin, void* end) {
    events.push_back(4);
    sync_begin = static_cast<unsigned char*>(begin);
    sync_range_ok = static_cast<unsigned char*>(end) == sync_begin + 31;
}
unsigned char* norrath_host_request_object(unsigned word) { return word == 0x1000 ? request_storage : nullptr; }
unsigned norrath_host_object_word(const unsigned char* object) { return object == request_storage ? 0x1000 : 0; }
}
#include "send_to_iop.cpp"
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
static void put(unsigned char* at, unsigned v) { std::memcpy(at, &v, 4); }
static unsigned get(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static unsigned char* entry(int port, int slot) { return norrath_libpad_port_info_view + slot * 28 + port * 112; }
static void reset(int port, int slot, unsigned pending, unsigned count, bool warning) {
    std::memset(norrath_libpad_port_info_view, 0x11, sizeof norrath_libpad_port_info_view);
    std::memset(request_storage, 0x22, sizeof request_storage);
    put(entry(port, slot) + 4, 0x1000);
    put(entry(port, slot) + 8, 0x20000);
    put(entry(port, slot) + 12, pending);
    put(request_storage, count);
    put(norrath_libpad_is_warning_word, warning ? 1 : 0);
    events.clear(); sync_begin = nullptr;
}
int main() {
    // No pending transfer, even count -> odd after increment: second half.
    reset(1, 2, 0, 4, true);
    set_dma_result = 7;
    check(_send_to_iop(1, 2) == 1, "returns 1");
    check(events == (std::vector<int>{4, 2}), "no stat call without pending id; sync then set-dma");
    check(get(request_storage) == 5 && sync_begin == request_storage && sync_range_ok, "count incremented, sync range");
    check(packet_seen[0] == 0x1000 && packet_seen[1] == 0x20020 && packet_seen[2] == 32 && packet_seen[3] == 0, "packet words (odd count)");
    check(get(entry(1, 2) + 12) == 7, "new id stored");
    // Odd count -> even: first half.
    reset(0, 0, 0, 5, false);
    set_dma_result = 9;
    check(_send_to_iop(0, 0) == 1 && packet_seen[1] == 0x20000 && get(request_storage) == 6, "even new count uses first half");
    // Pending id finished (negative stat): sends.
    reset(1, 3, 12, 0, true);
    stat_result = -1; set_dma_result = 3;
    check(_send_to_iop(1, 3) == 1 && events == (std::vector<int>{1012, 4, 2}) && get(entry(1, 3) + 12) == 3, "stat negative sends");
    // Pending id still running: no send, prints when warning.
    reset(1, 3, 12, 0, true);
    stat_result = 0;
    check(_send_to_iop(1, 3) == 0 && events == (std::vector<int>{1012, 3}) && get(request_storage) == 0 && get(entry(1, 3) + 12) == 12, "stat nonnegative skips and warns");
    reset(1, 3, 12, 0, false);
    stat_result = 5;
    check(_send_to_iop(1, 3) == 0 && events == (std::vector<int>{1012}), "silent without warning flag");
    // SetDma failure.
    reset(0, 1, 0, 8, true);
    set_dma_result = 0;
    check(_send_to_iop(0, 1) == 0 && events == (std::vector<int>{4, 2, 3}) && get(entry(0, 1) + 12) == 0 && get(request_storage) == 9, "set-dma failure warns, keeps id");
    reset(0, 1, 0, 8, false);
    check(_send_to_iop(0, 1) == 0 && events == (std::vector<int>{4, 2}), "set-dma failure silent");
    if (failures) return 2;
    std::puts("_send_to_iop cases passed");
    return 0;
}
