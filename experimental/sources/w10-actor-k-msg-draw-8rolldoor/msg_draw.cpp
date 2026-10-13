// Observed msg_draw__8RollDoor at 0x0021c690, 212 bytes. Takes t, the pointer at object+36
// (a block whose word +12 and word +20 are the two resource pointers handed to modelDraw).
// Builds a local 64-byte matrix with matMakeTransYaw__FP8Matrix34G6Point3s from the three
// floats at object+16 and the angle (halfword +56) minus 0x8000 as a signed halfword, then
// applies matRot__FP8Matrix34is(matrix, 0, halfword +88). Finally calls modelDraw with
// (t[12], t[20], 3, the three floats at +16, matrix, 0, -1, 0, 0), the last argument being
// passed on the stack. Nothing is returned. Nothing here ran on the EE. What the arguments
// mean beyond their types is unknown.
struct Point3 { float x, y, z; };
struct Matrix34 { float m[16]; };

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* matrix, Point3 position, short angle);
void matRot__FP8Matrix34is(Matrix34* matrix, int axis, short angle);
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, Matrix34* matrix, void* animation,
    unsigned long flags, void* record, int extra);

void msg_draw__8RollDoor(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = *reinterpret_cast<unsigned char**>(o + 36);
    Matrix34 matrix;
    Point3 position = *reinterpret_cast<Point3*>(o + 16);
    matMakeTransYaw__FP8Matrix34G6Point3s(&matrix, position, static_cast<short>(*reinterpret_cast<unsigned short*>(o + 56) - 0x8000));
    matRot__FP8Matrix34is(&matrix, 0, *reinterpret_cast<short*>(o + 88));
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        *reinterpret_cast<void**>(t + 12), *reinterpret_cast<void**>(t + 20), 3, *reinterpret_cast<Point3*>(o + 16), &matrix, 0, ~0ul, 0, 0);
}
}
