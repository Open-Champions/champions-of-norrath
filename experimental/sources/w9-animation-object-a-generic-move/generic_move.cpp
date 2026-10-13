#include "../w9-animation-object-a-common/mem.hpp"
// Observed genericMove__10GameObjectG6Point3P10GameObject at 0x00141b70, 1508 bytes.
// Arguments: the object (self), a Point3 passed by value (a pointer to a copy on the
// EE) and an object to ignore (other). Result: a bit set, 1 = the world move reported a
// hit, 2 = a neighbour was pushed away, 4 = landed.
// Object words used: +8 pointer to the object's grid node (whose partner node at +8 is the
// one linked in a grid cell list), +12 flags, +16/+20/+24 position floats, +28 radius, +32 a
// height, +48 a vertical speed float, +72 the table pointer used for virtual calls.
//   1. The move is scaled by 1/60 per component. When its squared length exceeds 0.001,
//      objectMoveWithWorldCollision(self+16, move, radius, mode, 0) runs, with mode 1, or 3
//      when flag 0x20 is set, and bit 0x4 added when flag 0x40 is set. Result bit 1 is set
//      when it returns nonzero. The object's cell node is then re-linked (unlinked from its
//      current list, then pushed on the front of the head for its cell: ghostObjectGrid
//      when flag 0x800 is set, else objectGrid, cell index (gy*120+gx) with
//      g = (int(coord) + 15360) >> 8; outside the 120 by 120 grid it is offTheGrid).
//   2. Neighbours: objectFindInBox over x/y +- radius and z +- 1000, up to 64 results, ends
//      at a zero entry. top = z + height, best = -100000.0. For each result o, skipped when
//      o flag bit 0 is set, o == self or o == other, or sqrt(dx*dx+dy*dy) + 0.001 is not below
//      (self radius + o radius), the rest follows. "Platform" test: when either flag word has
//      0x10 set, o is skipped if top < o.z + 3.0 or o.z + o.height < self.z; else with
//      step = o.z + o.height - self.z below 5.0 and neither o flag 0x200 nor self flag 0x400,
//      best = max(best, o.z + o.height) and o is done; with step below 5.0 and one of those
//      two flags set, or with step 5.0 or more, handling continues as for non-platform.
//      Non-platform and continuing cases: when o flag 0x200 or self flag 0x400 is set, the
//      message step below runs. Otherwise, when |distance| > 0.001, the object is pushed
//      along (dx, dy) by ((sumRadii - distance) / distance) with a world move (mode 1),
//      re-linked, and result bit 2 is set; the message step then runs. A distance of 0.001
//      or less goes straight to the message step.
//      Message step: unless self flag 0x20000 is set, o's table slot at +48 is called with
//      (o, self); then self's slot at +48 is called with (self, o). A slot is a halfword
//      adjustment at +0 and a function at +4.
//   3. Ground: floor = worldFindStandHeight(world, copy of self position, radius), raised to
//      best when best is higher. speed = self[+48] - 9.0 (the constant 0x410fffff), stored;
//      z = z + speed * (1/60), stored; when z - 0.01 <= floor, z is set to floor and, if the
//      speed is negative, the speed becomes 0.0 and result bit 4 is set.
// Inferred: a generic physics step with gravity, support from platforms (flag 0x10), pushing
// between bodies and a collision message to both. Names are not claimed; the constants 9.0,
// 1/60 and 0.001 are exactly the ones in the image's pools.
// Unknown: the meaning of flag bits 0x1, 0x10, 0x20, 0x40, 0x200, 0x400, 0x800 and 0x20000
// beyond how they steer the code, the real name of slot +48, and the unused return of
// objectFindInBox.
extern "C" {
extern unsigned char objectGrid[];
extern unsigned char ghostObjectGrid[];
extern unsigned char* offTheGrid;
extern void* world;
int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float* position, const float* move, float radius, int mode, float* normal);
void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned char** out, int capacity);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const void* point, float radius);
}

namespace {
using namespace norrath_ao;

// Pool constants, written by bit pattern because the image's values are not always the
// nearest float to the decimal they read as: 1/60 (0x3c888888), 0.001 (0x3a83126e),
// 0.01 (0x3c23d70a) and 9.0 less one unit in the last place (0x410fffff).
const float pool_one_sixtieth = __builtin_bit_cast(float, 0x3c888888u);
const float pool_epsilon = __builtin_bit_cast(float, 0x3a83126eu);
const float pool_hundredth = __builtin_bit_cast(float, 0x3c23d70au);
const float pool_gravity = __builtin_bit_cast(float, 0x410fffffu);

__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}

