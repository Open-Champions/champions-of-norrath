// Observed RandomTeleportSpell__FP10GameObjectP6Point3f at 0x00108b68, 752 bytes. Arguments: an
// object, an optional target point (may be null) and a float reach r. It moves the object along a
// ray in a randomly chosen turning direction to the farthest clear spot within reach.
//   * One step of syncRandState (state = state * 0x19660d + 0x3c6ef35f) is taken; bit 16 of the new
//     value picks the turn step: -2048 when set, else +2048 (angle units of 1/65536 turn).
//   * The probe starts at the object's position (+16, +20, +24) with z raised by 12.0. The angle
//     starts at the object's 16-bit heading at +56. Up to 32 probes: the end point is the start
//     plus (icos(angle) * r, isin(angle) * r) at the raised z; worldHitscan(world, start, end - start
//     with z 0, 1200.0, 0) gives a fraction, taken as 1.0 when it is below 0 or above 1. When the
//     fraction is above the best so far (initially 0) the point at that fraction along the ray is
//     tested with worldFindHeight(world, point, object radius at +28); the probe is kept (best
//     fraction and angle) only when that height lies strictly within 12.0 of the object z at +24.
//     The search stops at the first probe whose kept fraction is exactly 1.0, otherwise the angle
//     turns by the step (wrapping to 16 bits).
//   * The object is moved to its position plus (icos(best) * r * best, isin(best) * r * best) with its
//     own z, objectUpdateInGrid is called, and when a target was given the heading at +56 becomes
//     iatan2(target.y - object y, target.x - object x).
// Inferred: a random-direction blink limited by walls and floor height. Unknown: what the spell's
// caller does with the result (nothing is returned) and the meaning of 1200.0 and the mode 0.
// Nothing here ran on the EE.
extern "C" {
extern int syncRandState;
extern void* world;
float icos__Fi(int angle);
float isin__Fi(int angle);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, const float* start, const float* direction, float max_distance, int mode);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, const float* point, float radius);
void objectUpdateInGrid__FP10GameObject(void* object);
int iatan2__Fff(float y, float x);
}

extern "C" void RandomTeleportSpell__FP10GameObjectP6Point3f(unsigned char* object, const float* target, float reach) {
    float* position = reinterpret_cast<float*>(object + 16);
    syncRandState = syncRandState * 0x19660d + 0x3c6ef35f;
    int step = ((syncRandState >> 16) & 1) ? -2048 : 2048;
    short best_angle = *reinterpret_cast<short*>(object + 56);
    short angle = best_angle;
    float best = 0.0f;
    float start[3] = {position[0], position[1], position[2]};
    float end[3];
    float delta[3];
    start[2] = start[2] + 12.0f;
    for (int i = 0; i < 32; ++i) {
        end[0] = start[0] + icos__Fi(angle) * reach;
        end[1] = start[1] + isin__Fi(angle) * reach;
        end[2] = start[2];
        delta[0] = end[0] - start[0];
        delta[1] = end[1] - start[1];
        delta[2] = 0.0f;
        float fraction = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, start, delta, 1200.0f, 0);
        if (fraction < 0.0f || 1.0f < fraction) fraction = 1.0f;
        if (best < fraction) {
            end[0] = start[0] + icos__Fi(angle) * reach * fraction;
            end[1] = start[1] + isin__Fi(angle) * reach * fraction;
            end[2] = start[2];
            float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, end, *reinterpret_cast<float*>(object + 28));
            float z = *reinterpret_cast<float*>(object + 24);
            if (z - 12.0f < height && height < z + 12.0f) {
                best = fraction;
                best_angle = angle;
            }
        }
        if (best == 1.0f) break;
        angle = static_cast<short>(angle + step);
    }
    end[0] = position[0] + icos__Fi(best_angle) * reach * best;
    end[1] = position[1] + isin__Fi(best_angle) * reach * best;
    end[2] = position[2];
    position[0] = end[0];
    position[1] = end[1];
    position[2] = end[2];
    objectUpdateInGrid__FP10GameObject(object);
    if (target) *reinterpret_cast<short*>(object + 56) = static_cast<short>(iatan2__Fff(target[1] - position[1], target[0] - position[0]));
}
