// Observed objectActiveProcessing__FP10GameObjectii at 0x142158, extent 436.
// Decides whether an object is inside the active range of the viewers, and when
// the answer differs from the caller's previous answer, moves the object between
// the run lists. Arguments: the object, the previous answer, a radius.
//   1. radius = 0xabe0 when bit 19 (0x80000) of the flag word at +12 is set,
//      otherwise the argument. The squared radius is the 32-bit integer product,
//      converted to float.
//   2. The object position is the three floats at +16. When the byte
//      objectVisUseCamCenter (0x483ac9) is nonzero the only reference point is
//      worldViewCenter (0x37ee90). Otherwise the reference points are the first
//      objectVisCount (signed byte, 0x483afe) entries of the 12-byte array
//      objectVisPoints (0x333d28); a count of zero or less gives no points.
//   3. The answer is 1 when ((dx*dx + dy*dy) + dz*dz) < squared radius for the
//      reference point (any point, first hit wins), else 0.
//   4. When the answer equals the previous answer it is returned unchanged.
//      Otherwise the run node (+4) is removed if present and the object is linked
//      with object+4 as the second head: answer 1 onto runList (0x486878); answer
//      0 onto slowRunList[cycle], where slowListCycle is advanced and wrapped at 60
//      exactly as objectAddToSlowRunList does (the arithmetic is repeated inline in
//      the image). The answer is returned.
// Unknown: what the callers mean by the radius values, and the meaning of bit 19.
#include "../w8-object-list-math-common/object_types.hpp"

struct Position {
    float x, y, z;
};
struct ActiveObject {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
    Position position;
};
struct HeadSlot {
    ListHead head;
    unsigned unobserved_word;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(ActiveObject, position) == 16 && sizeof(Position) == 12, "observed offsets");
#endif

extern "C" unsigned norrath_list_remove(ListNode* node);
extern "C" ListNode* listOnewayLink__FP8ListHeadT0Pv(ListHead* first_head, ListHead* second_head, void* value);
extern "C" HeadSlot runList;
extern "C" ListHead slowRunList[60];
extern "C" int slowListCycle;
extern "C" unsigned char objectVisUseCamCenter;
extern "C" signed char objectVisCount;
extern "C" Position objectVisPoints[];
extern "C" float worldViewCenter[3];

namespace {
bool within(const Position& a, const float* b, float squared_radius) {
    const float dx = a.x - b[0];
    const float dy = a.y - b[1];
    const float dz = a.z - b[2];
    return dx * dx + dy * dy + dz * dz < squared_radius;
}
}

extern "C" int objectActiveProcessing__FP10GameObjectii(ActiveObject* object, int previous, int radius_argument) {
    int radius = 0xabe0;
    if ((object->flags & 0x80000u) == 0) radius = radius_argument;
    const float squared_radius = static_cast<float>(radius * radius);
    int active = 0;
    if (objectVisUseCamCenter != 0) {
        if (within(object->position, worldViewCenter, squared_radius)) active = 1;
    } else {
        const int count = objectVisCount;
        for (int i = 0; i < count; ++i) {
            if (within(object->position, &objectVisPoints[i].x, squared_radius)) {
                active = 1;
                break;
            }
        }
    }
    if (active == previous) return active;
    if (active != 0) {
        if (object->run_node) norrath_list_remove(object->run_node);
        listOnewayLink__FP8ListHeadT0Pv(&runList.head, reinterpret_cast<ListHead*>(&object->run_node), object);
    } else {
        if (object->run_node) norrath_list_remove(object->run_node);
        int cycle = slowListCycle + 1;
        if (cycle >= 60) cycle = 0;
        slowListCycle = cycle;
        listOnewayLink__FP8ListHeadT0Pv(&slowRunList[cycle], reinterpret_cast<ListHead*>(&object->run_node), object);
    }
    return active;
}
