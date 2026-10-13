// Host contract for startForces__7Vehicle. Not an EE plan input.
#include "start_forces.cpp"

#include <cstdio>
#include <cstring>

int main() {
    int failures = 0;
    unsigned char o[2304], before[2304];
    std::memset(o, 0x6b, sizeof o);
    std::memcpy(before, o, sizeof o);
    startForces__7Vehicle(o);
    for (int i = 0; i < (int)sizeof o; ++i) {
        const bool zeroed = i >= 1992 && i < 2016;
        const unsigned char want = zeroed ? 0 : before[i];
        if (o[i] != want) { std::fprintf(stderr, "fail: byte %d\n", i); ++failures; }
    }
    return failures ? 1 : 0;
}
