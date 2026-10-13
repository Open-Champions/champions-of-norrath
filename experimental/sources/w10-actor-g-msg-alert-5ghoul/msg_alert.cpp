// Observed msg_alert__5GhoulifG6Point3P10GameObject at 0x001f75e0, 188 bytes.
// Arguments: object, an int, a float (unused), a position and another object. Does nothing unless
// the int is zero and the other object is non-null. It then asks the other object's class table
// (word at +4, a no-argument function) for its type node and compares it with __tf5Ghoul()
// through __eq__C9type_infoRC9type_info (node first). If they are equal and the unsigned
// halfword at +572 is below 2, the table entry at (word +72) + 184 is called with the object
// adjusted by its signed halfword and the value 13; the position is then copied to +272..283.
// Inferred: the entry is a state setter and the position is a remembered alert position.
// Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
void* __tf5Ghoul();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
}

extern "C" void msg_alert__5GhoulifG6Point3P10GameObject(void* self, int kind, float, Point3 position, void* other) {
    if (kind != 0) return;
    if (other == nullptr) return;
    const void* node = w10g::type_node_of(other);
    if (!__eq__C9type_infoRC9type_info(node, __tf5Ghoul())) return;
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<unsigned short*>(object + 572) >= 2) return;
    w10g::fn_from_word<void (*)(void*, int)>(w10g::entry_function(self, 184))(w10g::adjusted(self, 184), 13);
    *reinterpret_cast<Point3*>(object + 272) = position;
}
