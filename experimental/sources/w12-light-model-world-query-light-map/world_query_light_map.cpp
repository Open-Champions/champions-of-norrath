// Observed worldQueryLightMap__FP12_worldHeaderG6Point3 at 0x0017c368, extent 44 bytes.
// Arguments: a world header and a Point3 (passed as the address of a caller
// copy; the original copies it to its own stack frame and never reads it). The result is the
// float 1.0f in $f0. The world header is not read and nothing is written outside the frame.
// Inferred: a light-map lookup that is a stub in this build (always full brightness), because
// the body ignores both arguments. Unknown: whether another build or mode used the arguments.
struct Point3 {
    float x, y, z;
};
extern "C" {
float worldQueryLightMap__FP12_worldHeaderG6Point3(void* world, Point3 point) {
    (void)world;
    (void)point;
    return 1.0f;
}
}
