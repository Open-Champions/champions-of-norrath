// Host contract for addPassenger__7Vehiclei. Not an EE plan input. Pointer
// fields are host-sized here; the offsets used do not overlap.
#include "add_passenger.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

int frameNm;
unsigned char player[4 * 23984];

static int failures = 0;
static std::vector<int> seats_asked;
static int activate_calls;
static unsigned char* activate_arg;
static int memset_calls;

extern "C" void* memset(void* d, int v, unsigned long n) {
    ++memset_calls;
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
// Seat k sits k*10 along x so the nearest seat to x=34 is seat 3 unless it is taken.
extern "C" void getSeatPosition__7VehicleiP6Point3(unsigned char*, int seat, Point3* out) {
    seats_asked.push_back(seat);
    out->x = seat * 10.0f; out->y = 0; out->z = 0;
}
static void activate(unsigned char* self) { ++activate_calls; activate_arg = self; }

static int& w(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }

static unsigned char vehicle[2304];
static unsigned char vtable[256];
static unsigned char record[4][2048];

static void reset(int count, int driver, int flag) {
    std::memset(vehicle, 0x11, sizeof vehicle);
    std::memset(vtable, 0, sizeof vtable);
    short delta = 0x24; std::memcpy(vtable + 160, &delta, 2);
    void (*fn)(unsigned char*) = activate; std::memcpy(vtable + 164, &fn, sizeof fn);
    unsigned char* vt = vtable; std::memcpy(vehicle + 72, &vt, sizeof vt);
    w(vehicle, 976) = count; w(vehicle, 956) = driver; w(vehicle, 2148) = flag; w(vehicle, 980) = 0;
    for (int k = 0; k < 4; ++k) w(vehicle, 960 + 4 * k) = -1;
    for (int i = 0; i < 4; ++i) {
        std::memset(record[i], 0, sizeof record[i]);
        float x = 34.0f; std::memcpy(record[i] + 16, &x, 4);
        unsigned char* rp = record[i]; std::memcpy(player + i * 23984 + 21872, &rp, sizeof rp);
    }
    frameNm = 777; seats_asked.clear(); activate_calls = 0; memset_calls = 0;
}

int main() {
    // First rider, no driver, flag clear: seat 0, driver set, activate called with vehicle+0x24.
    reset(4, -1, 0);
    addPassenger__7Vehiclei(vehicle, 2);
    expect(w(vehicle, 960) == 2 && w(vehicle, 980) == 1 && w(vehicle, 984) == 1, "seat 0 taken");
    expect(w(vehicle, 956) == 2 && w(vehicle, 1032) == 1, "driver recorded");
    expect(seats_asked.empty(), "no seat search for the driver seat");
    expect(activate_calls == 1 && activate_arg == vehicle + 0x24, "virtual called with the adjusted this");
    expect(w(vehicle, 1004 + 8) == 777, "frame number stored");
    expect(w(record[2], 1400) == 16, "record word 1400");
    unsigned char* back; std::memcpy(&back, record[2] + 1328, sizeof back);
    expect(back == vehicle, "record points at the vehicle");

    // With a driver, seat 3 is the nearest free seat to x=34; seats 1..3 are asked.
    reset(4, 1, 0);
    addPassenger__7Vehiclei(vehicle, 0);
    expect(seats_asked.size() == 3 && seats_asked[0] == 1 && seats_asked[2] == 3, "seats 1..3 scanned");
    expect(w(vehicle, 972) == 0 && w(vehicle, 960) == -1 && w(vehicle, 996) == 1, "seat 3 chosen");
    expect(activate_calls == 0 && w(vehicle, 956) == 1, "no activate with a driver");

    // An occupied seat is skipped.
    reset(4, 1, 0);
    w(vehicle, 972) = 5;
    addPassenger__7Vehiclei(vehicle, 1);
    expect(seats_asked.size() == 2 && w(vehicle, 968) == 1, "seat 2 chosen when 3 is taken");

    // No driver but flag set: scanned like the driver-present case, no activate.
    reset(4, -1, 9);
    addPassenger__7Vehiclei(vehicle, 3);
    expect(seats_asked.size() == 3 && activate_calls == 0 && w(vehicle, 956) == -1, "flag blocks driver path");

    // Reset call.
    reset(4, 3, 0);
    w(vehicle, 980) = 3; w(vehicle, 1004) = 5; w(vehicle, 1016) = 5;
    memset_calls = 0;
    addPassenger__7Vehiclei(vehicle, -1);
    expect(w(vehicle, 980) == 0 && w(vehicle, 960) == -1 && w(vehicle, 972) == -1, "reset words");
    expect(w(vehicle, 1004) == 0 && w(vehicle, 1016) == 0 && w(vehicle, 1020) == 0x11111111, "16 bytes cleared");
    expect(w(vehicle, 956) == 3, "driver untouched by reset");
    return failures ? 1 : 0;
}
