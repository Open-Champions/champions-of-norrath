// Host contract for genericMove__10GameObjectG6Point3P10GameObject. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include "../w9-animation-object-a-common/host_check.hpp"

// ---- host address map: 32-bit target words <-> host objects --------------------------------
static unsigned char arena[1 << 20];
extern "C" unsigned char objectGrid[57600];
extern "C" unsigned char ghostObjectGrid[57600];
extern "C" unsigned char* offTheGrid;
extern "C" void* world;
unsigned char objectGrid[57600];
unsigned char ghostObjectGrid[57600];
unsigned char* offTheGrid;
void* world = reinterpret_cast<void*>(0x4444);
static void slot_function(unsigned char* self, unsigned char* argument);
struct Segment { unsigned char* base; unsigned size; unsigned address; };
static Segment segments[] = {
    {arena, sizeof arena, 0x100000u},
    {objectGrid, sizeof objectGrid, 0x300000u},
    {ghostObjectGrid, sizeof ghostObjectGrid, 0x400000u},
    {reinterpret_cast<unsigned char*>(&offTheGrid), sizeof offTheGrid, 0x500000u},
    {reinterpret_cast<unsigned char*>(&slot_function), 16, 0x600000u},
};
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    for (const Segment& s : segments) if (address >= s.address && address - s.address < s.size) return s.base + (address - s.address);
    std::printf("bad target address %08x\n", address);
    std::abort();
}
extern "C" unsigned norrath_host_address(const unsigned char* pointer) {
    for (const Segment& s : segments) if (pointer >= s.base && pointer < s.base + s.size) return s.address + static_cast<unsigned>(pointer - s.base);
    std::printf("bad host pointer\n");
    std::abort();
}

// ---- mocks ----------------------------------------------------------------------------------
struct MoveCall { float position_before[3]; float move[3]; float radius; int mode; void* normal; };
static std::vector<MoveCall> move_calls;
static std::vector<int> move_returns;
static std::vector<std::vector<float>> move_new_positions;   // written into the position by each call
static std::vector<unsigned char*> found_objects;
static float find_low[3], find_high[3];
static int find_capacity, find_calls;
static std::vector<float> stand_points;
static float stand_radius, stand_result;
static void* stand_world;
static std::vector<std::pair<unsigned char*, unsigned char*>> slot_calls;
static std::vector<std::string> order;
extern "C" int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float* position, const float* move, float radius, int mode, float* normal) {
    MoveCall c;
    std::memcpy(c.position_before, position, 12); std::memcpy(c.move, move, 12);
    c.radius = radius; c.mode = mode; c.normal = normal;
    const size_t i = move_calls.size();
    move_calls.push_back(c);
    order.push_back("move");
    if (i < move_new_positions.size()) std::memcpy(position, move_new_positions[i].data(), 12);
    return i < move_returns.size() ? move_returns[i] : 0;
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned char** out, int capacity) {
    std::memcpy(find_low, low, 12); std::memcpy(find_high, high, 12); find_capacity = capacity; ++find_calls;
    size_t i = 0;
    for (; i < found_objects.size(); ++i) out[i] = found_objects[i];
    out[i] = nullptr;
    order.push_back("find");
}
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* w, const void* point, float radius) {
    const float* f = static_cast<const float*>(point);
    stand_points = {f[0], f[1], f[2]}; stand_radius = radius; stand_world = w;
    order.push_back("stand");
    return stand_result;
}
static void slot_function(unsigned char* self, unsigned char* argument) { slot_calls.push_back({self, argument}); order.push_back("slot"); }
#include "generic_move.cpp"

// ---- helpers --------------------------------------------------------------------------------
static int failures;
static void check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
static unsigned char* const vtable = arena + 0x8000;
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

