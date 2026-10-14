// Observed: Init__8VICameraR8VIRasterRC7VIVect3T2ffff at 0x010858d8 is 444
// bytes. Arguments: this, raster, vector p, vector q, floats fov (f12), aspect
// (f13), near (f14), far (f15).
// 1. A VIMatrix44 local is zeroed.
// 2. t = tanf(fov * 0.017453292f * 0.5f) (0x3c8efa34 is the degree factor).
// 3. Frustum__8VIRasterR10VIMatrix44ffffff(raster, matrix, -t*near*aspect,
//    t*near*aspect, t*near, -t*near, near, far).
// 4. Stores near at offset 0, far at offset 4, fov at offset 20, aspect at
//    offset 24 of this.
// 5. Calls CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(this, q, x, y, z) and
//    Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(this, p, x, y, z,
//    matrix) with three zeroed VIVect3 locals x, y, z.
// 6. Destroys the matrix local (_$_10VIMatrix44 with 2) and returns -1 if the
//    Update result was negative, else 0.
// Inferred: the float roles, from the tangent/frustum use; the Frustum argument
// order is the mangled order and its parameter meanings are unknown.
// Unknown: whether p and q are position and target, and the size of VICamera.
extern "C" {
struct VIVect3Local {
    float x, y, z;
};
struct VIMatrixLocal {
    float m[16];
};
float tanf(float angle);
int Frustum__8VIRasterR10VIMatrix44ffffff(void* raster, VIMatrixLocal* matrix, float a, float b, float c, float d, float e, float f);
int CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z);
int Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z, VIMatrixLocal* matrix);
void _$_10VIMatrix44(VIMatrixLocal* matrix, int flags);

int Init__8VICameraR8VIRasterRC7VIVect3T2ffff(void* camera, void* raster, void* p, void* q,
                                              float fov, float aspect, float near_plane, float far_plane) {
    alignas(16) VIMatrixLocal matrix;
    for (int index = 0; index < 16; ++index) matrix.m[index] = 0.0f;
    alignas(16) VIVect3Local x = {0.0f, 0.0f, 0.0f};
    alignas(16) VIVect3Local y = {0.0f, 0.0f, 0.0f};
    alignas(16) VIVect3Local z = {0.0f, 0.0f, 0.0f};
    float t = tanf(fov * 0.017453292f * 0.5f);
    float height = t * near_plane;
    Frustum__8VIRasterR10VIMatrix44ffffff(raster, &matrix, -t * near_plane * aspect, height * aspect,
                                          height, -t * near_plane, near_plane, far_plane);
    float* words = static_cast<float*>(camera);
    words[0] = near_plane;
    words[1] = far_plane;
    words[5] = fov;
    words[6] = aspect;
    CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(camera, q, &x, &y, &z);
    int result = Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(camera, p, &x, &y, &z, &matrix);
    _$_10VIMatrix44(&matrix, 2);
    return result < 0 ? -1 : 0;
}
}
