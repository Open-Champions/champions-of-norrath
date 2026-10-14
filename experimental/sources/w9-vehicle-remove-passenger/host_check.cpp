// Host contract for removePassenger__7Vehiclei. Not an EE plan input. Pointer
// fields are host-sized here; the offsets used do not overlap.
#include "remove_passenger.cpp"

#include <cstdio>
#include <cstring>

unsigned char player[4 * 23984];

static int failures = 0;
static int dismount_calls, find_calls, deactivate_calls;
static unsigned char* dismount_arg;
static unsigned char* deactivate_arg;
static float find_arg[3];

extern "C" void dismount__6Player(unsigned char* r) { ++dismount_calls; dismount_arg = r; }
extern "C" void objectFindEmptySpot__FG6Point3iiPc(Point3* result, const Point3* spot, int a, int b, char* c) {
    ++find_calls;
    find_arg[0] = spot->x; find_arg[1] = spot->y; find_arg[2] = spot->z;
    if (a != 48 || b != 48 || c != 0) { std::fprintf(stderr, "fail: find args\n"); ++failures; }
    result->x = 1000; result->y = 2000; result->z = 3000;
}
static void deactivate(unsigned char* self) { ++deactivate_calls; deactivate_arg = self; }

static int& w(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static float& fl(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }

static unsigned char vehicle[2304];
static unsigned char vtable[256];
static unsigned char record[4][2048];

static void reset(int driver) {
    std::memset(vehicle, 0x11, sizeof vehicle);
    std::memset(vtable, 0, sizeof vtable);
    short delta = 0x30; std::memcpy(vtable + 168, &delta, 2);
    void (*fn)(unsigned char*) = deactivate; std::memcpy(vtable + 172, &fn, sizeof fn);
    unsigned char* vt = vtable; std::memcpy(vehicle + 72, &vt, sizeof vt);
    w(vehicle, 976) = 4; w(vehicle, 956) = driver; w(vehicle, 980) = 3; w(vehicle, 1032) = 1;
    w(vehicle, 960) = 2; w(vehicle, 964) = 1; w(vehicle, 968) = 3; w(vehicle, 972) = -1;
    for (int k = 0; k < 4; ++k) w(vehicle, 984 + 4 * k) = 1;
    fl(vehicle, 1116) = 1; fl(vehicle, 1120) = 2; fl(vehicle, 1124) = 3; fl(vehicle, 808) = 5;
    for (int i = 0; i < 4; ++i) {
        std::memset(record[i], 0, sizeof record[i]);
        fl(record[i], 16) = 10; fl(record[i], 20) = 20; fl(record[i], 24) = 30;
        unsigned char* back = vehicle; std::memcpy(record[i] + 1328, &back, sizeof back);
        unsigned char* rp = record[i]; std::memcpy(player + i * 23984 + 21872, &rp, sizeof rp);
    }
    dismount_calls = find_calls = deactivate_calls = 0;
}

int main() {
    // A passenger who is not the driver.
    reset(2);
    removePassenger__7Vehiclei(vehicle, 1);
    expect(w(vehicle, 964) == -1 && w(vehicle, 960) == 2 && w(vehicle, 968) == 3, "only the matching seat word freed");
    expect(w(vehicle, 980) == 2 && w(vehicle, 988) == -1 && w(vehicle, 984) == 1, "count and flag");
    expect(dismount_calls == 1 && dismount_arg == record[1], "dismount on the record");
    unsigned char* back; std::memcpy(&back, record[1] + 1328, sizeof back);
    expect(back == 0 && find_calls == 0 && w(vehicle, 1032) == 1 && fl(record[1], 16) == 10, "no spot search");
    expect(deactivate_calls == 0 && w(vehicle, 956) == 2 && fl(vehicle, 808) == 30.0f, "30.0 stored");

    // The driver: spot search from record position + 20 * first row.
    reset(3);
    removePassenger__7Vehiclei(vehicle, 3);
    expect(w(vehicle, 968) == -1 && find_calls == 1, "driver seat freed, spot found");
    expect(find_arg[0] == 30 && find_arg[1] == 60 && find_arg[2] == 90, "20 * row + position");
    expect(fl(record[3], 16) == 1000 && fl(record[3], 20) == 2000 && fl(record[3], 24) == 3000, "result stored");
    expect(w(vehicle, 1032) == 0 && w(vehicle, 956) == -1, "driver cleared");
    expect(deactivate_calls == 1 && deactivate_arg == vehicle + 0x30, "slot 168 virtual with delta");
    expect(fl(vehicle, 808) == 30.0f, "30.0 stored");

    // Not present: nothing changes.
    reset(0);
    unsigned char before[sizeof vehicle]; std::memcpy(before, vehicle, sizeof vehicle);
    removePassenger__7Vehiclei(vehicle, 0);
    expect(std::memcmp(before, vehicle, sizeof vehicle) == 0 && dismount_calls == 0, "absent index writes nothing");

    // Only slots below the count are scanned.
    reset(0);
    w(vehicle, 976) = 3; w(vehicle, 972) = 0;
    removePassenger__7Vehiclei(vehicle, 0);
    expect(w(vehicle, 972) == 0 && dismount_calls == 0, "slot at the count is not scanned");
    return failures ? 1 : 0;
}
