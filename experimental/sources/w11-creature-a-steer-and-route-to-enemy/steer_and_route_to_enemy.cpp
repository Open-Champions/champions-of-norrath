// Observed steerAndRouteToEnemy__8Creature at 0x001130b8, 112 bytes.
// When the list head word at +576 is null, virtual slot byte 184 of the table at this+72 is
// called (adjusted this, integer 0) and its result is returned. Otherwise the point at
// object+16 of the object named by the head node's word at +12 is copied and passed by address to
// steerAndRouteToPosition__8CreatureG6Point3, whose result is returned.
// Unknown: what slot 184 does and what the integer 0 selects.
#include "../w11-creature-a-common/words.hpp"

extern "C" int steerAndRouteToPosition__8CreatureG6Point3(void* self, w11_creature_a::Point3 position);

extern "C" int steerAndRouteToEnemy__8Creature(void* self) {
    using namespace w11_creature_a;
    void* head = at<void*>(self, 576);
    if (head == nullptr) {
        const Slot s = slot(self, 184);
        return reinterpret_cast<int (*)(void*, int)>(s.function)(s.self, 0);
    }
    const Point3 point = at<Point3>(at<void*>(head, 12), 16);
    return steerAndRouteToPosition__8CreatureG6Point3(self, point);
}
