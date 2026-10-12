// Host contract for GetNPCPosition__7VehicleP6Point3Ps. Not an EE plan input.
#include "get_npc_position.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int seat_calls = 0;
static unsigned char* seen_vehicle;
static int seen_seat = -1;
static void* seen_point;
static int atan_calls = 0;
static float seen_a, seen_b;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void getSeatPosition__7VehicleiP6Point3(unsigned char* vehicle, int seat, void* point) {
    ++seat_calls;
    seen_vehicle = vehicle;
    seen_seat = seat;
    seen_point = point;
    const float marker[3] = {1.5f, 2.5f, 3.5f};
    std::memcpy(point, marker, sizeof marker);
}

extern "C" short iatan2__Fff(float a, float b) {
    ++atan_calls;
    seen_a = a;
    seen_b = b;
    return static_cast<short>(0x1234);
}

int main() {
    static unsigned char vehicle[2304];
    float point[4] = {0, 0, 0, 77.0f};
    short yaw = 0x7777;
    const float x = 4.0f, z = -9.0f;
    std::memcpy(vehicle + 1128, &x, 4);
    std::memcpy(vehicle + 1132, &z, 4);
    GetNPCPosition__7VehicleP6Point3Ps(vehicle, point, &yaw);
    expect(seat_calls == 1 && seen_vehicle == vehicle && seen_point == point, "seat position asked for this vehicle and point");
    expect(seen_seat == 0, "seat zero");
    expect(point[0] == 1.5f && point[1] == 2.5f && point[2] == 3.5f && point[3] == 77.0f, "point comes from the seat call alone");
    expect(atan_calls == 1, "one angle call");
    expect(seen_a == 9.0f, "first angle argument is minus the float at +1132");
    expect(seen_b == -4.0f, "second angle argument is minus the float at +1128");
    expect(yaw == 0x1234, "angle result stored as the halfword");
    return failures ? 1 : 0;
}
