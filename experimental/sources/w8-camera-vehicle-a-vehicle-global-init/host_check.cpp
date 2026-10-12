// Host contract for _GLOBAL_$I$__7VehicleG6Point3sPPci. Not an EE plan input.
#include "global_init.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int calls;
struct Seen {
    void* self;
    int ints[8];
    unsigned floats[3];
    int tail[3];
    void (*callback)(void*);
};
static Seen seen[8];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                                   float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    if (calls < 8) {
        Seen& s = seen[calls];
        s.self = self;
        const int ints[8] = {a, b, c, d, e, f, g, h};
        std::memcpy(s.ints, ints, sizeof ints);
        std::memcpy(&s.floats[0], &x, 4);
        std::memcpy(&s.floats[1], &y, 4);
        std::memcpy(&s.floats[2], &z, 4);
        s.tail[0] = i; s.tail[1] = j; s.tail[2] = k;
        s.callback = callback;
    }
    ++calls;
}

int main() {
    struct Want {
        int ints[8];
        unsigned floats[3];
        int tail[3];
    };
    const Want want[6] = {
        {{80, 80, 60, 60, 80, 80, 60, 0}, {0x41f00000u, 0x42f00000u, 0x3dccccccu}, {2, 0, 4}},
        {{70, 70, 50, 60, 70, 70, 50, 0}, {0x42200000u, 0x42c80000u, 0x3dccccccu}, {2, 0, 4}},
        {{80, 80, 60, 60, 80, 80, 60, 0}, {0x42a15c28u, 0x43964666u, 0x3dccccccu}, {2, 0, 4}},
        {{70, 70, 50, 60, 70, 70, 50, 0}, {0x4272b851u, 0x43af4666u, 0x3dccccccu}, {2, 0, 4}},
        {{234, 61, 4, 128, 92, 88, 64, 22}, {0x41c80000u, 0x42480000u, 0xc0833333u}, {2, 1, 8}},
        {{79, 65, 63, 81, 50, 40, 9, 12}, {0x42480000u, 0x43160000u, 0xbfe66666u}, {2, 0, 6}},
    };
    std::memset(zero_point, 0xaa, sizeof zero_point);
    _GLOBAL_$I$__7VehicleG6Point3sPPci();
    expect(calls == 6, "six constructions");
    for (int n = 0; n < 6 && n < calls; ++n) {
        expect(seen[n].self == particle_defs[n], "object n is at stride 80 in the file-local array");
        expect(std::memcmp(seen[n].ints, want[n].ints, sizeof want[n].ints) == 0, "ints 1..8");
        expect(std::memcmp(seen[n].floats, want[n].floats, sizeof want[n].floats) == 0, "float bit patterns");
        expect(std::memcmp(seen[n].tail, want[n].tail, sizeof want[n].tail) == 0, "ints 9..11");
        expect(seen[n].callback == nullptr, "null callback");
    }
    expect(reinterpret_cast<unsigned char*>(particle_defs[1]) - reinterpret_cast<unsigned char*>(particle_defs[0]) == 80, "stride is 80 bytes");
    bool zero = true;
    for (unsigned char b : zero_point) zero = zero && b == 0;
    expect(zero, "the 12-byte point is cleared");
    // The helper guards.
    std::memset(zero_point, 0xaa, sizeof zero_point);
    calls = 0;
    vehicle_static_init(0, 0xffff);
    vehicle_static_init(2, 0xffff);
    vehicle_static_init(1, 0);
    vehicle_static_init(1, 0xfffe);
    expect(calls == 0 && zero_point[0] == 0xaa, "wrong arguments do nothing");
    return failures ? 1 : 0;
}
