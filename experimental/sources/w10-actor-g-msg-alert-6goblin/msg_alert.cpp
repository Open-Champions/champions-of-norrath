// Observed msg_alert__6GoblinifG6Point3P10GameObject at 0x0022ef80, 340 bytes.
// Arguments: object, an int kind, a float (unused), a position and another object. Nothing happens
// when the signed byte at +570 is nonzero.
//  kind 0: needs a non-null other object whose type node (class table word +4, no-argument
//   function) equals __tf6Goblin(), and the unsigned halfword at +572 below 2. Then the table entry
//   at (word +72) + 184 is called with the adjusted object and 2, and the position is copied to
//   +272..283.
//  kind 2: needs the word at +992 nonzero, a non-null other object and the signed halfword at +572
//   equal to 3 or 5. The angle iatan2(other.y - self.y, other.x - self.x) (fields +20 and +16 of
//   each, evaluated as iatan2__Fff(f12 = difference of +20, f13 = difference of +16)) is
//   compared with the signed halfword at +56: when the difference is within -9999..9999 inclusive
//   the table entry at +184 is called with the adjusted object and 15.
//  other kinds do nothing.
// Inferred: the angle test is a "roughly facing the other object" check. Unknown: the angle unit.
// Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
void* __tf6Goblin();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
int iatan2__Fff(float y, float x);
}

extern "C" void msg_alert__6GoblinifG6Point3P10GameObject(void* self, int kind, float, Point3 position, void* other) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(object[570]) != 0) return;
    if (kind == 0) {
        if (other == nullptr) return;
        const void* node = w10g::type_node_of(other);
        if (!__eq__C9type_infoRC9type_info(node, __tf6Goblin())) return;
        if (*reinterpret_cast<unsigned short*>(object + 572) >= 2) return;
        w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), 2);
        *reinterpret_cast<Point3*>(object + 272) = position;
    } else if (kind == 2) {
        if (*reinterpret_cast<unsigned*>(object + 992) == 0) return;
        if (other == nullptr) return;
        const short state = *reinterpret_cast<short*>(object + 572);
        if (state != 3 && state != 5) return;
        const unsigned char* theirs = static_cast<const unsigned char*>(other);
        const float dy = *reinterpret_cast<const float*>(theirs + 20) - *reinterpret_cast<float*>(object + 20);
        const float dx = *reinterpret_cast<const float*>(theirs + 16) - *reinterpret_cast<float*>(object + 16);
        const int angle = iatan2__Fff(dy, dx);
        const unsigned diff = static_cast<unsigned>(angle - *reinterpret_cast<short*>(object + 56));
        if (diff + 9999u < 19999u) {
            w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), 15);
        }
    }
}
