// Host contract for addSmokelet__10StaticFire; not an EE input. Pointer words are offsets from a host base.
// The expectation recomputes the record from the same scripted rolls with a separately written model.
#define W11_ACTOR_C_HOST 1
#include "add_smokelet.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
static std::vector<int> rolls;
static size_t used;
extern "C" unsigned eRand__10GameObject(void*) { return rolls.at(used++); }
static float comp(int r) { double d = r * (1.0 / 65536.0); return static_cast<float>(d * 2.0 - 1.0); }
int main() {
    static unsigned char base[4096];
    w11_actor_c_host_base = base;
    int failures = 0;
    struct Case { int cap, head, tail; float a, b; std::vector<int> rolls; };
    // 32768 -> 0.0 per component; 65536 -> +1; accepted when the squared length is at most 1
    const Case cases[] = {
        {6, 0, 0, 1.0f, 2.0f, {32768, 32768, 32768, 40000, 20000}},
        {6, 5, 1, 1.5f, 1.0f, {65536, 65536, 32768, 32768, 32768, 40000, 50000, 10000}},          // rejected once (2 > 1)
        {6, 5, 0, 1.0f, 1.0f, {32768, 32768, 32768}},                                                             // full: next wraps onto tail
        {6, 2, 3, 1.0f, 1.0f, {32768, 32768, 32768}},                                                             // full without wrap
        {4, 3, 1, 3.0f, 0.5f, {16384, 49152, 16384, 123456, 7}},                                          // negative z -> abs
        {4, 1, 0, 1000.0f, 1.0f, {50000, 40000, 30000, 100000, 3000}},                                // large values wrap in 16 bits
    };
    for (const Case& c : cases) {
        std::memset(base, 0, sizeof base);
        unsigned char* obj = base + 64;
        short cap = static_cast<short>(c.cap), head = static_cast<short>(c.head), tail = static_cast<short>(c.tail);
        std::memcpy(obj + 118, &cap, 2); std::memcpy(obj + 120, &head, 2); std::memcpy(obj + 122, &tail, 2);
        std::memcpy(obj + 28, &c.a, 4); std::memcpy(obj + 100, &c.b, 4);
        *reinterpret_cast<unsigned*>(obj + 1188) = 2048;
        std::memset(base + 2048, 0x5a, 80);
        rolls = c.rolls; used = 0;
        // model
        size_t k = 0;
        float v[3];
        for (;;) {
            for (int i = 0; i < 3; ++i) v[i] = comp(c.rolls[k++]);
            if (!(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] > 1.0f)) break;
        }
        const double s = static_cast<double>(c.a * c.b);
        int next = (head + 1 == cap) ? 0 : head + 1;
        unsigned char want[80]; std::memset(want, 0x5a, 80);
        short wantHead = head;
        if (next != tail) {
            double z = v[2] * s; if (z < 0) z = -z;
            short f[5] = {static_cast<short>(static_cast<int>(v[0] * s * 16.0)), static_cast<short>(static_cast<int>(v[1] * s * 16.0)),
                          static_cast<short>(static_cast<int>(z * 16.0)),
                          static_cast<short>(static_cast<int>((c.rolls[k] / 65536.0 - 0.5) * (1.0f / 60.0f) * 224.0)),
                          static_cast<short>(static_cast<int>((c.rolls[k + 1] / 65536.0 - 0.5) * (1.0f / 60.0f) * 224.0))};
            std::memcpy(want + head * 10, f, 10);
            wantHead = static_cast<short>(next);
            k += 2;
        }
        addSmokelet__10StaticFire(obj);
        if (used != k) { std::fprintf(stderr, "draws %zu want %zu\n", used, k); ++failures; }
        short got; std::memcpy(&got, obj + 120, 2);
        if (got != wantHead) { std::fprintf(stderr, "head %d want %d\n", got, wantHead); ++failures; }
        if (std::memcmp(base + 2048, want, 80) != 0) {
            std::fprintf(stderr, "record bytes differ (cap=%d head=%d tail=%d)\n", c.cap, c.head, c.tail);
            for (int i = 0; i < 10; ++i) std::fprintf(stderr, " %02x/%02x", base[2048 + head * 10 + i], want[head * 10 + i]);
            std::fputc('\n', stderr); ++failures;
        }
    }
    if (!failures) std::puts("addSmokelet ok");
    return failures != 0;
}
