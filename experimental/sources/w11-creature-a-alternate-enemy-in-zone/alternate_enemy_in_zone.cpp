// Observed alternateEnemyInZone__8Creatureffss at 0x0010aac0, 1268 bytes.
// Arguments: the creature, singles near (f12) and far (f13), halfwords centre and width.
// Returns the chosen object or null. The creature's signed halfword at +792 selects the source of
// candidates.
// Non-zero halfword: with reach the product of far and the double 1.4 (0x3ff6666666666666) taken
// through the image's soft-double fptodp, dpmul and dptofp, a box from the creature position (+16)
// minus reach to the position plus reach, per axis, is passed with a 128-entry output array and
// the capacity 128 to objectFindInBox__FG6Point3T0PP10GameObjecti (the array is not cleared first); the walk runs
// until a null entry. For each object o a score is the result of the virtual function at byte 4 of
// the table entry at (table at +72) + 160 called with o as the argument, on the creature adjusted
// by the entry's signed halfword at byte 0. A zero score skips o. Otherwise, with dsq the squared
// distance (3-D) between the creature and o: dsq < near*near skips o. d = o's position minus the
// creature's, except that d.z compares o.z + 0.5*o[+32] against creature.z + 0.5*creature[+32].
// With h = (short)(16384 - iatan2__Fff(d.x, d.y)) - centre truncated to 16 bits, o is skipped when
// width < |h|. A hitscan worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, d, 1.1, 3) from
// the creature position with z raised by the creature's single at +32 skips o when its result is
// from 0.0 to 1.0 inclusive. The score then gains 1 when dsq < best (best starts as far*far), 1
// when o is the object of the node at +752 (if any), and 1 when o is the object of the node at
// +576 (if any). o becomes the answer, with best = dsq and its score kept, when its score is
// greater than the kept score (starting 0) or equal with dsq < best.
// Zero halfword: the nmPlayers entries of the player table are scanned (the object pointer at byte
// 21872 of each 23984-byte record; a null pointer, or an object whose word at +12 has bit 0x8, is
// skipped). The heading test uses d as above and the same iatan2 form but needs |h| < width. The
// horizontal distance to the player must satisfy distance < best (starting at far) and
// near - r <= distance < far + r (r being the player's single at +28; note far, not best, is used
// here), and a hitscan from the creature position (z raised by creature +32) with length 1.1 and
// mask 3 must give a result below 0.0 or above 1.0. A player that passes becomes the answer and
// best = distance.
// All three 1.1 literals are the image's 0x3f8ccccc.
// Inferred: the virtual call at table byte 160 judges a candidate (suitability), so the highest
// scoring, then nearest, object wins.
// Unknown: the meaning of that virtual call, the halfword at +792 and the nodes at +752 and +576
// beyond the node at +576 being the enemy list head used by enemyInZone.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern void* world;
extern int nmPlayers;
extern unsigned char player[];
int iatan2__Fff(float x, float y);
void objectFindInBox__FG6Point3T0PP10GameObjecti(w11_creature_a::Point3 minimum, w11_creature_a::Point3 maximum, void** output, int capacity);
unsigned long long fptodp(float value);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* header, w11_creature_a::Point3 from, w11_creature_a::Point3 direction, float length, int mask);
}

extern "C" void* alternateEnemyInZone__8Creatureffss(void* self, float near_limit, float far_limit, short centre, short width) {
    using namespace w11_creature_a;
    void* best = nullptr;
    if (at<short>(self, 792) != 0) {
        void* found[128];
        const float far_squared = far_limit * far_limit;
        const float reach = dptofp(dpmul(fptodp(far_limit), 0x3ff6666666666666ull));
        Point3 minimum, maximum;
        const Point3 origin = at<Point3>(self, 16);
        minimum.x = origin.x - reach;
        minimum.y = origin.y - reach;
        minimum.z = origin.z - reach;
        maximum.x = origin.x + reach;
        maximum.y = origin.y + reach;
        maximum.z = origin.z + reach;
        objectFindInBox__FG6Point3T0PP10GameObjecti(minimum, maximum, found, 128);
        int best_score = 0;
        float best_distance = far_squared;
        for (void** entry = found; *entry != nullptr; ++entry) {
            void* object = *entry;
            Slot judge = slot(self, 160);
            int score = reinterpret_cast<int (*)(void*, void*)>(judge.function)(judge.self, object);
            if (score == 0) continue;
            const Point3 position = at<Point3>(self, 16);
            const Point3 other = at<Point3>(object, 16);
            const float ex = position.x - other.x;
            const float ey = position.y - other.y;
            const float ez = position.z - other.z;
            const float distance = ex * ex + ey * ey + ez * ez;
            if (distance < near_limit * near_limit) continue;
            Point3 direction;
            direction.x = other.x - position.x;
            direction.y = other.y - position.y;
            direction.z = (other.z + at<float>(object, 32) * 0.5f) - (position.z + at<float>(self, 32) * 0.5f);
            int difference = static_cast<short>(static_cast<short>(16384 - iatan2__Fff(direction.x, direction.y)) - centre);
            if (difference < 0) difference = -difference;
            if (width < difference) continue;
            Point3 from = at<Point3>(self, 16);
            from.z = from.z + at<float>(self, 32);
            const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, __builtin_bit_cast(float, 0x3f8ccccc), 3);
            if (0.0f <= hit && hit <= 1.0f) continue;
            if (distance < best_distance) ++score;
            if (at<void*>(self, 752) != nullptr && object == at<void*>(at<void*>(self, 752), 12)) ++score;
            if (at<void*>(self, 576) != nullptr && object == at<void*>(at<void*>(self, 576), 12)) ++score;
            if (best_score < score || (score == best_score && distance < best_distance)) {
                best = object;
                best_distance = distance;
                best_score = score;
            }
        }
        return best;
    }
    float best_distance = far_limit;
    for (int index = 0; index < nmPlayers; ++index) {
        void* target = at<void*>(player + index * 23984, 21872);
        if (target == nullptr) continue;
        if ((at<unsigned>(target, 12) & 0x8u) != 0) continue;
        const Point3 position = at<Point3>(self, 16);
        Point3 direction;
        direction.x = at<float>(target, 16) - position.x;
        direction.y = at<float>(target, 20) - position.y;
        direction.z = (at<float>(target, 24) + at<float>(target, 32) * 0.5f) - (position.z + at<float>(self, 32) * 0.5f);
        int difference = static_cast<short>(static_cast<short>(16384 - iatan2__Fff(direction.x, direction.y)) - centre);
        if (difference < 0) difference = -difference;
        if (!(difference < width)) continue;
        target = at<void*>(player + index * 23984, 21872);
        const float dx = position.x - at<float>(target, 16);
        const float dy = position.y - at<float>(target, 20);
        const float distance = __builtin_sqrtf(dx * dx + dy * dy);
        if (!(distance < best_distance)) continue;
        const float radius = at<float>(target, 28);
        if (!(distance < far_limit + radius)) continue;
        if (!(near_limit - radius <= distance)) continue;
        Point3 from = at<Point3>(self, 16);
        from.z = from.z + at<float>(self, 32);
        const float hit = worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, direction, __builtin_bit_cast(float, 0x3f8ccccc), 3);
        if (hit < 0.0f || 1.0f < hit) {
            best_distance = distance;
            best = at<void*>(player + index * 23984, 21872);
        }
    }
    return best;
}
