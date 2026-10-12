// Host contract for GetPlayerPosition__7VehicleiP6Point3Ps. Not an EE plan input.
// cvt.w.s is modelled as round to nearest, ties to even.
#include "get_player_position.cpp"

#include <cmath>
#include <initializer_list>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int seat_calls, rot_calls;
static int seat_seen = -99;
static void* seat_vehicle;
static void* seat_point;
static int rot_axis;
static short rot_angle;
static float rot_input[9];
static float atan_a, atan_b;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void getSeatPosition__7VehicleiP6Point3(void* vehicle, int seat, void* point) {
    ++seat_calls; seat_vehicle = vehicle; seat_seen = seat; seat_point = point;
}
extern "C" void matRot__FP8Matrix33is(float* m, int axis, short angle) {
    ++rot_calls; rot_axis = axis; rot_angle = angle;
    std::memcpy(rot_input, m, sizeof rot_input);
    m[3] = 100.0f + angle;   // the rotation result is what the yaw is read from
    m[4] = -7.0f;
}
extern "C" short iatan2__Fff(float a, float b) { atan_a = a; atan_b = b; return static_cast<short>(0x2222); }

static void put_word(unsigned char* c, int off, int v) { std::memcpy(c + off, &v, 4); }
static void put_float(unsigned char* c, int off, float v) { std::memcpy(c + off, &v, 4); }

int main() {
    static unsigned char vehicle[2304];
    static unsigned char occupants[4][512];
    // Seat search.
    struct Case { int count; int table[4]; int player; int want; };
    const Case cases[] = {
        {0, {5, 5, 5, 5}, 5, 0}, {-3, {5, 5, 5, 5}, 5, 0}, {1, {9, 0, 0, 0}, 9, 0}, {1, {9, 0, 0, 0}, 4, 1},
        {3, {1, 2, 3, 0}, 1, 0}, {3, {1, 2, 3, 0}, 2, 1}, {3, {1, 2, 3, 0}, 3, 2}, {3, {1, 2, 3, 0}, 7, 3},
        {4, {2, 2, 2, 2}, 2, 0}, {4, {1, 2, 2, 2}, 2, 1}, {3, {1, 2, 3, 3}, 3, 2},
        {3, {1, 2, 3, 4}, 4, 3},  // index n is not read as a match, only the first n entries are searched
    };
    for (const Case& c : cases) {
        std::memset(vehicle, 0, sizeof vehicle);
        put_word(vehicle, 976, c.count);
        for (int i = 0; i < 4; ++i) put_word(vehicle, 960 + 4 * i, c.table[i]);
        const float fx = 3.0f, fz = -8.0f;
        put_float(vehicle, 1128, fx); put_float(vehicle, 1132, fz);
        seat_calls = 0; rot_calls = 0;
        short yaw = 0; float point[3];
        GetPlayerPosition__7VehicleiP6Point3Ps(vehicle, c.player, point, &yaw);
        expect(seat_calls == 1 && seat_vehicle == vehicle && seat_point == point, "one seat call with the output point");
        expect(seat_seen == c.want, "seat index");
        expect(rot_calls == 0 && yaw == 0x2222, "null occupant uses the vehicle's direction floats");
        expect(atan_a == 8.0f && atan_b == -3.0f, "angle arguments from +1132 then +1128, negated");
    }
    // Occupant present at the found seat, and at index n when nothing matches.
    for (int pass = 0; pass < 2; ++pass) {
        std::memset(vehicle, 0, sizeof vehicle);
        put_word(vehicle, 976, 2);
        put_word(vehicle, 960, 11); put_word(vehicle, 964, 12);
        const int player = pass == 0 ? 12 : 99;
        const int seat = pass == 0 ? 1 : 2;
        std::memset(occupants, 0, sizeof occupants);
        const float degrees = 90.0f;
        put_float(occupants[seat], 280, degrees);
        unsigned char* p = occupants[seat];
        std::memcpy(vehicle + 2088 + 4 * seat, &p, sizeof p);
        for (int i = 0; i < 9; ++i) put_float(vehicle, 1116 + 4 * i, 1.0f + i);
        seat_calls = rot_calls = 0;
        short yaw = 0; float point[3];
        GetPlayerPosition__7VehicleiP6Point3Ps(vehicle, player, point, &yaw);
        expect(seat_seen == seat && rot_calls == 1 && rot_axis == 2, "rotation about axis 2 for the occupant's seat");
        float want = 90.0f * 0.0055555552244186f * 32767.0f;
        int angle = static_cast<int>(std::nearbyint(want));
        expect(rot_angle == static_cast<short>(angle), "angle from float +280 times the two constants");
        expect(rot_angle > 16380 && rot_angle < 16386, "ninety degrees scales to about 16384");
        bool same = true;
        for (int i = 0; i < 9; ++i) same = same && rot_input[i] == 1.0f + i;
        expect(same, "matrix copied from vehicle+1116");
        expect(atan_a == 7.0f && atan_b == -(100.0f + rot_angle), "yaw from the rotated copy: -m[4], -m[3]");
        expect(yaw == 0x2222, "yaw stored");
    }
    // Large and negative angles wrap to a short.
    for (float degrees : {-90.0f, 360.0f, 1000.0f, 0.0f}) {
        std::memset(vehicle, 0, sizeof vehicle);
        put_word(vehicle, 976, 1); put_word(vehicle, 960, 1);
        std::memset(occupants, 0, sizeof occupants);
        put_float(occupants[0], 280, degrees);
        unsigned char* p = occupants[0];
        std::memcpy(vehicle + 2088, &p, sizeof p);
        short yaw; float point[3];
        GetPlayerPosition__7VehicleiP6Point3Ps(vehicle, 1, point, &yaw);
        const float want = degrees * 0.0055555552244186f * 32767.0f;
        expect(rot_angle == static_cast<short>(static_cast<int>(std::nearbyint(want))), "angle conversion and wrap");
    }
    return failures ? 1 : 0;
}
