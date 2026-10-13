// Host contract for set__4Boat. Not an EE plan input. The expected stores are
// listed as raw float bit patterns (or vehicle-relative addresses) together
// with the number of external calls that must already have happened when the
// store lands, so a wrong value, a missing store, a stray write, or a store
// on the wrong side of setMassAndIBody__7Vehicle / addWheel__7VehiclePciffff
// all fail.
#include "boat_set.cpp"

#include <cstdio>
#include <cstring>
#include <cstdint>

struct Store { int offset; unsigned bits; int address; int calls_before; };
static const Store expected[] = {
    {884, 0x461c4000u, 0, 0},
    {896, 0x437a0000u, 0, 0},
    {892, 0x42f00000u, 0, 0},
    {888, 0x42f00000u, 0, 0},
    {940, 0x42480000u, 0, 1},
    {924, 0x41700000u, 0, 1},
    {920, 0x41c80000u, 0, 1},
    {1044, 0x00000001u, 0, 1},
    {856, 0x43160000u, 0, 1},
    {864, 0x41f00000u, 0, 1},
    {868, 0x42340000u, 0, 1},
    {872, 0x3f666666u, 0, 1},
    {876, 0xbd4cccccu, 0, 1},
    {880, 0xbdccccccu, 0, 1},
    {764, 0x3f8cccccu, 0, 1},
    {944, 0x42200000u, 0, 1},
    {948, 0x40000000u, 0, 1},
    {2344, 0x3eec4ec4u, 0, 1},
    {2348, 0x47afc800u, 0, 1},
    {2352, 0x459c4000u, 0, 1},
    {2064, 0, 2344, 1},
    {2068, 0, 2328, 1},
    {928, 0x42480000u, 0, 1},
    {932, 0x42480000u, 0, 1},
    {936, 0x42480000u, 0, 1},
    {860, 0x42340000u, 0, 1},
    {2328, 0x3eec4ec4u, 0, 1},
    {2332, 0x47afc800u, 0, 1},
    {2336, 0x459c4000u, 0, 1},
    {2340, 0x3f800000u, 0, 1},
    {2356, 0x3f800000u, 0, 1},
    {2056, 0, 2328, 1},
    {2060, 0, 2344, 1},
    {908, 0x3f800000u, 0, 1},
    {900, 0x00000000u, 0, 1},
    {904, 0x00000000u, 0, 1},
    {1000, 0x00000000u, 0, 1},
    {912, 0x3f800000u, 0, 1},
    {916, 0x3d4cccccu, 0, 1},
};
struct Call { int is_wheel; const char* name; int count; unsigned floats[4]; };
static const Call expected_calls[] = {
    {0, "", 0, {0, 0, 0, 0}},
};

static unsigned char* object;
static int call_index;
static int failures;
static unsigned char seen[2400];

static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }

static void check_stores_before(int calls) {
    // Every store that belongs before this call must be visible; every later one must not.
    for (const Store& s : expected) {
        unsigned have; std::memcpy(&have, object + s.offset, 4);
        const bool written = std::memcmp(object + s.offset, seen + s.offset, 4) != 0;
        if (s.calls_before < calls) {
            if (s.address) {
                (void)have;  // address-valued stores are checked after the run
            } else expect(have == s.bits, "value store before call");
        } else if (s.calls_before >= calls && !s.address && s.bits != 0x6b6b6b6bu) {
            expect(!written || have == s.bits, "later store has not happened early");
            (void)written;
        }
    }
}

extern "C" void setMassAndIBody__7Vehicle(unsigned char* v) {
    expect(v == object, "mass object");
    expect(expected_calls[call_index].is_wheel == 0, "call order: setMass");
    check_stores_before(call_index);
    ++call_index;
}


int main() {
    static unsigned char storage[2400];
    object = storage;
    std::memset(storage, 0x6b, sizeof storage);
    std::memcpy(seen, storage, sizeof storage);
    set__4Boat(object);
    expect(call_index == (int)(sizeof expected_calls / sizeof expected_calls[0]), "all calls made");
    for (const Store& s : expected) {
        if (s.address) {
            // Host pointers are wider than the 4-byte target slots, so a later
            // address store can overwrite the high half of an earlier slot; compare
            // the low half, and skip a slot whose low half a later store covers.
            bool clobbered = false;
            for (const Store& later : expected)
                if (&later > &s && later.address && later.offset + (int)sizeof(void*) > s.offset && later.offset < s.offset + 4) clobbered = true;
            unsigned have; std::memcpy(&have, object + s.offset, 4);
            const unsigned want = (unsigned)(std::uintptr_t)(object + s.address);
            expect(clobbered || have == want, "address store");
        } else {
            unsigned have; std::memcpy(&have, object + s.offset, 4);
            expect(have == s.bits, "value store");
        }
    }
    // Nothing outside the listed words changed.
    for (int i = 0; i < (int)sizeof storage; ++i) {
        bool listed = false;
        for (const Store& s : expected) if (i >= s.offset && i < s.offset + (s.address ? (int)sizeof(void*) : 4)) listed = true;
        if (!listed) expect(storage[i] == 0x6b, "unlisted byte untouched");
    }
    return failures ? 1 : 0;
}
