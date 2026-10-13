// Observed GetLastTarget__6Camera at 0x0026fc68, 32 bytes. The original returns a
// three-float vector by value through a hidden result pointer (first register); the
// camera is the second register. It copies camera+24..35 (12 bytes) to the result and
// returns the result pointer. The reviewed compiler returns a 12-byte struct differently,
// so this source spells the hidden pointer out. Whether those floats are a target
// position is inferred from the name only. Nothing here was executed on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" Point3* GetLastTarget__6Camera(Point3* result, const unsigned char* camera) {
    *result = *reinterpret_cast<const Point3*>(camera + 24);
    return result;
}
