// Host contract for start__3WebG6Point3T1i; not an EE input. Includes the source.
#include "start.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char w[96];
    int bad = 0;
    std::memset(w, 0x77, sizeof w);
    int base = 1000;
    std::memcpy(w + 48, &base, 4);
    start__3WebG6Point3T1i(w, Point3{1.5f, -2.25f, 3.0f}, Point3{10.0f, 20.0f, -30.5f}, 25);
    unsigned char want[96];
    std::memset(want, 0x77, sizeof want);
    const float p[6] = {1.5f, -2.25f, 3.0f, 10.0f, 20.0f, -30.5f};
    std::memcpy(want + 16, p, 12); std::memcpy(want + 32, p + 3, 12);
    const int words[][2] = {{8, 40}, {0, 1}, {44, 1025}, {56, 0}, {12, 0}, {48, 1000}};
    for (auto& e : words) std::memcpy(want + e[0], &e[1], 4);
    if (std::memcmp(w, want, sizeof w) != 0) { std::puts("fail: stored fields"); bad = 1; }
    return bad;
}
