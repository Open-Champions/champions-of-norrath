// Host contract for gameRadialDamage__FP10GameObjectG6Point3iffiii; not an EE input. 32-bit words are offsets from a host arena. The
// box search is a stub that returns a fixed object list and logs its corners; the hurt entry logs object, amount, source and flags.
// The authored function was also compared with the original in a private R5900 run (306 scenarios, every original word but one nop covered;
// mutants of the flag mask, the byte test, the equality tests and the amount handling are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static unsigned char arena[0x4000] __attribute__((aligned(16)));
struct Hit { unsigned char* object; float amount; void* source; int flags; unsigned function; };
static std::vector<Hit> hits;
static Point3 got_low, got_high;
static int got_capacity;
static std::vector<unsigned> to_list;
void w14_projectile_a_host_hurt(unsigned fn, void* adjusted, float amount, void* source, int flags) {
    hits.push_back({static_cast<unsigned char*>(adjusted), amount, source, flags, fn});
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity) {
    got_low = low; got_high = high; got_capacity = capacity;
    for (size_t i = 0; i < to_list.size(); ++i) out[i] = to_list[i];
    out[to_list.size()] = 0;
}
#include "damage.cpp"

static unsigned char* object(int i) { return arena + 0x100 + 0x200 * i; }
static unsigned offset_of(void* p) { return static_cast<unsigned>(static_cast<unsigned char*>(p) - arena); }
static void make(int i, float x, float y, float radius, unsigned flags, signed char kind, short adjust) {
    using namespace w14pa;
    unsigned char* o = object(i);
    std::memset(o, 0, 0x200);
    real(o, 16) = x; real(o, 20) = y; real(o, 28) = radius;
    word(o, 12) = flags; o[172] = static_cast<unsigned char>(kind);
    unsigned char* table = o + 0x100;
    word(o, 72) = offset_of(table);
    *reinterpret_cast<short*>(table + 16) = adjust;
    word(table, 20) = 0xabc0u + i;
}
static float reference(float x, float y, float radius, Point3 c, float r1, float scale, int m, int a) {
    float ax = std::fabs(x - c.x), ay = std::fabs(y - c.y);
    float d = (ax + ay) - std::fmin(ax, ay) * w14pa::bits(0x3eaa7ef9u) - radius;
    if (r1 <= d) return -1.0f;
    float v = (r1 - d) * scale;
    if (v < static_cast<float>(m)) v = static_cast<float>(m);
    v = std::fmin(v, static_cast<float>(a));
    return static_cast<float>(static_cast<int>(v));
}
int main() {
    using namespace w14pa;
    w14_projectile_a_host_base = arena;
    Point3 c = {100.0f, 50.0f, 7.0f};
    for (int i = 0; i < 6; ++i) make(i, 0, 0, 0, 0, 0, 0);
    // 0: the source (skipped); 1: in range; 2: in range, flagged 0x20; 3: masked out; 4: out of range; 5: exactly at the edge
    make(0, 101, 50, 0, 0, 0, 0);
    make(1, 104, 53, 1.0f, 0x00, 0, 8);
    make(2, 97, 54, 0.5f, 0x20, 16, -8);
    make(3, 102, 52, 0, 0x40, 0, 0);
    make(4, 160, 50, 0, 0, 0, 0);
    make(5, 108, 50, 0, 0, 0, 0);  // distance exactly 8
    to_list = {offset_of(object(0)), offset_of(object(1)), offset_of(object(2)), offset_of(object(3)), offset_of(object(4)), offset_of(object(5))};
    unsigned char* source = object(0);
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 90, 5.0f, 8.0f, 0x1234, 3, 0x40);
    if (got_capacity != 128 || got_low.x != 92.0f || got_low.y != 42.0f || got_low.z != -1.0f || got_high.x != 108.0f || got_high.y != 58.0f || got_high.z != 15.0f) return 1;
    // span 3 > 1: scale 90/3 = 30
    float want1 = reference(104, 53, 1.0f, c, 8.0f, 30.0f, 3, 90);
    float want2 = reference(97, 54, 0.5f, c, 8.0f, 30.0f, 3, 90);
    if (hits.size() != 2) return 2;
    if (hits[0].object != object(1) + 8 || hits[0].amount != want1 || hits[0].source != source || hits[0].flags != 0x1234 || hits[0].function != 0xabc1u) return 3;
    if (hits[1].object != object(2) - 8 || hits[1].amount != want2 || hits[1].function != 0xabc2u) return 4;
    // the filter bit: unflagged objects are skipped, flagged ones only when the byte at +172 is 16
    hits.clear();
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 90, 5.0f, 8.0f, 0x08000000, 3, 0);
    if (hits.size() != 1 || hits[0].object != object(2) - 8) return 5;
    object(2)[172] = 15;
    hits.clear();
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 90, 5.0f, 8.0f, 0x08000000, 3, 0);
    if (!hits.empty()) return 6;
    // span 5 gives the scale 5 / 5 = 1.0; a span of exactly 1.0 keeps the default scale 100.0; the floor and the limit apply
    hits.clear();
    to_list = {offset_of(object(1))};
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 5, 5.0f, 6.0f + 4.0f, 0, 0, 0);
    if (hits.size() != 1 || hits[0].amount != reference(104, 53, 1.0f, c, 10.0f, 5.0f / 5.0f, 0, 5)) return 7;
    hits.clear();
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 5, 5.0f, 10.0f, 0, 4, 0);
    if (hits.size() != 1 || hits[0].amount != 4.0f) return 8;  // (10 - 5.001) * 1.0 truncates to 4; the floor 4 is not above it
    hits.clear();
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 77, 5.0f, 6.0f, 0, 0, 0);
    if (hits.size() != 1 || hits[0].amount != reference(104, 53, 1.0f, c, 6.0f, 100.0f, 0, 77)) return 9;
    // an empty list does nothing
    hits.clear(); to_list.clear();
    gameRadialDamage__FP10GameObjectG6Point3iffiii(source, c, 90, 5.0f, 8.0f, 0, 0, 0);
    return hits.empty() ? 0 : 10;
}
