// Observed worldCanPass__FP12_worldHeaderG6Point3T1fi at 0x00176e88, 624 bytes. Arguments: a world
// header, a start point and an end point (Point3 values passed by reference in this ABI; the image
// copies them to locals), a radius r and an integer flag. Returns 1 when a body of radius r can
// travel from the start to the end along the flat (x, y) line, 0 when a probe is blocked.
// Method, in the image's order:
//   * step = min(24, 2 * r). The planar offset (dx, dy) = end - start gives the distance
//     d = sqrt(dx * dx + dy * dy) and the unit direction (dx / d, dy / d, 0) (the image forms
//     1 / d once and multiplies). When d <= 12 (also for a NaN distance) the result is 1 at once.
//   * Otherwise a current position starts at the start point and a phase counter p starts at 0.
//     While 12 < d: the current z is set to worldFindStandHeight(world, current, r); a motion is
//     the direction scaled by min(d, p == 0 ? step : 3.0); worldClipMotion(world, current, motion,
//     r, flag, null) gives a fraction f.
//       - f < 1 (blocked): when p is already nonzero the result is 0. Otherwise p becomes 1 and the
//         loop repeats from the same position and distance (so a first block retries in 3.0 steps).
//       - otherwise: when p is nonzero it advances and wraps to 0 after reaching 21 (so up to 20
//         short steps follow a block, then long steps resume). d is reduced by step (always the
//         long step, even after a short or clamped motion), and the motion is added to the
//         current position (x, y, z).
//     When the loop ends the result is 1.
// The image also copies the unit direction to an unused local. Inferred: it is a line-of-walk test
// that probes with the body's radius and retries a blocked probe with small steps, to get around
// small obstacles. Unknown: what the integer flag selects inside the clip routine; the motion's z
// is 0 so the added z is the direction's z times the step (0). Nothing here ran on the EE.
extern "C" {
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius);
float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* world, const float* position, const float* motion,
                                                              float radius, int flag, float* result);
}

extern "C" int worldCanPass__FP12_worldHeaderG6Point3T1fi(void* world, const float* start, const float* end, float radius, int flag) {
    float from[3] = {start[0], start[1], start[2]};
    float to[3] = {end[0], end[1], end[2]};
    float current[3] = {from[0], from[1], from[2]};
    float step_limit = 2.0f * radius;
    float step = 24.0f < step_limit ? 24.0f : step_limit;
    float dx = to[0] - from[0];
    float dy = to[1] - from[1];
    float direction[3] = {dx, dy, 0.0f};
    float distance = __builtin_sqrtf(dx * dx + dy * dy);
    float inverse = 1.0f / distance;
    direction[0] *= inverse;
    direction[1] *= inverse;
    direction[2] *= inverse;
    if (!(12.0f < distance)) return 1;
    int phase = 0;
    float motion[3];
    do {
        current[2] = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, current, radius);
        float length = phase != 0 ? 3.0f : step;
        length = distance < length ? distance : length;
        motion[0] = direction[0] * length;
        motion[1] = direction[1] * length;
        motion[2] = direction[2] * length;
        float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(world, current, motion, radius, flag, nullptr);
        if (fraction < 1.0f) {
            if (phase != 0) return 0;
            phase = 1;
        } else {
            if (phase != 0) {
                phase = phase + 1;
                if (!(phase < 21)) phase = 0;
            }
            distance -= step;
            current[0] += motion[0];
            current[1] += motion[1];
            current[2] += motion[2];
        }
    } while (12.0f < distance);
    return 1;
}
