// Observed addImpulse__7VehicleP6Point3T1i at 0x00272e20, 336 bytes. Arguments: the vehicle v, an impulse
// vector I, a point P and an int. The int is never read (the register that carries it is overwritten
// at the start). Steps:
//   1. With k the float at v+672: the three floats at v+1188 each gain I's component times k.
//   2. The cross product P x I = (Py*Iz - Pz*Iy, Pz*Ix - Px*Iz, Px*Iy - Py*Ix) is passed through
//      matUntransform with the 3x3 matrix at v+1116 into a local vector w.
//   3. w is scaled component-wise by the three floats at v+1224, the word at v+2144 is set to 1, and the
//      scaled w is added to the three floats at v+1200.
// Inferred: this applies a linear impulse and the resulting torque to a rigid body; v+1188 would be its
// velocity, v+1200 its angular velocity, v+672 an inverse mass and v+1224 an inverse inertia in
// body coordinates, v+1116 the body orientation. None of that is established here, and the flag at v+2144
// is only known to be set. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);

void addImpulse__7VehicleP6Point3T1i(void* self, const Point3* impulse, const Point3* point, int) {
    unsigned char* const v = static_cast<unsigned char*>(self);
    const float k = *reinterpret_cast<float*>(v + 672);
    Point3* velocity = reinterpret_cast<Point3*>(v + 1188);
    velocity->x += impulse->x * k;
    velocity->y += impulse->y * k;
    velocity->z += impulse->z * k;
    Point3 torque;
    torque.x = point->y * impulse->z - point->z * impulse->y;
    torque.y = point->z * impulse->x - point->x * impulse->z;
    torque.z = point->x * impulse->y - point->y * impulse->x;
    Point3 local;
    matUntransform__FP6Point3P8Matrix33T0(&local, v + 1116, &torque);
    const Point3* inertia = reinterpret_cast<const Point3*>(v + 1224);
    Point3* angular = reinterpret_cast<Point3*>(v + 1200);
    local.x *= inertia->x;
    *reinterpret_cast<int*>(v + 2144) = 1;
    local.y *= inertia->y;
    local.z *= inertia->z;
    angular->x += local.x;
    angular->y += local.y;
    angular->z += local.z;
}
}
