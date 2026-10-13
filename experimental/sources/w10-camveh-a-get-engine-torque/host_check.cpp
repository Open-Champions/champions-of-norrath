// Host contract for getEngineTorque__7Vehicle. Not an EE plan input.
#include "get_engine_torque.cpp"

#include <cstdio>
#include <cstring>

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }

int main() {
    int failures = 0;
    // The host pointer is 8 bytes; keep the table pointer in a union-free way
    // by placing the vehicle so that the stored 8-byte value is the address.
    static float table[2 + 10];
    for (int i = 0; i < 10; ++i) table[2 + i] = 100.0f * (i + 1);  // 100, 200, ... 1000
    alignas(8) unsigned char o[2304];
    std::memset(o, 0, sizeof o);
    float* p = table;
    std::memcpy(o + 2052, &p, sizeof p);  // the host's wider pointer; the contract reads sizeof(float*) here
    put(o, 916, 0.5f);
    struct Case { float rpm, factor; int lock; float want; } cases[] = {
        {0.0f, 1.0f, 0, 100.0f},
        {500.0f, 1.0f, 0, 150.0f},        // halfway between table[0]=100 and table[1]=200
        {2500.0f, 2.0f, 0, 700.0f},       // 300..400 midpoint 350, times 2
        {9000.0f, 1.0f, 0, 1000.0f},      // position 9: hi is also 9
        {12000.0f, 1.0f, 0, 1000.0f - 0.0f + 0.0f},  // position 12 clamps to 9; rpm past 9000 extrapolates with lo=9,hi=9 so no slope
        {500.0f, 0.0f, 0, -250.0f},       // factor 0: 0 - rpm * 0.5
        {500.0f, 1.0f, 1, 0.0f},          // locked
    };
    for (const Case& c : cases) {
        put(o, 748, c.rpm); put(o, 740, c.factor);
        int lock = c.lock; std::memcpy(o + 1024, &lock, 4);
        float got = getEngineTorque__7Vehicle(o);
        if (got != c.want) { std::fprintf(stderr, "fail: rpm %g factor %g lock %d got %g want %g\n", c.rpm, c.factor, c.lock, got, c.want); ++failures; }
    }
    return failures ? 1 : 0;
}
