// Observed changeOrientation__7VehicleG6Point3 at 0x00277fd8, 216 bytes. The
// Point3 argument arrives by hidden reference. With q the quaternion floats
// at vehicle+1084, v the Point3 and spin = (v.x * 0.5, v.y * 0.5, v.z * 0.5,
// 0.0):
//   product = quatMul(q, spin); q += product (component-wise, x y z w);
//   quatNormalize__FP4Quat(q); quatToMat__FP8Matrix33P4Quat(vehicle+1116, q).
// This is the same update backup__7Vehicle performs, with the angular
// velocity replaced by the argument and without its sign flip and time step.
// Nothing is returned. Nothing here was executed on the EE.
struct __attribute__((may_alias)) Quat { float x, y, z, w; };

extern "C" {
void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b);
void quatNormalize__FP4Quat(Quat* q);
void quatToMat__FP8Matrix33P4Quat(void* matrix, Quat* q);
}

typedef float AliasedFloat __attribute__((may_alias));

extern "C" void changeOrientation__7VehicleG6Point3(unsigned char* vehicle, const AliasedFloat* v) {
    Quat* const orientation = reinterpret_cast<Quat*>(vehicle + 1084);
    Quat product;
    Quat spin;
    const float x = v[0];
    const float y = v[1];
    const float z = v[2];
    spin.w = 0.0f;
    spin.x = x * 0.5f;
    spin.y = y * 0.5f;
    spin.z = z * 0.5f;
    quatMul__FP4QuatN20(&product, orientation, &spin);
    orientation->x = orientation->x + product.x;
    orientation->y = orientation->y + product.y;
    orientation->z = orientation->z + product.z;
    orientation->w = orientation->w + product.w;
    quatNormalize__FP4Quat(orientation);
    quatToMat__FP8Matrix33P4Quat(vehicle + 1116, orientation);
}