// Object at arena+offset: grid node at +0x200.. of its own block, partner at +0x240.
struct Obj { unsigned char* p; unsigned char* node; unsigned char* partner; unsigned char* head_word; };
static Obj make_object(unsigned slot, unsigned flags, float x, float y, float z, float radius, float height) {
    unsigned char* p = arena + 0x1000 + 0x400 * slot;
    std::memset(p, 0, 0x400);
    sw_(p + 12, flags);
    sf_(p + 16, x); sf_(p + 20, y); sf_(p + 24, z); sf_(p + 28, radius); sf_(p + 32, height); sf_(p + 48, 0.0f);
    unsigned char* node = arena + 0x20000 + 0x100 * slot;
    unsigned char* partner = node + 0x40;
    unsigned char* head_word = arena + 0x28000 + 8 * slot;
    std::memset(node, 0, 0x80);
    sw_(p + 8, addr(node));
    sw_(node + 8, addr(partner));
    sw_(head_word, addr(partner));          // the partner is the first node of a private list
    sw_(partner, 0); sw_(partner + 4, addr(head_word));
    sw_(p + 72, addr(vtable));
    return {p, node, partner, head_word};
}
static unsigned char* cell_head(unsigned char* grid, int gx, int gy) { return grid + 4 * (gy * 120 + gx); }
static void reset_world() {
    std::memset(objectGrid, 0, sizeof objectGrid); std::memset(ghostObjectGrid, 0, sizeof ghostObjectGrid);
    offTheGrid = nullptr;
    std::memset(vtable, 0, 0x80);
    sh_(vtable + 48, 0); sw_(vtable + 52, 0x600000u);
    move_calls.clear(); move_returns.clear(); move_new_positions.clear(); found_objects.clear(); find_calls = 0;
    slot_calls.clear(); order.clear(); stand_result = -1000.0f;
}
static const float k_sixtieth = __builtin_bit_cast(float, 0x3c888888u);   // the image's pool value
int main() {
    float move[3];
    // 1. Tiny move: nothing is moved, the neighbour search runs, nothing is re-linked.
    {
        reset_world();
        Obj self = make_object(0, 0, 5, 6, 7, 2, 3);
        move[0] = 0.5f; move[1] = 0; move[2] = 0;      // 0.5/60 squared is far below 0.001
        stand_result = 0.0f;
        const int r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.empty(), "tiny move makes no world move");
        check(find_calls == 1 && find_capacity == 64, "box search made once with capacity 64");
        check(find_low[0] == 3 && find_low[1] == 4 && find_low[2] == 7 - 1000 && find_high[0] == 7 && find_high[1] == 8 && find_high[2] == 1007, "search box");
        check(stand_points == std::vector<float>({5, 6, 7}) && stand_radius == 2 && stand_world == world, "stand height query");
        check(r == 0 || r == 4, "result is only the landing bit");
        check(hw(self.partner + 4) == addr(self.head_word), "node not re-linked");
    }
    // 2. The move threshold: 0.001 on the squared scaled length (strict).
    {
        reset_world();
        Obj self = make_object(0, 0, 5, 6, 7, 2, 3);
        stand_result = 100.0f;
        move[0] = std::sqrt(0.001f) * 60.0f * 1.02f; move[1] = 0; move[2] = 0;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.size() == 1, "just above the threshold moves");
        reset_world();
        self = make_object(0, 0, 5, 6, 7, 2, 3);
        move[0] = std::sqrt(0.001f) * 60.0f * 0.98f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.empty(), "just below the threshold does not move");
    }
    // 3. Mode bits and the scaled vector; result bit 1 follows the return value; re-link in the grid.
    for (int variant = 0; variant < 4; ++variant) {
        reset_world();
        const unsigned flags = (variant & 1 ? 0x20u : 0u) | (variant & 2 ? 0x40u : 0u) | 0x800u;
        Obj self = make_object(0, flags, 5, 6, 7, 2, 3);
        move_returns = {variant == 3 ? 0 : 1};
        move_new_positions = {{300.0f, -300.0f, 7.0f}};      // cell: (300+15360)>>8 = 61, (-300+15360)>>8 = 58
        move[0] = 6; move[1] = 60; move[2] = -30;
        stand_result = 100.0f;
        const int r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        const int expected_mode = (variant & 1 ? 3 : 1) | (variant & 2 ? 4 : 0);
        check(move_calls.size() == 1 && move_calls[0].mode == expected_mode, "mode");
        check(move_calls[0].move[0] == 6 * k_sixtieth && move_calls[0].move[1] == 60 * k_sixtieth && move_calls[0].move[2] == -30 * k_sixtieth, "move scaled by 1/60");
        check(move_calls[0].radius == 2 && move_calls[0].normal == nullptr && move_calls[0].position_before[0] == 5, "move arguments");
        check(((r & 1) != 0) == (variant != 3), "result bit 1 follows the return value");
        check(hw(cell_head(ghostObjectGrid, 61, 58)) == addr(self.partner), "node pushed on the ghost grid cell");
        check(hw(self.partner + 4) == addr(cell_head(ghostObjectGrid, 61, 58)) && hw(self.partner) == 0, "node links");
        check(hw(self.head_word) == 0, "old list head now empty");
    }
    // 4. Cell selection: ordinary grid, off the grid on each edge, occupied cells get their first node patched.
    {
        struct Case { float x, y; bool on; int gx, gy; } cases[] = {
            {0, 0, true, 60, 60}, {-15360, -15360, true, 0, 0}, {15359, 15359, true, 119, 119}, {15360, 0, false, 0, 0},
            {-15361, 0, false, 0, 0}, {0, 15360, false, 0, 0}, {0, -15361, false, 0, 0}, {255.9f, 256.0f, true, 60, 61}};
        for (const Case& c : cases) {
            reset_world();
            Obj self = make_object(0, 0, 5, 6, 7, 2, 3);
            Obj resident = make_object(1, 0, 0, 0, 0, 1, 1);
            unsigned char* head = c.on ? cell_head(objectGrid, c.gx, c.gy) : reinterpret_cast<unsigned char*>(&offTheGrid);
            sw_(head, addr(resident.partner));
            sw_(resident.partner + 4, addr(head));
            sw_(resident.partner, 0);
            move_returns = {1};
            move_new_positions = {{c.x, c.y, 7.0f}};
            move[0] = 6; move[1] = 0; move[2] = 0;
            stand_result = 100.0f;
            genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
            check(hw(head) == addr(self.partner), "head takes the node");
            check(hw(self.partner) == addr(resident.partner) && hw(resident.partner + 4) == addr(self.partner), "old first is chained behind");
            check(hw(self.partner + 4) == addr(head), "previous word is the head address");
        }
    }
    // 5. Neighbour skipping: flag bit 0, self, and other are ignored; far neighbours are ignored.
    {
        reset_world();
        Obj self = make_object(0, 0, 0, 0, 0, 2, 3);
        Obj other = make_object(1, 0, 1, 0, 0, 2, 3);
        Obj dead = make_object(2, 1, 1, 0, 0, 2, 3);
        Obj far = make_object(3, 0, 10, 0, 0, 2, 3);
        found_objects = {self.p, other.p, dead.p, far.p};
        stand_result = 100.0f;
        move[0] = move[1] = move[2] = 0;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, other.p);
        check(slot_calls.empty(), "skipped neighbours send nothing");
    }
    // 6. A touching neighbour: push apart, then both slots (neighbour first), adjusted by the halfword at the slot.
    {
        reset_world();
        sh_(vtable + 48, 16);
        Obj self = make_object(0, 0, 0, 0, 0, 2, 3);
        Obj near = make_object(1, 0, 3, 0, 0, 2, 3);       // distance 3 < 4 - 0.001
        found_objects = {near.p};
        move_returns = {0, 1};
        move_new_positions = {{-1.0f, 0.0f, 0.0f}};
        stand_result = 100.0f;
        move[0] = move[1] = move[2] = 0;
        const int r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.size() == 1, "one push move");
        // dx = -3, dy = 0; factor = (4 - 3)/3.
        const float factor = (4.0f - 3.0f) / 3.0f;
        check(move_calls[0].move[0] == -3.0f * factor && move_calls[0].move[1] == 0.0f * factor && move_calls[0].move[2] == 0.0f, "push vector");
        check(move_calls[0].mode == 1 && move_calls[0].radius == 2, "push arguments");
        check((r & 2) != 0, "push sets bit 2");
        check(slot_calls.size() == 2 && slot_calls[0].first == near.p + 16 && slot_calls[0].second == self.p &&
              slot_calls[1].first == self.p + 16 && slot_calls[1].second == near.p, "slot order and adjustment");
        check(order == std::vector<std::string>({"find", "move", "slot", "slot", "stand"}), "call order");
        // Flag 0x20000 on the object: only its own slot runs.
        reset_world();
        self = make_object(0, 0x20000, 0, 0, 0, 2, 3);
        near = make_object(1, 0, 3, 0, 0, 2, 3);
        found_objects = {near.p};
        stand_result = 100.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.size() == 1 && slot_calls[0].first == self.p && slot_calls[0].second == near.p, "0x20000 skips the neighbour's slot");
    }
    // 7. Exactly overlapping centres: no push, the slots still run. Reach equality counts as apart.
    {
        reset_world();
        Obj self = make_object(0, 0, 0, 0, 0, 2, 3);
        Obj same = make_object(1, 0, 0, 0, 0, 2, 3);
        found_objects = {same.p};
        stand_result = 100.0f;
        move[0] = move[1] = move[2] = 0;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.empty() && slot_calls.size() == 2, "zero distance: no push, slots run");
        reset_world();
        self = make_object(0, 0, 0, 0, 0, 2, 3);
        Obj edge = make_object(1, 0, 4, 0, 0, 2, 3);      // distance + 0.001 is not below the reach 4
        found_objects = {edge.p};
        stand_result = 100.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.empty() && move_calls.empty(), "touching at the edge does nothing");
        // Within 0.001 of the reach is still apart: 3.9995 + 0.001 is not below 4.
        reset_world();
        self = make_object(0, 0, 0, 0, 0, 2, 3);
        edge = make_object(1, 0, 3.9995f, 0, 0, 2, 3);
        found_objects = {edge.p};
        stand_result = 100.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.empty() && move_calls.empty(), "inside the 0.001 margin is still apart");
        reset_world();
        self = make_object(0, 0, 0, 0, 0, 2, 3);
        edge = make_object(1, 0, 3.99f, 0, 0, 2, 3);
        found_objects = {edge.p};
        stand_result = 100.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.size() == 2 && move_calls.size() == 1, "beyond the margin touches");
    }
    // 8. Flags 0x200 (neighbour) or 0x400 (self): slots run without a push.
    for (int which = 0; which < 2; ++which) {
        reset_world();
        Obj self = make_object(0, which ? 0x400u : 0u, 0, 0, 0, 2, 3);
        Obj near = make_object(1, which ? 0u : 0x200u, 1, 0, 0, 2, 3);
        found_objects = {near.p};
        stand_result = 100.0f;
        move[0] = move[1] = move[2] = 0;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.empty() && slot_calls.size() == 2, "no push when flagged");
    }
    // 9. Platforms (flag 0x10): a low step raises the floor; no slots; the highest step wins; the
    //    z + 3.0 and top tests skip.
    {
        reset_world();
        Obj self = make_object(0, 0, 0, 0, 10, 2, 3);                  // top 13
        Obj a = make_object(1, 0x10, 1, 0, 8, 2, 3);                   // top 11, step 1: raises to 11
        Obj b = make_object(2, 0x10, 1, 0, 6, 2, 5);                   // top 11 again: not higher
        Obj c = make_object(3, 0x10, 1, 0, 8.5f, 2, 3);                // top 11.5: higher, step 1.5
        Obj d = make_object(4, 0x10, 1, 0, 16, 2, 3);                  // z + 3 = 19 > 13? no: 13 < 19 -> skipped
        Obj e = make_object(5, 0x10, 1, 0, 0, 2, 3);                   // top 3 < self z 10 -> skipped
        found_objects = {c.p, a.p, b.p, d.p, e.p};   // the highest first: later, lower tops must not lower it
        stand_result = 5.0f;
        move[0] = move[1] = move[2] = 0;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.empty() && move_calls.empty(), "platforms send nothing");
        check(lf_(self.p + 24) == 11.5f, "z snaps to the highest platform top");
    }
    {
        // Step of 5 or more is not a platform step: handled like an ordinary neighbour (push).
        reset_world();
        Obj self = make_object(0, 0, 0, 0, 10, 2, 3);
        Obj tall = make_object(1, 0x10, 1, 0, 5, 2, 10);               // top 15, step 5.0 exactly
        found_objects = {tall.p};
        stand_result = 5.0f;
        move[0] = move[1] = move[2] = 0;
        move_returns = {0};
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.size() == 1 && slot_calls.size() == 2, "step of 5.0 is pushed like an ordinary neighbour");
        // Platform with a step below 5 but a 0x200 flag on the neighbour: slots, no push.
        reset_world();
        self = make_object(0, 0, 0, 0, 10, 2, 3);
        tall = make_object(1, 0x10 | 0x200, 1, 0, 8, 2, 3);
        found_objects = {tall.p};
        stand_result = 5.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(move_calls.empty() && slot_calls.size() == 2, "platform step with 0x200: slots only");
        // Self flag 0x10 alone also selects the platform test.
        reset_world();
        self = make_object(0, 0x10, 0, 0, 10, 2, 3);
        Obj plain = make_object(1, 0, 1, 0, 8, 2, 3);
        found_objects = {plain.p};
        stand_result = 5.0f;
        genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(slot_calls.empty() && move_calls.empty() && lf_(self.p + 24) == 11.0f, "self flag 0x10 selects platform handling");
    }
    // 10. Ground and gravity.
    {
        reset_world();
        Obj self = make_object(0, 0, 0, 0, 10, 2, 3);
        sf_(self.p + 48, 0.0f);
        stand_result = 0.0f;
        move[0] = move[1] = move[2] = 0;
        int r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        const float speed = 0.0f - __builtin_bit_cast(float, 0x410fffffu);
        check(lf_(self.p + 48) == speed && lf_(self.p + 24) == 10.0f + speed * __builtin_bit_cast(float, 0x3c888888u) && r == 0, "falling: speed and z");
        // Landing: z - 0.01 <= floor snaps z, zeroes a negative speed, and sets bit 4.
        reset_world();
        self = make_object(0, 0, 0, 0, 10, 2, 3);
        sf_(self.p + 48, -30.0f);
        stand_result = 9.9f;
        r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(r == 4 && lf_(self.p + 24) == 9.9f && lf_(self.p + 48) == 0.0f, "landing");
        // Rising through the floor: z snaps but the speed is kept and no bit is set.
        reset_world();
        self = make_object(0, 0, 0, 0, 10, 2, 3);
        sf_(self.p + 48, 30.0f);
        stand_result = 11.0f;
        r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(r == 0 && lf_(self.p + 24) == 11.0f && lf_(self.p + 48) == 30.0f - __builtin_bit_cast(float, 0x410fffffu), "rising snaps z only");
        // Boundary: floor exactly z' - 0.01 lands (<=).
        reset_world();
        self = make_object(0, 0, 0, 0, 10, 2, 3);
        sf_(self.p + 48, 0.0f);
        const float new_z = 10.0f + (0.0f - __builtin_bit_cast(float, 0x410fffffu)) * __builtin_bit_cast(float, 0x3c888888u);
        stand_result = new_z - __builtin_bit_cast(float, 0x3c23d70au);
        r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(r == 4, "equality at 0.01 lands");
        reset_world();
        self = make_object(0, 0, 0, 0, 10, 2, 3);
        sf_(self.p + 48, 0.0f);
        stand_result = std::nextafter(new_z - __builtin_bit_cast(float, 0x3c23d70au), 0.0f);
        r = genericMove__10GameObjectG6Point3P10GameObject(self.p, move, nullptr);
        check(r == 0 && lf_(self.p + 24) == new_z, "just below the floor margin does not land");
    }
    if (failures) return 1;
    std::printf("genericMove check passed\n");
    return 0;
}
