#include "../w9-animation-object-c-common/mem.hpp"
// Observed steerAndRouteToPosition__8CreatureG6Point3 at 0x001102b8, 1572 bytes. Arguments: the
// creature and a 12-byte target point T (copied). Creature fields used: position P at +16 (z at
// +24), radius r (float) at +28, signed halfword heading at +56, vtable word at +72, float speed
// at +176, floats at +180 and +184, signed bytes at +569 and +570, unsigned byte at +800, ints
// at +712, +732, +736, +740 and +808, a point Q at +720 and a steering point at +760. Returns 1
// when it considers the creature to have reached T, otherwise 0. The global frame counter
// frameNm (0x4844a4) is called "frame" below.
//  1. D = T - P and dist = sqrt((Dx*Dx + Dy*Dy) + Dz*Dz). Returns 1 when dist < r.
//  2. When ((frame ^ byte569) & 7) == 0 and dist < 2r, takes iatan2(Dy, Dx) minus the heading
//     (32-bit subtraction); returns 1 when its magnitude is 16385 or more.
//  3. clearance = worldHitscan(world, (Px, Py, Pz + 13.0), D, 1.0999999, 3).
//  4. When the signed byte at +570 is zero and ((frame ^ byte569) & 7) == 0, the steering point
//     at +760 is replaced by wallAndObjectSteering(creature, min(dist, r + 10.0), 2r, no object).
//  5. When both the x and y floats of the steering point are nonzero, the route point R becomes
//     steering + P. Otherwise: if int+808 < int+740, clearance becomes 0.5 when int+736 is
//     nonzero and 1.5 when it is zero; then, only when 0.0 <= clearance <= 1.0, R starts as
//     getCurrentRoutePoint(creature) and up to 16 times: R.x == -1.0, or the planar distance
//     from P to R below r, or int+712 below frame, makes it store frame + 180 in int+712 and
//     replace R with getNextRoutePoint(creature, &R, T, r, 0.0, 0); the loop ends early when R.x
//     is -1.0 or no replacement was needed. If R.x is still -1.0 afterwards R becomes T. (R
//     starts as T in every case.)
//  6. Virtual slot 176 (this adjusted by the slot's signed halfword, 0, &R) returns a short
//     angle; when its magnitude is 16001 or more the floats at +180 and +184 and the speed
//     s = float at +176 are halved. s is then scaled by (60 - signed byte800) * 0.016666668
//     when byte800 is 1..60.
//  7. move2(creature, (icos(heading) * s, isin(heading) * s, 0)); its result is unused.
//  8. If cutMode (0x484318) is nonzero or 5.0 < squared distance between P and the point at
//     +720: that point becomes P, int+732 becomes int+808, and it returns 0. Otherwise with
//     a = int+740 and b = int+808: a < b and int+732 < b - 300 sets int+740 = b + 240 and
//     int+736 to 0 if 0.0 <= clearance <= 1.0 else 1; a == b and int+732 < a - 300 asks
//     objectFindEmptySpot(P, 72, (int)r, &found) and, when found is nonzero, moves P there and
//     runs objectUpdateInGrid; every other case does nothing. These paths return 0.
// Inferred: this steers a creature toward a point along an avoidance route, with a stuck
// timer in the integer fields; names are not claimed.
// Unknown: what the integer fields and the vtable slot mean, and what bit patterns of +569 and
// +570 select.
extern "C" {
extern unsigned char* world;
extern int frameNm;
extern signed char cutMode;
float worldHitscan__FP12_worldHeaderG6Point3T1fi(unsigned char* world_header, const float* origin, const float* delta, float limit, int flags);
int iatan2__Fff(float y, float x);
void wallAndObjectSteering__FP8CreatureffP10GameObject(float* result, unsigned char* self, float a, float b, unsigned char* object);
void getCurrentRoutePoint__8Creature(float* result, unsigned char* self);
void getNextRoutePoint__8CreatureP6Point3G6Point3ffi(unsigned char* self, float* point, const float* target, float a, float b, int c);
float icos__Fi(int angle);
float isin__Fi(int angle);
int move2__8CreatureG6Point3(unsigned char* self, const float* push);
unsigned char* objectFindEmptySpot__FG6Point3iiPc(float* result, const float* centre, int spread, int radius, char* found);
void objectUpdateInGrid__FP10GameObject(unsigned char* object);
}

