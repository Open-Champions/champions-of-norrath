// Observed objectMoveWithWorldCollision__FP6Point3G6Point3fiT0 at 0x00141900, 624 bytes.
// Arguments: a position Point3 pointer (read first, written back at the end), a motion
// Point3 (by address, copied), a float radius, an int mask and an optional output Point3
// pointer. Returns an int: 1 if any pass was deflected, else 0.
// The routine keeps a working position (copy of the position), a working motion (copy of
// the motion) and a remaining fraction that starts at 1.0. Up to eleven passes run, each:
//  1. step = working motion * remaining (componentwise).
//  2. fraction = worldClipMotion(world, working position, step, radius, mask, normal),
//     where `world` is the word at 0x004844a0 and normal is a local Point3.
//  3. remaining -= fraction; working position += step * fraction.
//  4. If remaining is above 0.1 (the float at 0x004802d0, 0x3dcccccd): dot = normal . working
//     motion, push = 0.1 - dot; when push is above zero the flag becomes 1 and the working
//     motion is increased by normal * push.
//  5. The pass count drops (the counter starts at 10 and the loop ends when it reaches -1),
//     and the loop also ends when remaining is no longer above 0.1.
// The position is then overwritten with the working position, the local normal is copied to
// the optional output pointer when it is not null, and the flag is returned.
// Inferred: the local Point3 written by worldClipMotion is a surface normal, because the
// push removes the part of the motion along it; no field name is claimed. The same 0.1 float
// is used for both tests (one register in the image).
// Unknown: the meaning of the mask; the original leaves the local normal unwritten when
// worldClipMotion never reports a block (this version starts it at zero, so the copied
// output differs from stack garbage only in that case).
extern "C" float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* world, const float* position, const float* motion,
                                                                      float radius, int mask, float* out);
extern "C" void* world;

extern "C" int objectMoveWithWorldCollision__FP6Point3G6Point3fiT0(float* position, const float* motion, float radius, int mask,
                                                                   float* out) {
    const float threshold = 0.1f;
    float delta[3] = {motion[0], motion[1], motion[2]};
    float current[3] = {position[0], position[1], position[2]};
    float normal[3] = {0.0f, 0.0f, 0.0f};
    float remaining = 1.0f;
    int deflected = 0;
    int counter = 10;
    for (;;) {
        float step[3] = {delta[0] * remaining, delta[1] * remaining, delta[2] * remaining};
        const float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(world, current, step, radius, mask, normal);
        remaining -= fraction;
        const float moved_x = step[0] * fraction;
        const float moved_y = step[1] * fraction;
        const float moved_z = step[2] * fraction;
        current[0] += moved_x;
        current[1] += moved_y;
        current[2] += moved_z;
        if (threshold < remaining) {
            const float dot = normal[0] * delta[0] + normal[1] * delta[1] + normal[2] * delta[2];
            const float push = threshold - dot;
            if (0.0f < push) {
                deflected = 1;
                const float push_x = normal[0] * push;
                const float push_y = normal[1] * push;
                const float push_z = normal[2] * push;
                delta[0] += push_x;
                delta[1] += push_y;
                delta[2] += push_z;
            }
        }
        if (--counter == -1) break;
        if (!(threshold < remaining)) break;
    }
    position[0] = current[0];
    position[1] = current[1];
    position[2] = current[2];
    if (out != nullptr) {
        out[0] = normal[0];
        out[1] = normal[1];
        out[2] = normal[2];
    }
    return deflected;
}
