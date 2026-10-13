// Observed produceGeneralDamage__8CreatureffssiiiiPQ24item11description at 0x00112f80, 308 bytes.
// Arguments: the creature, singles a and b, halfwords s1 and s2, four ints i1..i4 and an item
// description pointer. The target comes from one of two searches:
//   when the list head word at +576 is non-null, the point (+16) of the object named by the head
//   node's word at +12 is copied and pointInZone__8CreatureG6Point3ffss(creature, point, a, b, s1,
//   s2) is called; when it answers nonzero the target is that head node's object (the head is
//   read again after the call);
//   otherwise (null head or a zero answer) the target is
//   alternateEnemyInZone__8Creatureffss(creature, a, b, s1, s2).
// With a nonzero target the result is
// gameAttackResolver(target, creature, (float)i1, (float)i2, i3, 0, 0, i4, description, 0) and
// is returned; with no target the result is 0.
// Unknown: what a, b, s1 and s2 select inside the zone tests and what the ints mean.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
int pointInZone__8CreatureG6Point3ffss(void* self, w11_creature_a::Point3 point, float a, float b, short s1, short s2);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short s1, short s2);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* description, bool g);
}

extern "C" int produceGeneralDamage__8CreatureffssiiiiPQ24item11description(void* self, float a, float b, short s1, short s2, int i1, int i2, int i3, int i4, void* description) {
    using namespace w11_creature_a;
    void* target = nullptr;
    bool found = false;
    if (at<void*>(self, 576) != nullptr) {
        const Point3 point = at<Point3>(at<void*>(at<void*>(self, 576), 12), 16);
        if (pointInZone__8CreatureG6Point3ffss(self, point, a, b, s1, s2) != 0) {
            target = at<void*>(at<void*>(self, 576), 12);
            found = true;
        }
    }
    if (!found) target = alternateEnemyInZone__8Creatureffss(self, a, b, s1, s2);
    int result = 0;
    if (target != nullptr) {
        result = gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
            target, self, static_cast<float>(i1), static_cast<float>(i2), i3, 0, 0, i4, description, false);
    }
    return result;
}
