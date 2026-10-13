// Host contract for updateSmokelets__10StaticFire; not an EE input. Pointer words are offsets from a host
// base. The expectation steps a model of the ring buffer written separately from the source.
#define W11_ACTOR_C_HOST 1
#include "update_smokelets.cpp"
#include <cstdio>
#include <cstring>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
int main() {
    static unsigned char base[4096];
    w11_actor_c_host_base = base;
    int failures = 0;
    struct Case { int cap, head, tail; float limit; };
    const Case cases[] = {{6, 0, 0, 50.0f}, {6, 4, 1, 50.0f}, {6, 2, 4, 50.0f}, {6, 5, 0, 1.0f}, {6, 1, 5, 20.0f},
                          {4, 3, 2, 70000.0f}, {4, 0, 3, -70000.0f}, {5, 3, 0, 3.9f}, {5, 4, 4, 0.0f}};
    for (const Case& c : cases) {
        std::memset(base, 0, sizeof base);
        unsigned char* obj = base + 64;
        short cap = static_cast<short>(c.cap), head = static_cast<short>(c.head), tail = static_cast<short>(c.tail);
        std::memcpy(obj + 118, &cap, 2); std::memcpy(obj + 120, &head, 2); std::memcpy(obj + 122, &tail, 2);
        std::memcpy(obj + 88, &c.limit, 4);
        *reinterpret_cast<unsigned*>(obj + 1188) = 2048;
        short rec[6][5];
        for (int i = 0; i < 6; ++i) {
            short v[5] = {static_cast<short>(100 * i), static_cast<short>(-50 * i), static_cast<short>(i * 3 + (i == 3 ? 32760 : 0) - 10), static_cast<short>(7 * i + 1), static_cast<short>(-3 * i)};
            std::memcpy(rec[i], v, 10); std::memcpy(base + 2048 + 10 * i, v, 10);
        }
        updateSmokelets__10StaticFire(obj);
        // model
        int lim = static_cast<short>(static_cast<int>(c.limit));
        int t = tail;
        short mtail = tail;
        int i = tail;
        while (i != head) {
            unsigned short a = static_cast<unsigned short>(rec[i][0] + rec[i][3]);
            unsigned short b = static_cast<unsigned short>(rec[i][1] + rec[i][4]);
            unsigned short d = static_cast<unsigned short>(rec[i][2] + 7);
            rec[i][0] = static_cast<short>(a); rec[i][1] = static_cast<short>(b); rec[i][2] = static_cast<short>(d);
            if (!(static_cast<short>(d) < lim)) { mtail = static_cast<short>(mtail + 1); if (mtail == cap) mtail = 0; }
            i = i + 1 == cap ? 0 : i + 1;
        }
        (void)t;
        if (std::memcmp(rec, base + 2048, 60) != 0) { std::fprintf(stderr, "records differ for case cap=%d head=%d tail=%d\n", c.cap, c.head, c.tail); ++failures; }
        short got; std::memcpy(&got, obj + 122, 2);
        if (got != mtail) { std::fprintf(stderr, "tail %d want %d (cap=%d head=%d tail=%d)\n", got, mtail, c.cap, c.head, c.tail); ++failures; }
        std::memcpy(&got, obj + 120, 2); if (got != head) ++failures;
        std::memcpy(&got, obj + 118, 2); if (got != cap) ++failures;
    }
    if (!failures) std::puts("updateSmokelets ok");
    return failures != 0;
}
