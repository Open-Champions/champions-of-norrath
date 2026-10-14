// Observed alreadyChained__14HateProjectileP10GameObject at 0x001ff6b8, 64 bytes (the tail is alignment padding). O is the projectile and X the
// second argument. The list whose first node is the word at O+84 is walked (each node's next is its word at +0 and
// the word it carries is at +12). The result is 1 when some node's word at +12 equals X and 0 otherwise, including
// for an empty list. The two projectile classes have byte-identical bodies. Nothing here ran on the EE.
#include "../w15-spell-fx-a-common/access.hpp"

extern "C" int alreadyChained__14HateProjectileP10GameObject(void* self, void* object) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    for (unsigned char* node = from_word(word(o, 84)); node != nullptr; node = from_word(word(node, 0))) {
        if (from_word(word(node, 12)) == static_cast<unsigned char*>(object)) return 1;
    }
    return 0;
}
