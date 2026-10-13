#include "../w10-actor-b-common/fields.hpp"
// Observed msg_moveCollision__10ArenabeastPP10GameObject at 0x236a70, 244 bytes. The
// second argument is a null-terminated list of object pointers. Nothing happens unless
// one of these holds, with the state being the signed halfword at object+572:
//   the state is 22 or 23; or the words at +972 and +968 are both zero and the state is 21.
// Then, for each listed object whose flag word (+12) has bit 0x20 set, in list order,
// gameAttackResolver(listed, self, single(short at +200), single(short at +202), 100000,
// 0, 0, 0x100041, 0, 0) is called and the word at object+972 is set to 1 (after the call).
// The re-test of +972 is not repeated, so one hit does not stop later ones in the list.
// Inferred: +972 is a "has already hit" latch for the charge states and +200/+202 are
// attack parameters. Unknown: the meaning of states 21, 22 and 23 and of the 0x100041
// attack-flag word; the function's result is not set, so it is declared void.
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* attacker, float a, float b, int c, int d, int e, int f, void* item, bool g);

extern "C" void msg_moveCollision__10ArenabeastPP10GameObject(void* self, void** list) {
    using namespace actor_b;
    const int state = field<SHalf>(self, 572);
    if (!(field<Word>(self, 972) == 0 && field<Word>(self, 968) == 0 && state == 21)) {
        if (state != 22 && state != 23) return;
    }
    const Word* entry = &field<Word>(list, 0);
    while (*entry != 0) {
        void* other = rtti_c::pointer(*entry++);
        if ((field<Word>(other, 12) & 0x20) != 0) {
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                other, self, static_cast<float>(field<SHalf>(self, 200)), static_cast<float>(field<SHalf>(self, 202)),
                100000, 0, 0, 0x100041, nullptr, false);
            field<Word>(self, 972) = 1;
        }
    }
}
