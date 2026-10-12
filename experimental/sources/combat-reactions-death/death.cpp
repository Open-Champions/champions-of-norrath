#include "../combat-reactions-bytes/bytes.hpp"
// Observed death__8Creature at 0x112af8, 168 bytes.
// findTagString runs with the bytes "deathScript" and occurrence 0; a non-null
// result is passed to scriptStart. findTagString then runs with "dropitem" and
// occurrence 0; a non-null result makes a 12-byte copy of the point at
// object+16 and passes the copy and the tag text to item::dropItem. The word at
// object+12 is then read: when bit 0x2 is set, bit 0x4000 is ORed into it and
// stored, the animation state at object+288 is set to its end, and the object
// is added to the delete list. When bit 0x2 is clear the word is not written.
// Inferred: norrath_actor_find_tag_string is the findTagString body,
// norrath_animation_set_to_end is animSetToEnd, and
// norrath_object_add_to_delete_list is objectAddToDeleteList (0x1437e8).
// item::dropItem is a static member whose Point3 is passed by address; its
// return value is not read. Unknown: the meaning of bits 0x2 and 0x4000.
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" void scriptStart__FPc(char* script);
extern "C" void dropItem__4itemG6Point3Pc(const void* point, char* name);
extern "C" void norrath_animation_set_to_end(void* state);
extern "C" unsigned norrath_object_add_to_delete_list(void* object);

extern "C" void death__8Creature(void* self) {
    using namespace combat_reactions;
    static char script_tag[] = {'d', 'e', 'a', 't', 'h', 'S', 'c', 'r', 'i', 'p', 't', '\0'};
    static char drop_tag[] = {'d', 'r', 'o', 'p', 'i', 't', 'e', 'm', '\0'};
    const Address base = address(self);
    char* script = norrath_actor_find_tag_string(self, script_tag, 0);
    if (script != nullptr) scriptStart__FPc(script);
    char* drop = norrath_actor_find_tag_string(self, drop_tag, 0);
    if (drop != nullptr) {
        unsigned char point[12];
        for (unsigned i = 0u; i < 12u; ++i) point[i] = bytes(base + 16u)[i];
        dropItem__4itemG6Point3Pc(point, drop);
    }
    const unsigned flags = read_word(base + 12u);
    if ((flags & 0x2u) != 0u) {
        write_word(base + 12u, flags | 0x4000u);
        norrath_animation_set_to_end(static_cast<unsigned char*>(self) + 288u);
        (void)norrath_object_add_to_delete_list(self);
    }
}
