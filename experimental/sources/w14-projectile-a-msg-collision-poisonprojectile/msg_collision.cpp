// Provenance: the body is the reviewed w13-spell-msg-a unit w13-spell-msg-a-msg-collision-16poisonprojectile, re-verified in this package against the original with a fresh paired run and given a host
// contract (the earlier unit has none and was left out of the plan). Select one of the two units, not both.
// Observed msg_collision__16PoisonProjectileP10GameObject at 0x0020fe10, 280 bytes. Arguments: the projectile o and
// the object T it touched. Nothing is returned. A run-time type is read the way the image does it: the object's
// table (the word at +72) holds at offset 4 a function that is called with no argument and returns the type-info
// pointer, compared through __eq__C9type_infoRC9type_info (only a result of exactly 1 counts as equal).
//  1. Q is the word at o+76. When Q is non-zero, the owner W is the word at Q+12; the function returns when the type
//     of T equals the type of W (T is read first, then W).
//  2. Otherwise (Q zero, or the types differ) the word at o+152 becomes 1 unless the type of o equals the type of T
//     (o is read first, then T).
// Each type read of a null object calls __throw_bad_typeid first (the image goes on to read through the null
// pointer anyway; that is not reproduced and is declared non-returning here).
// Inferred: this is a run-time type test between the projectile, what it hit and its owner; the word at +152 is set
// when the projectile hits a different kind of object. Unknown: what Q, o+152 and the types mean. Nothing here ran
// on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid() __attribute__((noreturn));
}

static void* type_of(unsigned char* object) {
    if (object == nullptr) __throw_bad_typeid();
    const unsigned function = *reinterpret_cast<unsigned*>(w14pa::table_entry(object, 0) + 4);
    return w14pa::from_word(w14pa::call_word(function));
}

extern "C" void msg_collision__16PoisonProjectileP10GameObject(void* self, void* other) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const t = static_cast<unsigned char*>(other);
    if (word(o, 76) != 0) {
        void* const typeOfOther = type_of(t);
        unsigned char* const owner = from_word(word(from_word(word(o, 76)), 12));
        if (__eq__C9type_infoRC9type_info(typeOfOther, type_of(owner)) == 1) return;
    }
    void* const typeOfSelf = type_of(o);
    if (__eq__C9type_infoRC9type_info(typeOfSelf, type_of(t)) != 1) word(o, 152) = 1;
}
