// Observed: buildShockChainList__15ShockProjectileP6Point3 at 0x0028d5e8 is 308 bytes in the pinned SLUS-20565 image. O is the
// projectile (first argument) and P the Point3 it is given. The signed bytes at O+395 and O+394 are compared first and the function
// returns when they are equal. Otherwise the list whose first node is the word at O+80 is walked (each node's next is its word at
// +0 and its object the word at +12). A node with a null object, or whose object has bit 3 of the word at +12 set, is skipped. For
// every other object the squared planar distance d = (x - P.x)^2 + (y - P.y)^2 is formed from the floats at object+16 and +20. The
// object becomes the candidate when d < the float at O+396, d < the best distance so far (initially the pool constant with bits
// 0x4cbeb25c) and alreadyChained__15ShockProjectileP10GameObject(O, object) returns zero. With a candidate,
// listOnewayLink(O+84, candidate, candidate) is called, then, if the signed byte at O+394 is nonzero, the byte at O+395 is raised
// by one and the function calls itself on (O, candidate+16) and on (O, P).
// Inferred: this builds the chain of objects a shock arc jumps between; O+394 is the chain length limit, O+395 the count so far
// and O+396 a squared range. Unknown: what the ignored objects with bit 3 are, and the helpers' bodies (imports). Nothing here ran
// on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" {
int alreadyChained__15ShockProjectileP10GameObject(void* self, void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* data);

void buildShockChainList__15ShockProjectileP6Point3(void* self, const Point3* point) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(o[395]) == static_cast<signed char>(o[394])) return;
    float best = 99980000.0f;
    unsigned char* candidate = nullptr;
    for (unsigned char* node = from_word(word(o, 80)); node != nullptr; node = from_word(word(node, 0))) {
        unsigned char* const object = from_word(word(node, 12));
        if (object == nullptr || (word(object, 12) & 8u) != 0u) continue;
        const float dx = real(object, 16) - point->x;
        const float dy = real(object, 20) - point->y;
        const float d = dx * dx + dy * dy;
        if (d < real(o, 396) && d < best && alreadyChained__15ShockProjectileP10GameObject(self, object) == 0) {
            best = d;
            candidate = object;
        }
    }
    if (candidate == nullptr) return;
    listOnewayLink__FP8ListHeadT0Pv(o + 84, candidate, candidate);
    if (static_cast<signed char>(o[394]) != 0) {
        o[395] = static_cast<unsigned char>(o[395] + 1);
        buildShockChainList__15ShockProjectileP6Point3(self, reinterpret_cast<const Point3*>(candidate + 16));
        buildShockChainList__15ShockProjectileP6Point3(self, point);
    }
}
}
