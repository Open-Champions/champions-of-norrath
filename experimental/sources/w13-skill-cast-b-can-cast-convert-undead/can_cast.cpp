// Observed canCast__18SkillConvertUndeadP8Creature at 0x00298d00, 268 bytes in .text.
// The skill argument is not read. Reading the floats at creature+16, +20, +24 as a position, the function:
//  1. builds a box from that position minus 329.0 to that position plus 329.0 on each axis and calls
//     objectFindInBox__FG6Point3T0PP10GameObjecti(low corner, high corner, list, 512) with a 512-pointer list in its frame;
//  2. walks the list from its start until a null entry. For each entry it calls the entry at table byte 160 of the
//     creature's own table (the word at creature+72; a signed halfword adjusts the receiver, a function word is at +4)
//     with (adjusted creature, entry). When that returns non-zero and the signed byte at entry+172 equals 16, the
//     function returns one at once;
//  3. when the walk ends without such an entry (including an empty list) the message for the key "convertundead_warn"
//     is looked up with textGetString__FPc and shown with hudSetInfoMessage__FPUsiic(text, 180, the word at
//     creature+960, 0), and the function returns zero.
// Inferred: the skill needs a nearby object whose byte at +172 is 16 and that passes a creature-level check; the key
// text suggests the message is a warning. Unknown: what the table entry at byte 160 is, what the byte value 16 marks,
// whether the list is always null-terminated, and the type of the 329.0 pool word (bits 0x43a48000 at gp-20972; written as a literal).
#include "../w13-skill-cast-b-common/skill_access.hpp"

extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** list, int max);
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag);
int can_cast_convert_undead(void* skill, void* creature) asm("canCast__18SkillConvertUndeadP8Creature");
}
static char warn_key[] = "convertundead_warn";
extern "C" int can_cast_convert_undead(void*, void* creature) {
    unsigned char* c = static_cast<unsigned char*>(creature);
    void* list[512];
    const float x = *reinterpret_cast<float*>(c + 16);
    const float y = *reinterpret_cast<float*>(c + 20);
    const float z = *reinterpret_cast<float*>(c + 24);
    const float radius = 329.0f;
    const Point3 low = {x - radius, y - radius, z - radius};
    const Point3 high = {x + radius, y + radius, z + radius};
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 512);
    for (void** entry = list; *entry; ++entry) {
        if (w13scb::call_object_entry(c, 72, 160, *entry) != 0 && static_cast<signed char>(static_cast<unsigned char*>(*entry)[172]) == 16) return 1;
    }
    hudSetInfoMessage__FPUsiic(textGetString__FPc(warn_key), 180, *reinterpret_cast<int*>(c + 960), 0);
    return 0;
}
