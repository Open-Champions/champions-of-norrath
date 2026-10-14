#include "../combat-reactions-bytes/bytes.hpp"
// Observed msg_collision__4GoldP10GameObject at 0x154db0, 184 bytes.
// Returns without a write when bit 0x20000 of the other object's word at +12
// is set, when the signed byte at object+76 is zero, when bit 0x40 of the
// other object's word at +12 is clear, or when the word at object+128 is zero.
// Otherwise the resource "inv_equip_coin.anm" is looked up in the lump "fx.lmp",
// played as a sequence at gain 1.0 with this object, and the amount at
// object+128 is given to the player index held at other+960 together with a
// 12-byte copy of the point at object+16. The amount word is then cleared and
// the object is added to the delete list.
// Inferred: norrath_lump_find_resource is lumpFindResource,
// norrath_sound_play_sequence is the soundPlaySequence body that takes an
// object (0x24d308), and norrath_object_add_to_delete_list is
// objectAddToDeleteList (0x1437e8). hudGivePlayerGold takes its Point3 by
// address and its return value is not read.
// Unknown: what the byte at object+76 and bit 0x20000 mean.
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name);
extern "C" int norrath_sound_play_sequence(void* animation_header, void* object, float gain);
extern "C" void hudGivePlayerGold__FiG6Point3i(int player, const void* point, int amount);
extern "C" unsigned norrath_object_add_to_delete_list(void* object);

extern "C" void msg_collision__4GoldP10GameObject(void* self, void* other) {
    using namespace combat_reactions;
    static char lump[] = {'f', 'x', '.', 'l', 'm', 'p', '\0'};
    static char resource[] = {'i', 'n', 'v', '_', 'e', 'q', 'u', 'i', 'p', '_', 'c', 'o', 'i', 'n', '.', 'a', 'n', 'm', '\0'};
    const Address base = address(self);
    const Address other_base = address(other);
    const unsigned other_flags = read_word(other_base + 12u);
    if ((other_flags & 0x20000u) != 0u) return;
    if ((read_u8(base + 76u) & 0xffu) == 0u) return;
    if ((other_flags & 0x40u) == 0u) return;
    if (read_word(base + 128u) == 0u) return;
    norrath_sound_play_sequence(norrath_lump_find_resource(lump, resource), self, 1.0f);
    unsigned char point[12];
    for (unsigned i = 0u; i < 12u; ++i) point[i] = bytes(base + 16u)[i];
    hudGivePlayerGold__FiG6Point3i(signed_word(read_word(other_base + 960u)), point,
                                   signed_word(read_word(base + 128u)));
    write_word(base + 128u, 0u);
    (void)norrath_object_add_to_delete_list(self);
}
