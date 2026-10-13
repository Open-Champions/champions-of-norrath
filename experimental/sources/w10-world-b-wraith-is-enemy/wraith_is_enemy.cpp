#include "../w10-world-b-common/mem.hpp"
// Observed isEnemy__6WraithP10GameObject at 0x00204ff8, 432 bytes in SLUS_205.65. Arguments:
// the Wraith and another object. Walks the first _6Wraith$s_wraithCount (0x004852a8) slots of
// the pointer array _6Wraith$s_rWraithList (0x0042bd18, 60 pointers). For a non-null slot p
// let w be the word at p+12 (an object):
//  * w equal to the Wraith itself: remember that the Wraith was seen (found = 1); next slot;
//  * otherwise w's word at +576 must be non-null, and the word at +12 of what it points to
//    must be the other object; if either test fails go on to the next slot;
//  * then dx/dy are taken in the x-y plane (floats at +16 and +20): nearDist = distance from
//    the Wraith to the other object, farDist = distance from w to the other object, each
//    sqrt(dx*dx + dy*dy). If nearDist * 3.0f < farDist, or nearDist < farDist with the Wraith already found,
//    the list at w+576 is destroyed with listDestroy__FP8ListHead(w + 576), the count is read
//    again and the word at w+576 is set to zero; the walk then continues. Any other outcome
//    ends the whole call with 0.
// A null other object, a count of zero or less, or the walk running to its end, returns
// isEnemy__8CreatureP10GameObject(self, other) (a tail call in the image).
// Inferred: the array holds the Wraiths currently alive; the +576 word is a list owned by a
// Wraith that is attached to one target; the routine drops a rival Wraith's claim when this
// Wraith is much closer. Unknown: the real meaning of +576 and the types stored there.
extern "C" void listDestroy__FP8ListHead(unsigned char* head);
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char* self, unsigned char* other);
extern unsigned char* _6Wraith$s_rWraithList[60];
extern int _6Wraith$s_wraithCount;

extern "C" int isEnemy__6WraithP10GameObject(unsigned char* self, unsigned char* other) {
    using namespace norrath_wb;
    int found = 0;
    if (other != 0) {
        int count = _6Wraith$s_wraithCount;
        for (int i = 0; i < count; ++i) {
            unsigned char* slot = _6Wraith$s_rWraithList[i];
            if (slot == 0) continue;
            unsigned char* w = load_pointer(slot + 12);
            if (w == self) {
                found = 1;
                continue;
            }
            unsigned char* claim = load_pointer(w + 576);
            if (claim == 0) continue;
            if (load_pointer(claim + 12) != other) continue;
            const float sx = load_float(self + 16) - load_float(other + 16);
            const float sy = load_float(self + 20) - load_float(other + 20);
            const float wx = load_float(w + 16) - load_float(other + 16);
            const float wy = load_float(w + 20) - load_float(other + 20);
            const float nearDist = __builtin_sqrtf(sx * sx + sy * sy);
            const float farDist = __builtin_sqrtf(wx * wx + wy * wy);
            if (!(nearDist * 3.0f < farDist)) {
                if (!(nearDist < farDist)) return 0;
                if (found != 1) return 0;
            }
            listDestroy__FP8ListHead(w + 576);
            count = _6Wraith$s_wraithCount;
            store_pointer(load_pointer(_6Wraith$s_rWraithList[i] + 12) + 576, 0);
        }
    }
    return isEnemy__8CreatureP10GameObject(self, other);
}
