// Observed getNewRotAndPos__7VehicleP6Point3T1fii at 0x00271e68, 1104 bytes.
// Arguments: vehicle, a Point3 `first`, a Point3 `second`, float `step`, an int
// that is never read, and an int `flag` (the sixth, in t0). V is the Point3 at
// +1188, W the Point3 at +1200, P the Point3 at +16, and M the 3x3 matrix of
// rows at +1116, +1128, +1140. In order:
//   1. V += (first * float(+672)) * step, component by component.
//   2. W.k += (second.k * float(+1224+4k)) * step for k = 0..2.
//   3. u = (0,0,1); a = -(M row 1); b = M row 0; q = a x u; r = q x a;
//      angle1 = acosf(r.b) * 57.2947998 - 90.0 (dot as (r.x*b.x + r.y*b.y) +
//      r.z*b.z); with limit L = float(+920): if angle1 <= -L and W.y < 0 then
//      W.y = 0; if L <= angle1 and W.y > 0 then W.y = 0.
//   4. m = matUntransform(M, u); angle2 = acosf(a.m) * 57.2947998 - 90.0 with
//      limit L2 = float(+924): if angle2 <= -L2 and W.x < 0 then W.x = 0; if
//      L2 <= angle2 and W.x > 0 then W.x = 0.
//   5. P += (V * 39.0) * step, then changeOrientation__7VehicleG6Point3 is
//      called with (W * step) as the caller-owned Point3.
//   6. When `flag` is zero: the word at +2164 becomes 0, buildCollisionBox and
//      objectUpdateInGrid run on the vehicle.
// Cross products are written a*b - c*d in the same pairing as the original.
// Nothing is returned. Nothing here was executed on the EE. What the fields
// and limits mean is not established here.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
float acosf(float value);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void changeOrientation__7VehicleG6Point3(unsigned char* vehicle, const Point3* turn);
void buildCollisionBox__7Vehicle(unsigned char* vehicle);
void objectUpdateInGrid__FP10GameObject(void* object);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));

extern "C" void getNewRotAndPos__7VehicleP6Point3T1fii(unsigned char* vehicle, const Point3* first, const Point3* second,
                                                       float step, int unused, int flag) {
    (void)unused;
    Point3* const velocity = reinterpret_cast<Point3*>(vehicle + 1188);
    Point3* const spin = reinterpret_cast<Point3*>(vehicle + 1200);
    Point3* const position = reinterpret_cast<Point3*>(vehicle + 16);
    const AliasedFloat* const matrix = reinterpret_cast<AliasedFloat*>(vehicle + 1116);

    const float scale = *reinterpret_cast<AliasedFloat*>(vehicle + 672);
    const float fx = first->x * scale;
    const float fy = first->y * scale;
    const float fz = first->z * scale;
    velocity->z = velocity->z + fz * step;
    velocity->y = velocity->y + fy * step;
    velocity->x = velocity->x + fx * step;

    spin->x = spin->x + second->x * *reinterpret_cast<AliasedFloat*>(vehicle + 1224) * step;
    spin->y = spin->y + second->y * *reinterpret_cast<AliasedFloat*>(vehicle + 1228) * step;
    spin->z = spin->z + second->z * *reinterpret_cast<AliasedFloat*>(vehicle + 1232) * step;

    Point3 up;
    Point3 a;
    Point3 b;
    Point3 q;
    Point3 r;
    Point3 m;
    up.x = 0.0f; up.y = 0.0f; up.z = 1.0f;
    a.x = -matrix[3]; a.y = -matrix[4]; a.z = -matrix[5];
    b.x = matrix[0]; b.y = matrix[1]; b.z = matrix[2];
    q.x = a.y * up.z - a.z * up.y;
    q.y = a.z * up.x - a.x * up.z;
    q.z = a.x * up.y - a.y * up.x;
    r.x = q.y * a.z - q.z * a.y;
    r.y = q.z * a.x - q.x * a.z;
    r.z = q.x * a.y - q.y * a.x;

    const float angle1 = acosf(r.x * b.x + r.y * b.y + r.z * b.z) * 0x1.ca5bc0p+5f - 0x1.680000p+6f;
    const float limit1 = *reinterpret_cast<AliasedFloat*>(vehicle + 920);
    if (angle1 <= -limit1) {
        if (spin->y < 0.0f) spin->y = 0.0f;
    }
    if (limit1 <= angle1) {
        if (0.0f < spin->y) spin->y = 0.0f;
    }

    matUntransform__FP6Point3P8Matrix33T0(&m, vehicle + 1116, &up);
    const float angle2 = acosf(a.x * m.x + a.y * m.y + a.z * m.z) * 0x1.ca5bc0p+5f - 0x1.680000p+6f;
    const float limit2 = *reinterpret_cast<AliasedFloat*>(vehicle + 924);
    if (angle2 <= -limit2) {
        if (spin->x < 0.0f) spin->x = 0.0f;
    }
    if (limit2 <= angle2) {
        if (0.0f < spin->x) spin->x = 0.0f;
    }

    const float sx = velocity->x * 0x1.380000p+5f;
    const float sy = velocity->y * 0x1.380000p+5f;
    const float sz = velocity->z * 0x1.380000p+5f;
    position->x = position->x + sx * step;
    position->y = position->y + sy * step;
    position->z = position->z + sz * step;
    Point3 turn;
    turn.x = spin->x * step;
    turn.y = spin->y * step;
    turn.z = spin->z * step;
    changeOrientation__7VehicleG6Point3(vehicle, &turn);

    if (flag == 0) {
        *reinterpret_cast<AliasedInt*>(vehicle + 2164) = 0;
        buildCollisionBox__7Vehicle(vehicle);
        objectUpdateInGrid__FP10GameObject(vehicle);
    }
}
