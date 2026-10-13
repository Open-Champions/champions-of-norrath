// Observed msg_collision__7CritterP10GameObject at 0x001f1910, 172 bytes.
// Let s be the signed halfword at object+572 (compared as signed for 12 and 13, as unsigned for
// the range test):
//  - s == 12: nothing happens;
//  - s == 13: calls virtual slot byte 184 of the table at object+72 with the int 14;
//  - s is 0 or 1 (unsigned compare below 2): if the word at object+1064 is zero nothing happens;
//    otherwise, when the other object is not null, calls listOnewayLink__FP8ListHeadT0Pv with
//    (object+576, other, other), and then in either case calls virtual slot byte 184 with 7;
//  - any other s: nothing happens.
// Inferred: slot 184 is the state setter (sibling tables fill it with setState__<Class>i), the
// list at +576 holds objects that have touched this one, and the second head argument is the head
// at the start of the other object. Unknown: the names of states 0, 1, 7, 12, 13 and 14 and of the
// word at +1064. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

struct ListHead;
extern "C" void* listOnewayLink__FP8ListHeadT0Pv(ListHead* first, ListHead* second, void* value);

extern "C" void msg_collision__7CritterP10GameObject(void* self, void* other) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const short state = *reinterpret_cast<short*>(object + 572);
    if (state == 12) return;
    if (state == 13) {
        w10_actor_d::call_slot(self, 184, 14);
        return;
    }
    if (static_cast<unsigned short>(state) < 2) {
        if (*reinterpret_cast<int*>(object + 1064) == 0) return;
        if (other != nullptr)
            listOnewayLink__FP8ListHeadT0Pv(reinterpret_cast<ListHead*>(object + 576), static_cast<ListHead*>(other), other);
        w10_actor_d::call_slot(self, 184, 7);
    }
}
