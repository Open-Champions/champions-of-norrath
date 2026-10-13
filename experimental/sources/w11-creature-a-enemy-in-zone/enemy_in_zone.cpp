// Observed enemyInZone__8Creaturefs at 0x00112c68, 64 bytes.
// Arguments: the creature, a single and a signed halfword. When the list head word at +576 is
// null the result is 0. Otherwise the object named by the head node's word at +12 is passed to
// objectInZone__8CreatureP10GameObjectfs together with the same single and halfword, and the
// result is 1 when that call returned nonzero and 0 otherwise.
// Inferred: the +576 head is the current enemy list (the same field updateEnemy maintains).
// Unknown: what the single and the halfword select inside objectInZone.
#include "../w11-creature-a-common/words.hpp"

extern "C" int objectInZone__8CreatureP10GameObjectfs(void* self, void* other, float range, short zone);

extern "C" int enemyInZone__8Creaturefs(void* self, float range, short zone) {
    using namespace w11_creature_a;
    void* head = at<void*>(self, 576);
    if (head == nullptr) return 0;
    return objectInZone__8CreatureP10GameObjectfs(self, at<void*>(head, 12), range, zone) != 0;
}
