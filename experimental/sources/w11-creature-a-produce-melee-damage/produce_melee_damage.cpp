// Observed produceMeleeDamage__8CreaturefiiiiPQ24item11description at 0x00112e18, 360 bytes.
// Arguments: the creature, a single reach, four ints p1..p4 and an item description pointer.
// With a null list head word at +576 the result is 0. Otherwise the target is the object named by
// the head node's word at +12. A strike point is formed from the creature position (+16) moved by
// reach along the heading, the signed halfword at +56: x += icos(heading) * reach,
// y += isin(heading) * reach, z unchanged. The head is read a second time after the two trig
// calls. When the distance from the strike point to that object's point (+16) is below 60.0 and
// bit 0x8 of that object's word at +12 is clear, the result is
// gameAttackResolver(first target, creature, (float)p1, (float)p2, p3, 0, 0, p4, description, 0)
// and is returned; otherwise the result is 0.
// Inferred: the callee is the attack resolver named by its symbol; the integer result is returned
// unchanged. Unknown: the meaning of p1..p4, the 60.0 limit's unit, and bit 0x8.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* description, bool g);
}

extern "C" int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void* self, float reach, int p1, int p2, int p3, int p4, void* description) {
    using namespace w11_creature_a;
    int result = 0;
    if (at<void*>(self, 576) != nullptr) {
        void* const target = at<void*>(at<void*>(self, 576), 12);
        Point3 strike;
        strike.x = at<float>(self, 16) + icos__Fi(at<short>(self, 56)) * reach;
        strike.y = at<float>(self, 20) + isin__Fi(at<short>(self, 56)) * reach;
        strike.z = at<float>(self, 24);
        void* const current = at<void*>(at<void*>(self, 576), 12);
        const float dx = at<float>(current, 16) - strike.x;
        const float dy = at<float>(current, 20) - strike.y;
        const float dz = at<float>(current, 24) - strike.z;
        if (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) < 60.0f && (at<unsigned>(current, 12) & 0x8u) == 0u) {
            result = gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                target, self, static_cast<float>(p1), static_cast<float>(p2), p3, 0, 0, p4, description, false);
        }
    }
    return result;
}
