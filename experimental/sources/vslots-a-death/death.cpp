// Observed death__8Creature at 0x00112af8, 168 bytes.
// Looks up "deathScript" and, when that tag exists, starts it. Looks up
// "dropitem" and, when that tag exists, copies the 12 bytes at creature+16
// and passes that point with the tag text to dropItem. Flag bit 2 then
// stores the flags word at creature+12 with bit 0x4000 set, sets the
// animation state at creature+288 to its end, and adds the creature to the
// delete list. Other flag bits do not.

extern "C" {
char* findTagString__10GameObjectPci(void* object, char* tag, int index);
int scriptStart__FPc(char* name);
void dropItem__4itemG6Point3Pc(const void* point, char* name);
void animSetToEnd__FP14AnimationState(void* state);
void objectAddToDeleteList__FP10GameObject(void* object);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void death__8Creature(void* self) {
    auto* creature = static_cast<unsigned char*>(self);
    char death_script[] = "deathScript";
    char* script = findTagString__10GameObjectPci(creature, death_script, 0);
    if (script != nullptr) scriptStart__FPc(script);
    char drop_item[] = "dropitem";
    char* item = findTagString__10GameObjectPci(creature, drop_item, 0);
    if (item != nullptr) {
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = creature[16u + i];
        dropItem__4itemG6Point3Pc(point, item);
    }
    const unsigned flags = read_word(creature + 12);
    if ((flags & 0x2u) != 0u) {
        write_word(creature + 12, flags | 0x4000u);
        animSetToEnd__FP14AnimationState(creature + 288);
        objectAddToDeleteList__FP10GameObject(creature);
    }
}
