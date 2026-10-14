// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp pad_get_dma_str.cpp
// The target stores a 32-bit pointer in the PadInfo word; the host maps the word 1..4 to one of four areas.
#include <cstdint>
#include <cstring>
extern "C" {
unsigned char norrath_libpad_port_info_view[240];
static int calls;
static void* seen_begin;
static void* seen_end;
void SyncDCache(void* b, void* e) { ++calls; seen_begin = b; seen_end = e; }
unsigned char* scePadGetDmaStr(int port, int slot);
}
alignas(16) static unsigned char area[4][256];
extern "C" unsigned char* norrath_host_area(unsigned word) { return word >= 1 && word <= 4 ? area[word - 1] : nullptr; }
static void set(int port, int slot, int a, int b) {
    std::memcpy(area[port * 2 + slot] + 88, &a, 4);
    std::memcpy(area[port * 2 + slot] + 216, &b, 4);
}
int main() {
    for (int port = 0; port < 2; ++port)
        for (int slot = 0; slot < 2; ++slot) {
            unsigned word = port * 2 + slot + 1;
            std::memcpy(norrath_libpad_port_info_view + port * 112 + slot * 28, &word, sizeof word);
        }
    for (int port = 0; port < 2; ++port)
        for (int slot = 0; slot < 2; ++slot) {
            unsigned char* a = area[port * 2 + slot];
            struct { int x, y, second; } cases[] = {{0, 1, 1}, {1, 0, 0}, {5, 5, 0}, {-1, 0, 1}, {0, -1, 0}, {-2147483647 - 1, 2147483647, 1}, {2147483647, -2147483647 - 1, 0}};
            for (auto& c : cases) {
                set(port, slot, c.x, c.y);
                calls = 0;
                unsigned char* r = scePadGetDmaStr(port, slot);
                if (calls != 1 || seen_begin != a || seen_end != a + 255) return 1;
                if (r != a + (c.second ? 128 : 0)) return 2;
            }
        }
    return 0;
}
