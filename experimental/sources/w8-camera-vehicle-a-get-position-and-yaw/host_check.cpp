// Host contract for GetPositionAndYaw__7VehicleP6Point3T1Ps. Not an EE plan input.
#include "get_position_and_yaw.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int transform_calls = 0;
static void* seen_matrix;
static const Point3* seen_input;
static Point3* seen_result;
static float seen_a, seen_b;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* result, void* matrix, const Point3* input) {
    ++transform_calls;
    seen_result = result;
    seen_matrix = matrix;
    seen_input = input;
    result->x = input->x * 2.0f;
    result->y = input->y * 3.0f;
    result->z = input->z * 5.0f;
}

extern "C" short iatan2__Fff(float a, float b) {
    seen_a = a;
    seen_b = b;
    return static_cast<short>(-0x2345);
}

int main() {
    static unsigned char vehicle[2304];
    const float position[3] = {100.0f, 200.0f, 300.0f};
    std::memcpy(vehicle + 16, position, sizeof position);
    const float x = 4.0f, z = -9.0f;
    std::memcpy(vehicle + 1128, &x, 4);
    std::memcpy(vehicle + 1132, &z, 4);
    const Point3 in = {1.0f, -2.0f, 0.5f};
    float out[4] = {0, 0, 0, 55.0f};
    short yaw = 0x7777;
    GetPositionAndYaw__7VehicleP6Point3T1Ps(vehicle, &in, reinterpret_cast<Point3*>(out), &yaw);
    expect(transform_calls == 1 && seen_matrix == vehicle + 1116, "matrix is at vehicle+1116");
    expect(seen_input == &in, "first point is the transform input");
    expect(seen_result != reinterpret_cast<Point3*>(out), "transform writes a local, not the output");
    expect(out[0] == 102.0f, "x is transformed then offset by +16");
    expect(out[1] == 194.0f, "y is transformed then offset by +20");
    expect(out[2] == 302.5f, "z is transformed then offset by +24");
    expect(out[3] == 55.0f, "fourth output word is untouched");
    expect(in.x == 1.0f && in.y == -2.0f && in.z == 0.5f, "input is not changed");
    expect(seen_a == 9.0f && seen_b == -4.0f, "angle arguments are minus +1132 then minus +1128");
    expect(yaw == -0x2345, "angle stored as halfword");
    return failures ? 1 : 0;
}
