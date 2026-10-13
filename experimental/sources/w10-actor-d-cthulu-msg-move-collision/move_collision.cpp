// Observed msg_moveCollision__6CthuluPP10GameObject at 0x00282038, 236 bytes.
// If the signed halfword at object+572 is 15 and 0.5 < the float at object+224:
//  - walks the null-terminated pointer array in the second argument; for each entry whose word
//    at entry+12 has bit 0x40 set, calls gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11
//    descriptionb with (entry, object, f1 = the signed halfword at +200 as float, f2 = the signed
//    halfword at +202 as float, 100000, 0, 0, 0x40041, null, false);
//  - then calls virtual slot byte 184 of the table at object+72 with the int 3 (the same slot
//    that sibling tables fill with setState__<Class>i).
// Otherwise forwards both arguments to the Creature moveCollision routine. Nothing is returned.
// Inferred: this is the charge attack that hurts what the body runs into; the slot call puts the
// creature into state 3 afterwards. Unknown: the meaning of 15, 0x40, 0x40041 and 100000.
// Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

extern "C" {
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* description, bool g);
void msg_moveCollision__8CreaturePP10GameObject(void* self, void** list);
}

extern "C" void msg_moveCollision__6CthuluPP10GameObject(void* self, unsigned* list) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<short*>(object + 572) == 15 && 0.5f < *reinterpret_cast<float*>(object + 224)) {
        for (const unsigned* entry = list; *entry != 0; ++entry) {
            unsigned char* target = w10_actor_d::from_word(*entry);
            if ((*reinterpret_cast<unsigned*>(target + 12) & 0x40) != 0) {
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                    target, self, static_cast<float>(*reinterpret_cast<short*>(object + 200)),
                    static_cast<float>(*reinterpret_cast<short*>(object + 202)), 100000, 0, 0, 0x40041, nullptr, false);
            }
        }
        w10_actor_d::call_slot(self, 184, 3);
    } else {
        msg_moveCollision__8CreaturePP10GameObject(self, reinterpret_cast<void**>(list));
    }
}
