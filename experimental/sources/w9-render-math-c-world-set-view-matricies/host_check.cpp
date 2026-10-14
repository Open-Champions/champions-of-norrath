// Host contract for worldSetViewMatricies__FP6Point3P8Matrix34. Not an EE plan input.
// All collaborators are recorded stubs.
#include <cstdio>
#include <cstring>
#include <string>
float worldScrz = 777.0f;
extern "C" {
unsigned char worldCameraPos[12];
unsigned char worldCameraMatrix[48];
float worldViewMatrix[16];
float worldCharViewMatrix[16];
float worldWorldViewMatrix[16];
}
static std::string order;
static float trans_args[3];
static float local_after_trans[12];
static float view_args[7];
static float scale_arg = 0;
static float* frustum_arg = nullptr;
static int mul_dest_ok = 0;
extern "C" {
void matTrans__FP8Matrix34fff(float* m, float x, float y, float z) {
    order += "T"; trans_args[0] = x; trans_args[1] = y; trans_args[2] = z;
    for (int i = 0; i < 12; ++i) m[i] += 100.0f;       // visible change
    std::memcpy(local_after_trans, m, sizeof local_after_trans);
}
static void make(char tag, float* m, float a, float b, float c, float d, float e, float f, float g) {
    order += tag; float v[7] = {a, b, c, d, e, f, g};
    if (tag == 'V') std::memcpy(view_args, v, sizeof v); else if (std::memcmp(view_args, v, sizeof v) != 0) order += "!";
    for (int i = 0; i < 16; ++i) m[i] = float(tag);
}
void matMakeViewMatrix__FP8Matrix44fffffff(float* m, float a, float b, float c, float d, float e, float f, float g) { make('V', m, a, b, c, d, e, f, g); }
void matMakeCharViewMatrix__FP8Matrix44fffffff(float* m, float a, float b, float c, float d, float e, float f, float g) { make('C', m, a, b, c, d, e, f, g); }
void matMakeWorldViewMatrix__FP8Matrix44fffffff(float* m, float a, float b, float c, float d, float e, float f, float g) { make('W', m, a, b, c, d, e, f, g); }
void matScale__FP8Matrix34f(float* m, float s) { order += "S"; scale_arg = s; m[0] += 1000.0f; }
void matMul__FP8Matrix44P8Matrix34T0(float* dest, float* left, float* right) {
    order += "M";
    float tag = right[0];
    for (int i = 0; i < 16; ++i) dest[i] = tag + left[0];
}
void worldTransformFrustum__FP8Matrix34(void* m) { order += "F"; frustum_arg = static_cast<float*>(m); }
void worldSetViewMatricies__FP6Point3P8Matrix34(const float*, const float*);
}
int main() {
    float pos[3] = {1.5f, -2.5f, 3.5f};
    float mat[12];
    for (int i = 0; i < 12; ++i) mat[i] = float(i);
    worldSetViewMatricies__FP6Point3P8Matrix34(pos, mat);
    if (order != "TVMCMWSMF") { std::fprintf(stderr, "order %s\n", order.c_str()); return 1; }
    if (std::memcmp(worldCameraPos, pos, 12) != 0) return 2;
    if (trans_args[0] != -1.5f || trans_args[1] != 2.5f || trans_args[2] != -3.5f) return 3;
    if (std::memcmp(worldCameraMatrix, local_after_trans, 48) != 0) return 4;
    if (mat[0] != 0.0f) return 5;                                   // caller's matrix untouched
    if (view_args[2] != 777.0f || view_args[3] != 1.0f || view_args[5] != 18.0f || view_args[6] != 1800.0f) return 6;
    unsigned bits;
    std::memcpy(&bits, &view_args[0], 4); if (bits != 0x3f7fffffu) return 7;
    std::memcpy(&bits, &view_args[1], 4); if (bits != 0x3eeeeeeeu) return 8;
    std::memcpy(&bits, &view_args[4], 4); if (bits != 0x457fff00u) return 9;
    if (scale_arg != 4.0f) return 10;
    if (worldViewMatrix[0] != float('V') + 100.0f || worldCharViewMatrix[0] != float('C') + 100.0f) return 11;
    if (worldWorldViewMatrix[0] != float('W') + 1100.0f) return 12;  // made from the scaled local
    if (frustum_arg != reinterpret_cast<float*>(worldCameraMatrix)) return 13;
    std::printf("world set view matricies contract passed\n");
    return 0;
}