extern "C" int steerAndRouteToPosition__8CreatureG6Point3(unsigned char* self, const unsigned char* target_in) {
    using namespace norrath_aoc;
    float target[3];
    target[0] = load_float(target_in);
    target[1] = load_float(target_in + 4);
    target[2] = load_float(target_in + 8);
    float route[3] = {target[0], target[1], target[2]};
    unsigned char* const position = self + 16;
    float delta[3];
    delta[0] = target[0] - load_float(position);
    delta[1] = target[1] - load_float(position + 4);
    delta[2] = target[2] - load_float(position + 8);
    const float radius = load_float(self + 28);
    const float dist = square_root((delta[0] * delta[0] + delta[1] * delta[1]) + delta[2] * delta[2]);
    if (dist < radius) return 1;
    const bool phase = ((frameNm ^ load_signed_byte(self + 569)) & 7) == 0;
    if (phase && dist < radius + radius) {
        const int turn = iatan2__Fff(delta[1], delta[0]) - load_signed_half(self + 56);
        if (!((turn < 0 ? -turn : turn) < 16385)) return 1;
    }
    float origin[3];
    origin[0] = load_float(position);
    origin[1] = load_float(position + 4);
    origin[2] = load_float(self + 24) + 13.0f;
    float clearance = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, origin, delta, 0x1.199998p+0f, 3);
    if (load_signed_byte(self + 570) == 0 && phase) {
        float steer[3];
        const float reach = radius + 10.0f;
        wallAndObjectSteering__FP8CreatureffP10GameObject(steer, self, dist < reach ? dist : reach, radius + radius, nullptr);
        store_float(self + 760, steer[0]);
        store_float(self + 764, steer[1]);
        store_float(self + 768, steer[2]);
    }
    if (load_float(self + 760) != 0.0f && load_float(self + 764) != 0.0f) {
        route[0] = load_float(self + 760) + load_float(position);
        route[1] = load_float(self + 764) + load_float(position + 4);
        route[2] = load_float(self + 768) + load_float(position + 8);
    } else {
        if (static_cast<int>(load_word(self + 808)) < static_cast<int>(load_word(self + 740)))
            clearance = load_word(self + 736) != 0 ? 0.5f : 1.5f;
        if (0.0f <= clearance && clearance <= 1.0f) {
            getCurrentRoutePoint__8Creature(route, self);
            for (int attempts = 0; attempts < 16; ++attempts) {
                float reach = radius;
                if (route[0] != -1.0f) {
                    const float ax = route[0] - load_float(position);
                    const float ay = route[1] - load_float(position + 4);
                    reach = load_float(self + 28);
                    if (!(square_root(ax * ax + ay * ay) < reach) && !(static_cast<int>(load_word(self + 712)) < frameNm)) break;
                } else {
                    reach = load_float(self + 28);
                }
                store_word(self + 712, static_cast<unsigned>(frameNm + 180));
                getNextRoutePoint__8CreatureP6Point3G6Point3ffi(self, route, target, reach, 0.0f, 0);
                if (route[0] == -1.0f) break;
            }
            if (route[0] == -1.0f) { route[0] = target[0]; route[1] = target[1]; route[2] = target[2]; }
        }
    }
    float speed;
    {
        const int turn = static_cast<short>(call_virtual_int_pointer_returning_int(self, 176, 0, route));
        speed = load_float(self + 176);
        if (!((turn < 0 ? -turn : turn) < 16001)) {
            speed = speed * 0.5f;
            store_float(self + 180, load_float(self + 180) * 0.5f);
            store_float(self + 184, load_float(self + 184) * 0.5f);
        }
    }
    const unsigned shade = self[800];
    if (shade - 1u < 60u)
        speed = speed * (static_cast<float>(60 - static_cast<signed char>(shade)) * 0x1.111110p-6f);
    float push[3];
    push[0] = icos__Fi(load_signed_half(self + 56)) * speed;
    push[1] = isin__Fi(load_signed_half(self + 56)) * speed;
    push[2] = 0.0f;
    move2__8CreatureG6Point3(self, push);
    unsigned char* const last = self + 720;
    const float lx = load_float(position) - load_float(last);
    const float ly = load_float(position + 4) - load_float(last + 4);
    const float lz = load_float(position + 8) - load_float(last + 8);
    if (cutMode != 0 || 5.0f < (lx * lx + ly * ly) + lz * lz) {
        store_word(last, load_word(position));
        store_word(last + 4, load_word(position + 4));
        store_word(last + 8, load_word(position + 8));
        store_word(self + 732, load_word(self + 808));
        return 0;
    }
    const int a = static_cast<int>(load_word(self + 740));
    const int b = static_cast<int>(load_word(self + 808));
    if (a < b) {
        if (static_cast<int>(load_word(self + 732)) < b - 300) {
            store_word(self + 740, static_cast<unsigned>(b + 240));
            store_word(self + 736, (0.0f <= clearance && clearance <= 1.0f) ? 0u : 1u);
        }
        return 0;
    }
    if (a != b) return 0;
    if (!(static_cast<int>(load_word(self + 732)) < a - 300)) return 0;
    char found = 0;
    float here[3] = {load_float(position), load_float(position + 4), load_float(position + 8)};
    float spot[3];
    objectFindEmptySpot__FG6Point3iiPc(spot, here, 72, static_cast<int>(load_float(self + 28)), &found);
    if (found == 0) return 0;
    store_float(position, spot[0]);
    store_float(position + 4, spot[1]);
    store_float(position + 8, spot[2]);
    objectUpdateInGrid__FP10GameObject(self);
    return 0;
}
