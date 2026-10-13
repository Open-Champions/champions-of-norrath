// Observed msg_trigger__9Generatori at 0x001f1ff8, 140 bytes.
// When bit 0 of the word at object+980 is set, creates an object through
// objectCreateByName__FPcG6Point3sPPc: the name is the pointer stored at +960, the position is a
// copy of the 12 bytes at +16, the type is the signed halfword at +56 and the argument list is
// null. If that returns an object, bit 24 of its 64-bit field at +160 is set. In every case the
// Creature trigger routine is then called with the object and the incoming value, and its result
// is returned.
// Inferred: the name word is a string pointer. Unknown: what +960, +980 and bit 24 of +160 mean.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void* objectCreateByName__FPcG6Point3sPPc(char* name, Point3 position, short type, char** arguments);
int msg_trigger__8Creaturei(void* self, int value);
}

namespace {
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;
}

extern "C" int msg_trigger__9Generatori(void* self, int value) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if ((*reinterpret_cast<unsigned*>(object + 980) & 1) != 0) {
        char* name = reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(object + 960)));
        Point3 position = *reinterpret_cast<Point3*>(object + 16);
        short type = *reinterpret_cast<short*>(object + 56);
        void* created = objectCreateByName__FPcG6Point3sPPc(name, position, type, nullptr);
        if (created != nullptr) {
            *reinterpret_cast<AliasedDouble*>(static_cast<unsigned char*>(created) + 160) |= 0x01000000ull;
        }
    }
    return msg_trigger__8Creaturei(self, value);
}
