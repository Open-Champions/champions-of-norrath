// Observed GetPositionAndYaw__7VehicleP6Point3T1Ps at 0x00277a28, 176 bytes.
// Transforms the first point through the 3x3 matrix at vehicle+1116 with
// matTransform__FP6Point3P8Matrix33T0 (result, matrix, input) into a local
// point, then adds the three floats at vehicle+16 to it, component by
// component, and copies the 12 bytes to the second point. Finally stores
// iatan2__Fff(-float(vehicle+1132), -float(vehicle+1128)) as a halfword at the
// third argument. Nothing is returned. The matrix and position meaning is
// inferred only from use, not established. Nothing here was executed on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* result, void* matrix, const Point3* input);
extern "C" short iatan2__Fff(float a, float b);

typedef float __attribute__((may_alias)) AliasedFloat;

extern "C" void GetPositionAndYaw__7VehicleP6Point3T1Ps(unsigned char* vehicle, const Point3* in, Point3* out, short* yaw) {
    Point3 local;
    matTransform__FP6Point3P8Matrix33T0(&local, vehicle + 1116, in);
    const AliasedFloat* position = reinterpret_cast<AliasedFloat*>(vehicle + 16);
    local.x += position[0];
    local.y += position[1];
    local.z += position[2];
    *out = local;
    *yaw = iatan2__Fff(-*reinterpret_cast<AliasedFloat*>(vehicle + 1132),
                       -*reinterpret_cast<AliasedFloat*>(vehicle + 1128));
}
