// Observed: buildShockChainList__14HateProjectileP6Point3 at 0x001fe790 is 320 bytes in the pinned SLUS-20565 image. O is the
// projectile (first argument) and P the Point3 it is given. The function returns at once when the signed byte at O+386 is nonzero
// and equals the signed byte at O+387. Otherwise the list whose first node is the word at O+80 is walked (each node's next is its
// word at +0 and its object the word at +12). A node with a null object, or whose object has bit 3 of the word at +12 set, is
// skipped. For every other object the squared planar distance d = (x - P.x)^2 + (y - P.y)^2 is formed from the floats at
// object+16 and +20. The object becomes the candidate when d < the float at O+388, d < the best distance so far (initially the
// pool constant with bits 0x4cbeb25c) and alreadyChained__14HateProjectileP10GameObject(O, object) returns zero. With a candidate,
// listOnewayLink(O+84, candidate, candidate) is called, then, if the signed byte at O+386 is nonzero, the byte at O+387 is raised
// by one and the function calls itself on (O, candidate+16) and on (O, P).
// Inferred: the same chain-building step as the shock projectile with different field offsets; a zero limit at O+386 means no
// limit on the walk and no recursion. Unknown: the helpers' bodies (imports). Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
int alreadyChained__14HateProjectileP10GameObject(void* self, void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* data);

void buildShockChainList__14HateProjectileP6Point3(void* self, const Point3* point) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(o[386]) != 0 && static_cast<signed char>(o[387]) == static_cast<signed char>(o[386])) return;
    float best = 99980000.0f;
    unsigned char* candidate = nullptr;
    for (unsigned char* node = from_word(word(o, 80)); node != nullptr; node = from_word(word(node, 0))) {
        unsigned char* const object = from_word(word(node, 12));
        if (object == nullptr || (word(object, 12) & 8u) != 0u) continue;
        const float dx = real(object, 16) - point->x;
        const float dy = real(object, 20) - point->y;
        const float d = dx * dx + dy * dy;
        if (d < real(o, 388) && d < best && alreadyChained__14HateProjectileP10GameObject(self, object) == 0) {
            best = d;
            candidate = object;
        }
    }
    if (candidate == nullptr) return;
    listOnewayLink__FP8ListHeadT0Pv(o + 84, candidate, candidate);
    if (static_cast<signed char>(o[386]) != 0) {
        o[387] = static_cast<unsigned char>(o[387] + 1);
        buildShockChainList__14HateProjectileP6Point3(self, reinterpret_cast<const Point3*>(candidate + 16));
        buildShockChainList__14HateProjectileP6Point3(self, point);
    }
}
}