// Moves the object's cell node to the front of the list for its current cell.
__attribute__((always_inline)) inline void relink(unsigned char* self) {
    unsigned char* node = load_pointer(load_pointer(self + 8) + 8);
    store_word(load_pointer(node + 4), load_word(node));
    if (load_word(node) != 0u) store_word(load_pointer(node) + 4, load_word(node + 4));
    const int cell_x = (static_cast<int>(load_float(self + 16)) + 15360) >> 8;
    const int cell_y = (static_cast<int>(load_float(self + 20)) + 15360) >> 8;
    unsigned char* head;
    if (static_cast<unsigned>(cell_x) < 120u && cell_y >= 0 && cell_y < 120) {
        head = ((load_word(self + 12) & 0x800u) != 0u ? ghostObjectGrid : objectGrid) + (cell_y * 120 + cell_x) * 4;
    } else {
        head = reinterpret_cast<unsigned char*>(&offTheGrid);
    }
    store_pointer(node + 4, head);
    store_word(node, load_word(head));
    store_pointer(head, node);
    if (load_word(node) != 0u) store_pointer(load_pointer(node) + 4, node);
}

// Calls the table slot at +48 of object `callee` with `argument`.
__attribute__((always_inline)) inline void send(unsigned char* callee, unsigned char* argument) {
    unsigned char* slot = load_pointer(callee + 72) + 48;
    typedef void (*Slot)(unsigned char*, unsigned char*);
    reinterpret_cast<Slot>(load_pointer(slot + 4))(callee + load_signed_half(slot), argument);
}
}

extern "C" int genericMove__10GameObjectG6Point3P10GameObject(unsigned char* self, const float* move_in, unsigned char* other) {
    int result = 0;
    float move[3];
    float best = -100000.0f;
    const float scale = pool_one_sixtieth;
    move[0] = move_in[0] * scale;
    move[1] = move_in[1] * scale;
    move[2] = move_in[2] * scale;
    if (pool_epsilon < move[0] * move[0] + move[1] * move[1] + move[2] * move[2]) {
        const unsigned flags = load_word(self + 12);
        int mode = (flags & 0x20u) != 0u ? 3 : 1;
        if ((flags & 0x40u) != 0u) mode |= 4;
        float* position = reinterpret_cast<float*>(self + 16);
        result = objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(position, move, load_float(self + 28), mode, nullptr) != 0;
        relink(self);
    }
    float low[3], high[3];
    unsigned char* found[64];
    {
        const float radius = load_float(self + 28);
        const float x = load_float(self + 16), y = load_float(self + 20), z = load_float(self + 24);
        low[0] = x - radius; low[1] = y - radius; low[2] = z - 1000.0f;
        high[0] = x + radius; high[1] = y + radius; high[2] = z + 1000.0f;
    }
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    if (found[0] != nullptr) {
        const float top = load_float(self + 24) + load_float(self + 32);
        for (int index = 0; found[index] != nullptr; ++index) {
            unsigned char* o = found[index];
            const unsigned o_flags = load_word(o + 12);
            if ((o_flags & 1u) != 0u || o == self || o == other) continue;
            const float dx = load_float(self + 16) - load_float(o + 16);
            const float dy = load_float(self + 20) - load_float(o + 20);
            const float distance = square_root(dx * dx + dy * dy);
            const float reach = load_float(self + 28) + load_float(o + 28);
            if (!(distance + pool_epsilon < reach)) continue;
            unsigned self_flags = load_word(self + 12);
            bool messages = false;
            bool handled = false;
            if (((o_flags | self_flags) & 0x10u) != 0u) {
                const float o_z = load_float(o + 24);
                if (top < o_z + 3.0f) continue;
                const float o_top = o_z + load_float(o + 32);
                if (o_top < load_float(self + 24)) continue;
                if (o_top - load_float(self + 24) < 5.0f) {
                    if ((o_flags & 0x200u) != 0u) {
                        messages = true;
                    } else if ((self_flags & 0x400u) != 0u) {
                        messages = true;
                    } else {
                        if (best < o_top) best = o_top;
                        continue;
                    }
                    handled = true;
                }
            }
            if (!handled) {
                if ((o_flags & 0x200u) != 0u || (self_flags & 0x400u) != 0u) {
                    messages = true;
                } else if (pool_epsilon < __builtin_fabsf(distance)) {
                    float push[3];
                    const float factor = (reach - distance) / distance;
                    push[0] = dx * factor;
                    push[1] = dy * factor;
                    push[2] = 0.0f;
                    objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(
                        reinterpret_cast<float*>(self + 16), push, load_float(self + 28), 1, nullptr);
                    relink(self);
                    result |= 2;
                    self_flags = load_word(self + 12);
                    messages = true;
                } else {
                    messages = true;
                }
            }
            if (messages) {
                if ((self_flags & 0x20000u) == 0u) send(o, self);
                send(self, o);
            }
        }
    }
    float point[3];
    point[0] = load_float(self + 16);
    point[1] = load_float(self + 20);
    point[2] = load_float(self + 24);
    float floor_height = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, point, load_float(self + 28));
    if (floor_height < best) floor_height = best;
    const float speed = load_float(self + 48) - pool_gravity;
    const float z = load_float(self + 24) + speed * pool_one_sixtieth;
    store_float(self + 48, speed);
    store_float(self + 24, z);
    if (z - pool_hundredth <= floor_height) {
        store_float(self + 24, floor_height);
        if (speed < 0.0f) {
            store_float(self + 48, 0.0f);
            result |= 4;
        }
    }
    return result;
}
