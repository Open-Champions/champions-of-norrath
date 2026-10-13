// Observed backup__7Vehicle at 0x00272cc0, 352 bytes. Steps:
//   backupJustForces__7Vehicle(vehicle);
//   with v = Point3 at +1188 and p = Point3 at +16: each p component drops by
//   (v component * 39.0) * C, with C the float 0x3c888888 (about 1/60);
//   with w = Point3 at +1200: spin = Quat(w.x * -C * 0.5, w.y * -C * 0.5,
//   w.z * -C * 0.5, 0.0), product = quatMul(Quat at +1084, spin), and the four
//   floats at +1084 each gain the matching product component (x, y, z, w);
//   quatNormalize__FP4Quat on +1084; quatToMat__FP8Matrix33P4Quat writes the
//   matrix at +1116 from it.
// The multiply order in the original is (component * 39.0) * C and
// (component * -C) * 0.5. Nothing is returned. Nothing here was executed on
// the EE. What +1188 and +1200 hold is not established here.
struct __attribute__((may_alias)) Quat { float x, y, z, w; };

extern "C" {
void backupJustForces__7Vehicle(unsigned char* vehicle);
void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b);
void quatNormalize__FP4Quat(Quat* q);
void quatToMat__FP8Matrix33P4Quat(void* matrix, Quat* q);
}

typedef float AliasedFloat __attribute__((may_alias));

extern "C" void backup__7Vehicle(unsigned char* vehicle) {
    Quat* const orientation = reinterpret_cast<Quat*>(vehicle + 1084);
    Quat product;
    Quat spin;
    backupJustForces__7Vehicle(vehicle);
    const AliasedFloat* const v = reinterpret_cast<AliasedFloat*>(vehicle + 1188);
    AliasedFloat* const p = reinterpret_cast<AliasedFloat*>(vehicle + 16);
    const float sx = v[0] * 39.0f;
    const float sy = v[1] * 39.0f;
    const float sz = v[2] * 39.0f;
    p[2] = p[2] - sz * 0x1.111110p-6f;
    p[1] = p[1] - sy * 0x1.111110p-6f;
    p[0] = p[0] - sx * 0x1.111110p-6f;
    const AliasedFloat* const w = reinterpret_cast<AliasedFloat*>(vehicle + 1200);
    spin.x = w[0] * -0x1.111110p-6f * 0.5f;
    spin.y = w[1] * -0x1.111110p-6f * 0.5f;
    spin.z = w[2] * -0x1.111110p-6f * 0.5f;
    spin.w = 0.0f;
    quatMul__FP4QuatN20(&product, orientation, &spin);
    orientation->x = orientation->x + product.x;
    orientation->y = orientation->y + product.y;
    orientation->z = orientation->z + product.z;
    orientation->w = orientation->w + product.w;
    quatNormalize__FP4Quat(orientation);
    quatToMat__FP8Matrix33P4Quat(vehicle + 1116, orientation);
}
