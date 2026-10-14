// Host contract for updateAnimation__FP13animComponent. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "update_animation.cpp"
unsigned char* fsList;
unsigned char* fsListBack;
static unsigned char* free_state;
static int free_calls;
static float yaw_args[2];
extern "C" unsigned char* getFreeFrameState__Fv() { ++free_calls; return free_state; }
extern "C" int iatan2__Fff(float y, float x) { yaw_args[0] = y; yaw_args[1] = x; return 0x12345; }

// Independent bit writer: bits are appended one at a time, most significant first.
struct Bits {
    std::vector<bool> bits;
    void put(unsigned value, unsigned width) { for (int i = int(width) - 1; i >= 0; --i) bits.push_back((value >> i) & 1u); }
    void put_signed(int value, unsigned width) { put(static_cast<unsigned>(value) & ((1u << width) - 1u), width); }
    void store(unsigned char* out, unsigned halfwords) {
        for (unsigned h = 0; h != halfwords; ++h) {
            unsigned v = 0;
            for (unsigned b = 0; b != 16; ++b) { const unsigned i = h * 16 + b; v = (v << 1) | (i < bits.size() && bits[i] ? 1u : 0u); }
            sh_(out + h * 2, v);
        }
    }
};
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* comp = arena_at(0x100);
    unsigned char* header = arena_at(0x200);
    unsigned char* stream = arena_at(0x400);
    unsigned char* state = arena_at(0x1000);
    unsigned char* other = arena_at(0x2000);
    unsigned char* tail = arena_at(0x3000);
    std::memset(arena_at(0), 0, 0x4000);
    // Stream: two tracks then events.
    Bits b;
    // track 0: width 5, three values, width2 6, four values (order: v2 v3 v4 v5)
    b.put(4, 4); b.put_signed(3, 5); b.put_signed(-4, 5); b.put_signed(7, 5);
    b.put(5, 4); b.put_signed(10, 6); b.put_signed(-20, 6); b.put_signed(30, 6); b.put_signed(-30, 6);
    // track 1: width 3, width2 4
    b.put(2, 4); b.put_signed(1, 3); b.put_signed(-1, 3); b.put_signed(2, 3);
    b.put(3, 4); b.put_signed(1, 4); b.put_signed(2, 4); b.put_signed(-3, 4); b.put_signed(-2, 4);
    const unsigned events_start = unsigned(b.bits.size());
    // event 1: delta 10, flag 1 (position), track 1, width 4, x 3 y -2 z 1
    b.put(10, 8); b.put(1, 1); b.put(1, 6); b.put(3, 4); b.put_signed(3, 4); b.put_signed(-2, 4); b.put_signed(1, 4);
    const unsigned after_event1 = unsigned(b.bits.size());
    // event 2: delta 5, flag 0 (rotation rate), track 0, width 5, a b c d = 4 -4 3 -3
    b.put(5, 8); b.put(0, 1); b.put(0, 6); b.put(4, 4); b.put_signed(4, 5); b.put_signed(-4, 5); b.put_signed(3, 5); b.put_signed(-3, 5);
    const unsigned after_event2 = unsigned(b.bits.size());
    // event 3: delta 100, flag 1, track 1 (never reached in this test)
    b.put(100, 8); b.put(1, 1); b.put(1, 6); b.put(0, 4); b.put_signed(0, 1); b.put_signed(0, 1); b.put_signed(0, 1);
    b.store(stream, 64);
    sw_(header, 2); sw_(header + 4, 500); sw_(header + 8, norrath_host_address(stream));
    sw_(comp, norrath_host_address(header)); sh_(comp + 28, 0);
    // List: state (head) -> other -> tail, so a fresh state is linked in front.
    sw_(state + 8, 1000000); sw_(state + 12, norrath_host_address(other));
    sw_(other + 16, norrath_host_address(state)); sw_(other + 12, norrath_host_address(tail)); sw_(tail + 16, norrath_host_address(other));
    fsList = state; fsListBack = tail;
    free_state = state;
    sf_(comp + 24, 12.5f);
    updateAnimation__FP13animComponent(comp);
    expect(free_calls == 1 && hw(comp + 4) == norrath_host_address(state) && hw(state + 20) == norrath_host_address(comp), "a null state is replaced and linked both ways");
    // Track records: base state + 28 + 60 * i.
    unsigned char* t0 = state + 28; unsigned char* t1 = state + 28 + 60;
    expect(lf_(t0 + 4) == 3.0f / 64 && lf_(t0 + 8) == -4.0f / 64 && lf_(t0 + 12) == 7.0f / 64, "track 0 position values scaled by 2^-6");
    expect(lf_(t0 + 40) == 10.0f / 4096 && lf_(t0 + 28) == -20.0f / 4096 && lf_(t0 + 32) == 30.0f / 4096 && lf_(t0 + 36) == -30.0f / 4096, "track 0 rotation floats: first value to +40, then +28, +32, +36");
    expect(lf_(t1 + 4) == 1.0f / 64 && lf_(t1 + 8) == -1.0f / 64 && lf_(t1 + 12) == 2.0f / 64, "track 1 position values");
    expect(lf_(t1 + 40) == 1.0f / 4096 && lf_(t1 + 28) == 2.0f / 4096 && lf_(t1 + 32) == -3.0f / 4096 && lf_(t1 + 36) == -2.0f / 4096, "track 1 rotation floats");
    expect(lf_(t0 + 16) == 0 && lf_(t0 + 44) == 0 && lf_(t1 + 56) == 0 && lf_(t1 + 52) == 0, "rates start at zero");
    // Event 1 applies at frame 10 (target 12): track 1 velocity; event 2 is at frame 15, not applied.
    expect(lf_(t1 + 16) == 3.0f / 256 && lf_(t1 + 20) == -2.0f / 256 && lf_(t1 + 24) == 1.0f / 256 && hw(t1) == 10u, "event 1: velocity of track 1 set and its frame recorded");
    expect(hw(state + 8) == 10 && hw(state + 4) == after_event1, "state frame 10; bit position just after event 1 (event 2's delta was only peeked)");
    expect(hw(state + 24) == 0x2345, "yaw halfword from iatan2");
    {
        const float x = -20.0f / 4096, y = 30.0f / 4096, z = -30.0f / 4096, w = 10.0f / 4096;
        expect(yaw_args[0] == (x * y + z * w) + (x * y + z * w) && yaw_args[1] == ((x * x - y * y) - z * z) + w * w, "yaw arguments from track 0's rotation (x y z = +28 +32 +36, w = +40)");
    }
    // Second call, later target: no reset; event 2 applies at frame 15 with elapsed = 15 - 0.
    free_calls = 0;
    sf_(comp + 24, 20.0f);
    updateAnimation__FP13animComponent(comp);
    expect(free_calls == 0, "existing state kept");
    expect(lf_(t0 + 56) == 4.0f / 131072 && lf_(t0 + 44) == -4.0f / 131072 && lf_(t0 + 48) == 3.0f / 131072 && lf_(t0 + 52) == -3.0f / 131072, "event 2 sets rotation rates: +56 first, then +44, +48, +52 (a,b,c,d -> 56,44,48,52)");
    expect(hw(state + 8) == 15 && hw(state + 4) == after_event2 && (hw(t0) >> 16) == 15, "state frame 15 and halfword +2 of track 0 set");
    // Third call: target below the state's frame rebuilds from the start; the event at frame 10 is not reached at target 5.
    sf_(comp + 24, 5.0f);
    updateAnimation__FP13animComponent(comp);
    expect(lf_(t1 + 16) == 0 && lf_(t0 + 44) == 0 && hw(state + 8) == 0 && hw(state + 4) == events_start, "rebuild: rates cleared, frame 0, bit position at the first event");
    // Flag 0x800 skips track 0's planar position (x, y) but keeps its third value.
    sf_(t0 + 4, 123.0f); sf_(t0 + 8, 456.0f);
    sh_(comp + 28, 0x800); sf_(comp + 24, 1.0f); sw_(state + 8, 1000);
    updateAnimation__FP13animComponent(comp);
    expect(lf_(t0 + 4) == 123.0f && lf_(t0 + 8) == 456.0f && lf_(t0 + 12) == 7.0f / 64, "flag 0x800: track 0 x and y kept, z rebuilt");
    expect(lf_(t1 + 4) == 1.0f / 64, "flag 0x800 does not affect track 1");
    // Position integration: velocity of track 1 is integrated by (frame - h0) at the next position event.
    sh_(comp + 28, 0); sw_(state + 8, 1000); sf_(comp + 24, 12.0f);
    updateAnimation__FP13animComponent(comp);   // event 1 at frame 10: h0(track 1) = 10, velocity (3,-2,1)/256
    Bits c2;   // build a fresh stream with two position events on the same track to check integration
    c2.put(4, 4); c2.put_signed(0, 5); c2.put_signed(0, 5); c2.put_signed(0, 5); c2.put(0, 4); c2.put_signed(0, 1); c2.put_signed(0, 1); c2.put_signed(0, 1); c2.put_signed(0, 1);
    c2.put(10, 8); c2.put(1, 1); c2.put(0, 6); c2.put(3, 4); c2.put_signed(7, 4); c2.put_signed(-7, 4); c2.put_signed(4, 4);   // frame 10, track 0
    c2.put(6, 8); c2.put(1, 1); c2.put(0, 6); c2.put(3, 4); c2.put_signed(0, 4); c2.put_signed(0, 4); c2.put_signed(0, 4);     // frame 16: integrates for 6
    c2.put(255, 8);   // terminator: the next event is far beyond the target
    c2.store(stream, 64);
    sw_(header, 1); sw_(state + 8, 1000); sf_(comp + 24, 16.0f);
    updateAnimation__FP13animComponent(comp);
    expect(lf_(t0 + 4) == 0.0f + (7.0f / 256) * 6.0f && lf_(t0 + 8) == 0.0f + (-7.0f / 256) * 6.0f && lf_(t0 + 12) == 0.0f + (4.0f / 256) * 6.0f && (hw(t0) & 0xffff) == 16u && (hw(t0) >> 16) == 0u,
           "position grows by velocity * (frame - previous frame)");
    // Move to front: a state with a newer neighbour moves to the head of the list.
    std::memset(arena_at(0), 0, 0x4000);
    sw_(header, 0); sw_(header + 4, 10); sw_(header + 8, norrath_host_address(stream)); sw_(comp, norrath_host_address(header));
    Bits e; e.put(255, 8); e.store(stream, 8);
    unsigned char* a = arena_at(0x1000); unsigned char* m = arena_at(0x2000); unsigned char* z = arena_at(0x3000);   // a newest, z oldest
    sw_(a + 12, norrath_host_address(m)); sw_(m + 16, norrath_host_address(a)); sw_(m + 12, norrath_host_address(z)); sw_(z + 16, norrath_host_address(m));
    sw_(a + 8, 0); sw_(m + 8, 0); sw_(z + 8, 0);
    sw_(a, norrath_host_address(stream)); sw_(m, norrath_host_address(stream)); sw_(z, norrath_host_address(stream));
    fsList = a; fsListBack = z;
    sw_(comp + 4, norrath_host_address(m)); sf_(comp + 24, 1.0f);
    updateAnimation__FP13animComponent(comp);
    expect(fsList == m && hw(m + 16) == 0 && hw(m + 12) == norrath_host_address(a) && hw(a + 16) == norrath_host_address(m) && hw(a + 12) == norrath_host_address(z) && hw(z + 16) == norrath_host_address(a) && fsListBack == z,
           "middle state moves to the head");
    sw_(comp + 4, norrath_host_address(z));
    updateAnimation__FP13animComponent(comp);
    expect(fsList == z && fsListBack == a && hw(a + 12) == 0 && hw(z + 12) == norrath_host_address(m) && hw(m + 16) == norrath_host_address(z) && hw(z + 16) == 0, "the oldest state moves to the head and its newer neighbour becomes the tail");
    updateAnimation__FP13animComponent(comp);
    expect(fsList == z && hw(z + 16) == 0, "the head stays where it is");
    if (failures) return 1;
    std::printf("updateAnimation check passed\n");
    return 0;
}
