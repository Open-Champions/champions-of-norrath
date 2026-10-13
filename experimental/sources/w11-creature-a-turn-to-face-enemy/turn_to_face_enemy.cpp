// Observed turnToFaceEnemy__8Creature at 0x00112a98, 96 bytes.
// When the list head word at +576 is null nothing is called. Otherwise the object named by the
// head node's word at +12 supplies its point (the 12 bytes at object+16); a copy of it is passed
// by address, with the integer 0, to virtual slot byte 176 of the table at this+72 (adjusted
// this). The function always returns 0.
// Inferred: slot 176 is the turn-toward-point method (its argument shape matches
// turnToFace__8CreatureiG6Point3). Unknown: the meaning of the integer 0.
#include "../w11-creature-a-common/words.hpp"

extern "C" int turnToFaceEnemy__8Creature(void* self) {
    using namespace w11_creature_a;
    void* head = at<void*>(self, 576);
    if (head != nullptr) {
        const Slot s = slot(self, 176);
        void* enemy = at<void*>(head, 12);
        const Point3 point = at<Point3>(enemy, 16);
        reinterpret_cast<void (*)(void*, int, Point3)>(s.function)(s.self, 0, point);
    }
    return 0;
}
