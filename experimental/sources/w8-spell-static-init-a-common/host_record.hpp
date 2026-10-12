#pragma once
// Host-only recorder for the static-initializer contracts. Not an EE input.
#include <cstdio>
#include <cstring>

namespace w8_host {

struct Row {
    int ints[8];
    unsigned float_bits[3];
    int tail[3];
};

struct Call {
    void* self;
    Row row;
    void (*callback)(void*);
};

inline Call calls[64];
inline int call_count = 0;

}  // namespace w8_host

extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    using namespace w8_host;
    Call& call = calls[call_count++];
    call.self = self;
    const int values[8] = {a, b, c, d, e, f, g, h};
    std::memcpy(call.row.ints, values, sizeof values);
    const float floats[3] = {x, y, z};
    std::memcpy(call.row.float_bits, floats, sizeof floats);
    call.row.tail[0] = i;
    call.row.tail[1] = j;
    call.row.tail[2] = k;
    call.callback = callback;
}

namespace w8_host {

inline int check_wrapper(void (*wrapper)(), const Row* expected, void* const* objects, int count) {
    int bad = 0;
    for (int pass = 0; pass < 2; ++pass) {
        call_count = 0;
        wrapper();
        if (call_count != count) {
            std::printf("FAIL pass %d: %d constructions, expected %d\n", pass, call_count, count);
            return 1;
        }
        for (int n = 0; n < count; ++n) {
            const Call& call = calls[n];
            if (call.self != objects[n] || (reinterpret_cast<__UINTPTR_TYPE__>(call.self) & 15u) != 0) {
                std::printf("FAIL pass %d row %d: wrong or unaligned record\n", pass, n);
                bad = 1;
            }
            for (int m = 0; m < n; ++m)
                if (calls[m].self == call.self) bad = 1;
            if (call.callback != nullptr) bad = 1;
            if (std::memcmp(&call.row, &expected[n], sizeof(Row)) != 0) {
                std::printf("FAIL pass %d row %d: arguments differ\n", pass, n);
                bad = 1;
            }
            if (pass == 0) {
                std::printf("ROW %d", n);
                for (int v : call.row.ints) std::printf(" %d", v);
                for (unsigned v : call.row.float_bits) std::printf(" %08x", v);
                for (int v : call.row.tail) std::printf(" %d", v);
                std::printf("\n");
            }
        }
    }
    return bad;
}

inline int check_quiet_paths(void (*initializer)(int, int)) {
    const int args[][2] = {{0, 0xffff}, {1, 0}, {0, 0}, {2, 0xffff}, {1, 0xfffe}};
    for (const auto& a : args) {
        call_count = 0;
        initializer(a[0], a[1]);
        if (call_count != 0) {
            std::printf("FAIL: initializer(%d, %#x) constructed %d records\n", a[0], a[1], call_count);
            return 1;
        }
    }
    return 0;
}

}  // namespace w8_host
