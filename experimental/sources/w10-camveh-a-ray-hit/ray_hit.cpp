// Observed rayHit__7VehicleP6Point3T1fT1 at 0x002781d8, 220 bytes. Arguments:
// the vehicle (not read), a point (a1), a second Point3 (a2, not read), a
// float scale (f12) and a Point3 output (a3). Steps, with `world` the global
// pointer at 0x004844a0 and each Point3 passed to the world calls as a copy:
//   if worldTestForbidden__FP12_worldHeaderG6Point3f(world, point, 16.0)
//       equals 1.0, return -9999.0 and write nothing;
//   h = worldFindHeight__FP12_worldHeaderG6Point3f(world, point, 16.0);
//   out = (0.0, 0.0, 1.0);
//   return (point.z - h) / scale.
// The (0, 0, 1) output looks like an up normal and the result like a distance
// to the ground along z, but that is inferred; the field meanings are not
// established. Nothing here was executed on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char* world;
float worldTestForbidden__FP12_worldHeaderG6Point3f(unsigned char* world_header, const float* point, float radius);
float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* world_header, const float* point, float radius);
}

extern "C" float rayHit__7VehicleP6Point3T1fT1(void* vehicle, const Point3* point, const Point3* unused, float scale, Point3* out) {
    (void)vehicle;
    (void)unused;
    Point3 copy = *point;
    const float test_point[3] = {copy.x, copy.y, copy.z};
    if (worldTestForbidden__FP12_worldHeaderG6Point3f(world, test_point, 16.0f) == 1.0f) {
        return -9999.0f;
    }
    const float find_point[3] = {point->x, point->y, point->z};
    const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, find_point, 16.0f);
    out->z = 1.0f;
    out->x = 0.0f;
    out->y = 0.0f;
    return (point->z - height) / scale;
}
