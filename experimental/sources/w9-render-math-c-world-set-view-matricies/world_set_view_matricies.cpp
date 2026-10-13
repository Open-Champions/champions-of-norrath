// Observed worldSetViewMatricies__FP6Point3P8Matrix34 at 0x0017b4e8, 592 bytes.
// Arguments: a position (three floats) and a 48-byte Matrix34 (twelve floats).
// In order:
//  1. The position is copied to worldCameraPos (12 bytes).
//  2. The matrix is copied to a local, and matTrans__FP8Matrix34fff is applied to
//     the local with (-x, -y, -z) of the position.
//  3. The translated local (48 bytes) is stored to worldCameraMatrix.
//  4. A local 64-byte Matrix44 is built three times from the same seven float values
//     and multiplied (matMul__FP8Matrix44P8Matrix34T0 with the translated local and
//     that Matrix44) into three destinations:
//       matMakeViewMatrix__FP8Matrix44fffffff     -> worldViewMatrix
//       matMakeCharViewMatrix__FP8Matrix44fffffff -> worldCharViewMatrix
//       matMakeWorldViewMatrix__FP8Matrix44fffffff, after matScale__FP8Matrix34f(local, 4.0)
//                                                -> worldWorldViewMatrix
//     (matMul's first argument is the destination.)
//  5. worldTransformFrustum__FP8Matrix34 is called with worldCameraMatrix.
// The seven floats are, in argument order: the 32-bit pattern 0x3f7fffff, the pattern
// 0x3eeeeeee, the current value of worldScrz, 1.0, the pattern 0x457fff00, 18.0 and
// 1800.0. The first two and fifth are read from the small-data constant pool.
// The matScale call happens between the char view and world view products, so the
// world-view product is made from the already scaled local, and the view and char-view
// products use the unscaled one.
// Inferred: the seven values are projection parameters for the view builders.
// Unknown: their meanings and the types behind the Matrix names.
extern float worldScrz;
extern "C" {
extern unsigned char worldCameraPos[12];
extern unsigned char worldCameraMatrix[48];
extern float worldViewMatrix[16];
extern float worldCharViewMatrix[16];
extern float worldWorldViewMatrix[16];
void matTrans__FP8Matrix34fff(float* matrix, float x, float y, float z);
void matMakeViewMatrix__FP8Matrix44fffffff(float* matrix, float a, float b, float c, float d, float e, float f, float g);
void matMakeCharViewMatrix__FP8Matrix44fffffff(float* matrix, float a, float b, float c, float d, float e, float f, float g);
void matMakeWorldViewMatrix__FP8Matrix44fffffff(float* matrix, float a, float b, float c, float d, float e, float f, float g);
void matMul__FP8Matrix44P8Matrix34T0(float* destination, float* left, float* right);
void matScale__FP8Matrix34f(float* matrix, float scale);
void worldTransformFrustum__FP8Matrix34(void* matrix);
}

namespace {
inline float pattern(unsigned bits) { return __builtin_bit_cast(float, bits); }
}

extern "C" void worldSetViewMatricies__FP6Point3P8Matrix34(const float* position, const float* matrix) {
    float local[12];
    float temp[16];
    const float a = pattern(0x3f7fffffu);
    const float b = pattern(0x3eeeeeeeu);
    const float e = pattern(0x457fff00u);
    float* camera_pos = reinterpret_cast<float*>(worldCameraPos);
    camera_pos[0] = position[0];
    camera_pos[1] = position[1];
    camera_pos[2] = position[2];
    for (int i = 0; i < 12; ++i) local[i] = matrix[i];
    matTrans__FP8Matrix34fff(local, -position[0], -position[1], -position[2]);
    float* camera_matrix = reinterpret_cast<float*>(worldCameraMatrix);
    for (int i = 0; i < 12; ++i) camera_matrix[i] = local[i];
    const float c = worldScrz;
    matMakeViewMatrix__FP8Matrix44fffffff(temp, a, b, c, 1.0f, e, 18.0f, 1800.0f);
    matMul__FP8Matrix44P8Matrix34T0(worldViewMatrix, local, temp);
    matMakeCharViewMatrix__FP8Matrix44fffffff(temp, a, b, c, 1.0f, e, 18.0f, 1800.0f);
    matMul__FP8Matrix44P8Matrix34T0(worldCharViewMatrix, local, temp);
    matMakeWorldViewMatrix__FP8Matrix44fffffff(temp, a, b, c, 1.0f, e, 18.0f, 1800.0f);
    matScale__FP8Matrix34f(local, 4.0f);
    matMul__FP8Matrix44P8Matrix34T0(worldWorldViewMatrix, local, temp);
    worldTransformFrustum__FP8Matrix34(camera_matrix);
}
