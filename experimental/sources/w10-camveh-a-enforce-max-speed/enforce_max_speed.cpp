// Observed enforceMaxSpeed__6CameraP6Point3 at 0x0026fdf0, 148 bytes. Clamps
// the three floats of the Point3 argument in place: x and y into [-3.0, 3.0],
// z into [-1.0, 1.0]. A value above the upper limit is replaced first; only
// otherwise is it tested against the lower limit, so a NaN is left alone. The
// camera argument is not read. Nothing is returned. Nothing here was executed
// on the EE. That the Point3 is a speed is inferred from the name only.
typedef float AliasedFloat __attribute__((may_alias));

static inline void clamp(AliasedFloat* value, float limit) {
    const float v = *value;
    if (limit < v) {
        *value = limit;
    } else if (v < -limit) {
        *value = -limit;
    }
}

extern "C" void enforceMaxSpeed__6CameraP6Point3(void* camera, AliasedFloat* point) {
    (void)camera;
    clamp(point + 0, 3.0f);
    clamp(point + 1, 3.0f);
    clamp(point + 2, 1.0f);
}
