// Observed msg_collision__17VehicleBodySphereP10GameObject at 0x0021cf88, 48 bytes. Arguments: the sphere
// object S and another game object X. The word at +76 of S is read as an owner object O (not checked for null).
// The table whose address is the word at O+72 is indexed at byte 48: a signed halfword adjustment at +0 and a
// function word at +4. That function is called with (O + adjustment, X) and nothing is returned.
// Inferred: O is the object that owns the sphere and the call forwards the collision to it. Unknown: which
// virtual function the slot at byte 48 is, and the type of O.
// Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" void msg_collision__17VehicleBodySphereP10GameObject(void* self, void* other) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    unsigned char* const owner = from_word(word(s, 76));
    call_entry(owner, 48, to_word(other));
}
