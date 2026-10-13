// Observed msg_collision__16beetleProjectileP10GameObject at 0x00212620, 128 bytes. O is the projectile and X
// the second argument. A null X calls __throw_bad_typeid. Otherwise the run-time type of X is read (the
// function at word +4 of the table that the word at X+72 points to, called with no arguments) and compared,
// through __eq__C9type_infoRC9type_info, with the type of FireBeetle (__tf10FireBeetle). Only when they differ is
// msg_collision__13MissileWeaponP10GameObject(O, X) called, as the last action. Nothing else is touched.
// Inferred: the projectile ignores collisions with a FireBeetle (its likely owner). Unknown: why.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
void __throw_bad_typeid();
void* __tf10FireBeetle();
bool __eq__C9type_infoRC9type_info(const void* left, const void* right);
void msg_collision__13MissileWeaponP10GameObject(void* self, void* other);

void msg_collision__16beetleProjectileP10GameObject(void* self, void* other) {
    using namespace w13smb;
    if (other == nullptr) __throw_bad_typeid();
    unsigned char* const x = static_cast<unsigned char*>(other);
    void* const type = from_word(call_word(word(from_word(word(x, 72)), 4)));
    if (!__eq__C9type_infoRC9type_info(type, __tf10FireBeetle())) msg_collision__13MissileWeaponP10GameObject(self, other);
}
}
