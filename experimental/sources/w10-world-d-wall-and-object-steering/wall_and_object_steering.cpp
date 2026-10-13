// Observed wallAndObjectSteering__FP8CreatureffP10GameObject at 0x00108618, 1360 bytes.
// Arguments: a hidden return slot for a Point3, the creature, a float distance scale `reach`, a
// second float that is not read, and an object to ignore. Returns the slot's address with the
// Point3 written there (all zeros unless it steers).
//
// Creature fields used: Point3 position at +16, float at +28 (called `span` here), s16 heading at
// +56, int steering state at +772. The probes are worldHitscanWithObjects calls from the global
// `world` with fraction 1.0, the creature itself as the first object, slot array out-parameter
// (five GameObject pointers, zeroed first), mode 3 and flag 1. A negative fraction is replaced by
// 1.0 after each probe.
//   Probe A: from position + (0, 0, 13) along (icos(h) * reach, isin(h) * reach, span).
//   Probe B: start moved by (icos(h + 16384) * span, isin(h + 16384) * span) from the position
//            (z + 13), same direction.      Probe C: the same with h - 16384.
// If none of A, B, C is below 1.0 the state at +772 is cleared and the result stays zero. Otherwise,
// when the state is already 1 or 2 it is used. When it is zero, two more probes are made from
// position + (0, 0, 13) along (icos(h + 16384) * (span + 5), isin(h + 16384) * (span + 5), span)
// (D) and the same with h - 16384 (E), and the smallest of A, B and C selects the state:
//   with an ignore object: nothing changes when A's slot holds it and A equals the smallest, or
//   likewise for B, or for C; else state 2 if D's slot holds it and D is below the smallest, else
//   state 1 if E's slot holds it and E is below the smallest;
//   then, in every case that has not decided, state 2 when E is below 1.0, else state 1 when D is
//   below 1.0, else state 1 when C is not below 1.0, else state 2. The comparisons `A < 1 || B < 1
//   || C < 1` that the image repeats before each step are kept, though they are true there.
// State 1 returns (2 * icos(h - 12000) * span, 2 * isin(h - 12000) * span, 0); state 2 uses
// h + 12000; any other state returns zeros.
// Inferred: a wall/creature avoidance side step. Unknown: Creature and GameObject field names,
// and the meanings of mode 3 / flag 1 for the hitscan.
struct _worldHeader;
struct GameObject;
struct Creature;
struct Point3 { float x, y, z; };
extern _worldHeader* world;
extern "C" {
float icos__Fi(int);
float isin__Fi(int);
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    _worldHeader*, Point3, Point3, float, GameObject*, GameObject**, int, int);
}

namespace {
inline float probe(Point3 start, Point3 dir, GameObject* self, GameObject** slot) {
    float hit = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
        world, start, dir, 1.0f, self, slot, 3, 1);
    return hit < 0.0f ? 1.0f : hit;
}
}

// The compiler used here returns a 12-byte struct in registers, unlike the image, so the hidden
// return slot is written as the explicit first argument; the register layout is the same.
extern "C" Point3* wallAndObjectSteering__FP8CreatureffP10GameObject(Point3* out, Creature* creature, float reach, float, GameObject* ignore) {
    unsigned char* c = reinterpret_cast<unsigned char*>(creature);
    GameObject* const self = reinterpret_cast<GameObject*>(creature);
    Point3 result = {0.0f, 0.0f, 0.0f};
    GameObject* slots[5] = {0, 0, 0, 0, 0};
    const Point3 position = {*reinterpret_cast<float*>(c + 16), *reinterpret_cast<float*>(c + 20), *reinterpret_cast<float*>(c + 24)};
    const float span = *reinterpret_cast<float*>(c + 28);
    const int heading = *reinterpret_cast<short*>(c + 56);
    int& state = *reinterpret_cast<int*>(c + 772);

    Point3 dir = {icos__Fi(heading) * reach, isin__Fi(heading) * reach, span};
    Point3 start = {position.x, position.y, position.z + 13.0f};
    const float a = probe(start, dir, self, &slots[0]);
    start.x = position.x + icos__Fi(heading + 16384) * span;
    start.y = position.y + isin__Fi(heading + 16384) * span;
    const float b = probe(start, dir, self, &slots[1]);
    start.x = position.x + icos__Fi(heading - 16384) * span;
    start.y = position.y + isin__Fi(heading - 16384) * span;
    const float cc = probe(start, dir, self, &slots[2]);

    int current;
    if (a < 1.0f || b < 1.0f || cc < 1.0f) {
        current = state;
        if (current == 0) {
            start = Point3{position.x, position.y, position.z + 13.0f};
            const float least = __builtin_fminf(__builtin_fminf(a, b), cc);
            dir.x = icos__Fi(heading + 16384) * (span + 5.0f);
            dir.y = isin__Fi(heading + 16384) * (span + 5.0f);
            dir.z = span;
            const float d = probe(start, dir, self, &slots[3]);
            dir.x = icos__Fi(heading - 16384) * (span + 5.0f);
            dir.y = isin__Fi(heading - 16384) * (span + 5.0f);
            dir.z = span;
            const float e = probe(start, dir, self, &slots[4]);
            bool decided = false;
            int chosen = 0;
            if (ignore != 0 &&
                ((slots[0] == ignore && a == least) || (slots[1] == ignore && b == least) ||
                 (slots[2] == ignore && cc == least))) {
                decided = true;
            }
            if (!decided) {
                if (slots[3] == ignore && d < least) { chosen = 2; decided = true; }
                else if (slots[4] == ignore && e < least) { chosen = 1; decided = true; }
            }
            if (!decided) {
                if ((a < 1.0f || cc < 1.0f || b < 1.0f) && e < 1.0f) chosen = 2;
                else if ((a < 1.0f || cc < 1.0f || b < 1.0f) && d < 1.0f) chosen = 1;
                else if (!(cc < 1.0f)) chosen = 1;
                else chosen = 2;
            }
            if (chosen != 0) { state = chosen; current = chosen; }
        }
    } else {
        state = 0;
        *out = result;
        return out;
    }
    if (current == 1) {
        result.x = icos__Fi(heading - 12000) * span * 2.0f;
        result.y = isin__Fi(heading - 12000) * span * 2.0f;
    } else if (current == 2) {
        result.x = icos__Fi(heading + 12000) * span * 2.0f;
        result.y = isin__Fi(heading + 12000) * span * 2.0f;
    }
    *out = result;
    return out;
}
