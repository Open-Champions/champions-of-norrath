// Host contract for getSeatPosition__7VehicleiP6Point3. Not an EE plan input.
#include "get_seat_position.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int transform_calls, rot_calls;
static int rot_axis[4]; static short rot_angle[4];
static int copy_was_rotated_at_second_transform;

void matTransform__FP6Point3P8Matrix33T0(Point3* out, Matrix33* m, Point3* in) {
    ++transform_calls;
    if (transform_calls == 2) copy_was_rotated_at_second_transform = m->m[0] > 1000.0f;
    out->x = in->x * 2 + m->m[0];
    out->y = in->y * 2 + m->m[1];
    out->z = in->z * 2 + m->m[2];
}
void matRot__FP8Matrix33is(Matrix33* m, int axis, short angle) {
    rot_axis[rot_calls] = axis; rot_angle[rot_calls] = angle; ++rot_calls;
    m->m[0] += 1000.0f;
}

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static float get(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }

int main() {
    static unsigned char vehicle[2304];
    static unsigned char holder[400];
    Point3 out;
    put(vehicle, 16, 100); put(vehicle, 20, 200); put(vehicle, 24, 300);
    for (int i = 0; i < 9; ++i) put(vehicle, 1116 + 4 * i, float(i + 1));
    for (int s = 0; s < 3; ++s)
        for (int k = 0; k < 3; ++k) put(vehicle, 1932 + 12 * s + 4 * k, float(10 * s + k + 1));

    // No holder at seat 1: out = position + matrix*offset (stub: 2*offset + row).
    getSeatPosition__7VehicleiP6Point3(vehicle, 1, &out);
    expect(transform_calls == 1 && rot_calls == 0, "one transform, no rotation without a holder");
    expect(out.x == 100 + 2 * 11 + 1 && out.y == 200 + 2 * 12 + 2 && out.z == 300 + 2 * 13 + 3, "no-holder result");

    // Holder at seat 2: angles 90 and 45 degrees -> 16384-ish units.
    transform_calls = rot_calls = 0;
    put(holder, 280, 90.0f); put(holder, 284, 45.0f);
    put(holder, 288, 5); put(holder, 292, 6); put(holder, 296, 7);
    unsigned p = (unsigned)(unsigned long)holder; (void)p;
    unsigned char* hp = holder; std::memcpy(vehicle + 2088 + 8, &hp, sizeof hp);
    getSeatPosition__7VehicleiP6Point3(vehicle, 2, &out);
    expect(transform_calls == 2 && rot_calls == 2, "two transforms and two rotations");
    expect(rot_axis[0] == 2 && rot_axis[1] == 0, "axes 2 then 0");
    expect(rot_angle[0] == (short)(int)(90.0f * 0x1.6c16c0p-8f * 0x1.fffc00p+14f), "first angle");
    expect(rot_angle[1] == (short)(int)(45.0f * 0x1.6c16c0p-8f * 0x1.fffc00p+14f), "second angle");
    expect(copy_was_rotated_at_second_transform, "second transform uses the rotated copy");
    // first = 2*offset(21,22,23)+row(1,2,3) = 43,46,49 ; second = 2*(5,6,7)+(2001..) = 2*5+2001=2011, 12+2, 14+3
    expect(out.x == 100 + 2011 + 43 && out.y == 200 + 14 + 46 && out.z == 300 + 17 + 49, "holder result");
    expect(get(vehicle, 1116) == 1.0f, "vehicle rotation untouched");
    return failures ? 1 : 0;
}
