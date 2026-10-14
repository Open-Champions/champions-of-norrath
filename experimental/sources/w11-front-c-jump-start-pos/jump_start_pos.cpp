// Observed startPos at 0x008d5fc0, bss, symbol size 0; endPos is 16 bytes later, so the
// vector is 12 bytes of a 16-byte slot. initJumpEffect references it twice (authored code stores the camera position).
// Inferred: a three-float position. Unknown: whether the original padded the type.
struct Point3 {
    float x, y, z;
};
extern "C" {
Point3 startPos;
}
