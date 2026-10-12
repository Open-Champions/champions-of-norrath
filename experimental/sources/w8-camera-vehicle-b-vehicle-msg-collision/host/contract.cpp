// Host contract for msg_collision__7VehicleP10GameObject. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" unsigned msg_collision__7VehicleP10GameObject(void* vehicle, void* other);
static int calls = 0;
static void* seen[3];
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++calls;
    seen[0] = head; seen[1] = object_head; seen[2] = value;
    return reinterpret_cast<void*>(0x1234u);
}
static unsigned char vehicle[2300];
static unsigned char other[1400];
static void set(unsigned flags, unsigned marker) {
    std::memcpy(other + 12, &flags, 4);
    std::memcpy(other + 1328, &marker, 4);
    calls = 0;
}
int main() {
    // bit clear: always links, even with a marker
    set(0x0u, 7u);
    if (msg_collision__7VehicleP10GameObject(vehicle, other) != 0x1234u || calls != 1) { std::fprintf(stderr, "fail: bit clear\n"); return 1; }
    if (seen[0] != vehicle + 2136 || seen[1] != other || seen[2] != other) { std::fprintf(stderr, "fail: link args\n"); return 1; }
    // other bits set, bit 0x40 clear
    set(0xffffffbfu, 7u);
    if (msg_collision__7VehicleP10GameObject(vehicle, other) != 0x1234u || calls != 1) { std::fprintf(stderr, "fail: other bits\n"); return 1; }
    // bit set, marker zero: links
    set(0x40u, 0u);
    if (msg_collision__7VehicleP10GameObject(vehicle, other) != 0x1234u || calls != 1) { std::fprintf(stderr, "fail: bit set, no marker\n"); return 1; }
    // bit set, marker nonzero: returns marker, no call
    set(0x40u, 0xdeadbeefu);
    if (msg_collision__7VehicleP10GameObject(vehicle, other) != 0xdeadbeefu || calls != 0) { std::fprintf(stderr, "fail: bit set, marker\n"); return 1; }
    return 0;
}
