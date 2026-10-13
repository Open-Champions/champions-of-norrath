// Observed findEnemyLeadPosition__8CreaturefP6Point3 at 0x00113b40, 208 bytes.
// Arguments: the creature, a single divisor and an optional Point3 output pointer. With a null
// list head word at +576 the result is 0 and nothing is written. Otherwise the enemy is the
// object named by the head node's word at +12 and its point (+16) is copied. With
// t = sqrt(dx*dx + dy*dy) / divisor, where dx and dy are the point's x and y minus the creature's
// x and y, the copy is moved: x += enemy[+816] * t, y += enemy[+820] * t, z += enemy[+32] * 0.5.
// The copy is stored through the output pointer when it is not null, and the result is 1.
// Inferred: +816 and +820 are the enemy's x and y velocity and +32 a z rate, per their use here
// (the same fields creatureFindLeadYaw reads). Unknown: why z uses half the single at +32.
#include "../w11-creature-a-common/words.hpp"

extern "C" int findEnemyLeadPosition__8CreaturefP6Point3(void* self, float divisor, w11_creature_a::Point3* output) {
    using namespace w11_creature_a;
    void* head = at<void*>(self, 576);
    if (head == nullptr) return 0;
    void* enemy = at<void*>(head, 12);
    Point3 point = at<Point3>(enemy, 16);
    const float dx = point.x - at<float>(self, 16);
    const float dy = point.y - at<float>(self, 20);
    const float t = __builtin_sqrtf(dx * dx + dy * dy) / divisor;
    point.x = point.x + at<float>(enemy, 816) * t;
    point.y = point.y + at<float>(enemy, 820) * t;
    point.z = point.z + at<float>(enemy, 32) * 0.5f;
    if (output != nullptr) *output = point;
    return 1;
}
