// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <vector>

struct Call { void* target; void* attacker; float a, b; int c, d, e, f; void* item; bool g; unsigned latch; };
static std::vector<Call> calls;
static unsigned char* current;
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* item, bool g) {
    unsigned latch;
    std::memcpy(&latch, current + 972, 4);
    calls.push_back({target, attacker, a, b, c, d, e, f, item, g, latch});
}
#include "msg_move_collision.cpp"

static void put(unsigned char* base, unsigned off, unsigned value) { std::memcpy(base + off, &value, 4); }
static void put16(unsigned char* base, unsigned off, unsigned value) { std::memcpy(base + off, &value, 2); }
int main() {
    int failures = 0;
    alignas(8) unsigned char object[1024];
    alignas(8) unsigned char other[4][64];
    const unsigned flags[4] = {0x20u, 0x1fu, 0xffffffffu, 0x20u};
    const int states[] = {20, 21, 22, 23, 24, -22};
    for (int state : states) {
        for (unsigned latch : {0u, 1u}) {
            for (unsigned other968 : {0u, 3u}) {
                for (unsigned count : {0u, 1u, 3u, 4u}) {
                    std::memset(object, 0x31, sizeof object);
                    put16(object, 572, static_cast<unsigned>(state) & 0xffffu);
                    put(object, 972, latch);
                    put(object, 968, other968);
                    put16(object, 200, 0xfff6u);  // -10
                    put16(object, 202, 25u);
                    unsigned words[6] = {};
                    for (unsigned i = 0; i < 4; ++i) {
                        std::memset(other[i], 0, sizeof other[i]);
                        put(other[i], 12, flags[i]);
                        rtti_c::host_note(other[i]);
                    }
                    for (unsigned i = 0; i < count; ++i) words[i] = static_cast<unsigned>(reinterpret_cast<unsigned long long>(other[i]));
                    current = object;
                    calls.clear();
                    msg_moveCollision__10ArenabeastPP10GameObject(object, reinterpret_cast<void**>(words));
                    const bool acts = (latch == 0 && other968 == 0 && state == 21) || state == 22 || state == 23;
                    std::vector<unsigned> expect;
                    if (acts) for (unsigned i = 0; i < count; ++i) if (flags[i] & 0x20u) expect.push_back(i);
                    if (calls.size() != expect.size()) { std::fprintf(stderr, "fail: state %d count %zu\n", state, calls.size()); ++failures; continue; }
                    for (size_t k = 0; k < calls.size(); ++k) {
                        const Call& c = calls[k];
                        if (c.target != other[expect[k]] || c.attacker != object || c.a != -10.0f || c.b != 25.0f ||
                            c.c != 100000 || c.d != 0 || c.e != 0 || c.f != 0x100041 || c.item != nullptr || c.g) {
                            std::fprintf(stderr, "fail: call arguments\n"); ++failures;
                        }
                        const unsigned want = k == 0 ? latch : 1u;
                        if (c.latch != want) { std::fprintf(stderr, "fail: latch is set after the call, not before\n"); ++failures; }
                    }
                    unsigned after;
                    std::memcpy(&after, object + 972, 4);
                    if (after != (expect.empty() ? latch : 1u)) { std::fprintf(stderr, "fail: latch value\n"); ++failures; }
                    if (object[967] != 0x31 || object[976] != 0x31) { std::fprintf(stderr, "fail: neighbours\n"); ++failures; }
                }
            }
        }
    }
    return failures ? 1 : 0;
}
