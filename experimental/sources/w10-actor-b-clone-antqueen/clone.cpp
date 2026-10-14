#include "../w10-actor-b-common/fields.hpp"
// Observed clone__8AntQueen at 0x1171f0, 1396 bytes. Allocates 992 bytes; a null allocation
// returns null and writes nothing. Otherwise, in this order (offsets in the source and the copy):
//   GameObject part: the GameObject vtable (0x44d498) at +72, then the ListHead copy
//   constructor for the heads at +0, +4 and +8, then copies of +12, +16..+27, +28, +36, +32,
//   +40, +41; the Creature vtable (0x4488f0) replaces +72; then +48, +68, +52, +60, +56, +44,
//   +64, +76, +80, +84, +85; five words from +88, four from +108 and four from +124;
//   +140..+155, +156, +168, +160 (8 bytes), +172, +173, +176, +180..+187, +188, +192, +200, +212,
//   +202, +196, +204, +208, +216, +244, +220, +248, +236, +252, +224, +232, +228, +240, +256,
//   +260..+267, +268, +272..+279, +280, +284; the AnimationState copy constructor on +288;
//   +552 (8 bytes), +560, +564, +568, +569, +570, +572; the ListHead copy constructor on +576;
//   sixteen 8-byte blocks from +580; +708, +712, +716, +720..+727, +728, +732, +748, +740, +736,
//   +744; the ListHead copy constructor on +752; +756, +760..+767, +768, +772, +776..+783, +784,
//   +792, +788, +794, +796, +798, +800, +804, +808, +812, eight 8-byte blocks from +816, +880,
//   +882, +884, +886; the ListHead copy constructor on +888; +892; sixteen words from +896;
//   then the 8AntQueen vtable (0x44c440) at +72; +960 (byte), +964, +968 (byte), +972, +980,
//   +976, +984, +988 (halfword). The new object is returned.
// Bytes of the new object that this list does not name keep what the allocator returned.
// Inferred: this is the compiler-generated copy construction of 8AntQueen from its bases
// and members; the ListHead copy constructor does not read its source (it is only
// called with the matching offsets). Unknown: what the members at +576, +752 and +888 are.
extern "C" {
void* blockAlloc__Fi(int bytes);
void norrath_list_head_copy_construct(void* destination, const void* source);
void animation_state_copy(void* destination, const void* source) asm("__15AnimationState8RC15AnimationState8");
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_ctors_ant_lantern_view_44c440[];
}

namespace {
using namespace actor_b;
typedef unsigned long long __attribute__((may_alias, aligned(1))) Block8;
typedef unsigned long long __attribute__((may_alias)) Double;

struct Copier {
    unsigned char* to;
    const unsigned char* from;
    __attribute__((always_inline)) void word(unsigned offset) const { *reinterpret_cast<Word*>(to + offset) = *reinterpret_cast<const Word*>(from + offset); }
    __attribute__((always_inline)) void half(unsigned offset) const { *reinterpret_cast<Half*>(to + offset) = *reinterpret_cast<const Half*>(from + offset); }
    __attribute__((always_inline)) void byte(unsigned offset) const { to[offset] = from[offset]; }
    __attribute__((always_inline)) void block8(unsigned offset) const { *reinterpret_cast<Block8*>(to + offset) = *reinterpret_cast<const Block8*>(from + offset); }
    __attribute__((always_inline)) void dword(unsigned offset) const { *reinterpret_cast<Double*>(to + offset) = *reinterpret_cast<const Double*>(from + offset); }
    __attribute__((always_inline)) void words(unsigned offset, unsigned count) const { for (unsigned i = 0; i < count; ++i) word(offset + 4 * i); }
    __attribute__((always_inline)) void blocks8(unsigned offset, unsigned count) const { for (unsigned i = 0; i < count; ++i) block8(offset + 8 * i); }
    __attribute__((always_inline)) void list(unsigned offset) const { norrath_list_head_copy_construct(to + offset, from + offset); }
};
}

extern "C" void* clone__8AntQueen(void* source_storage) {
    const unsigned char* source = static_cast<const unsigned char*>(source_storage);
    unsigned char* created = static_cast<unsigned char*>(blockAlloc__Fi(992));
    if (created == nullptr) return nullptr;
    const Copier c = {created, source};
    field<Word>(created, 72) = rtti_c::address_bits(norrath_game_object_vtable_view);
    c.list(0);
    c.list(4);
    c.list(8);
    c.word(12);
    c.block8(16);
    c.word(24);
    c.word(28);
    c.word(36);
    c.word(32);
    c.byte(40);
    field<Word>(created, 72) = rtti_c::address_bits(norrath_actor_view_4488f0);
    c.byte(41);
    c.word(48);
    c.word(68);
    c.word(52);
    c.word(60);
    c.half(56);
    c.word(44);
    c.word(64);
    c.byte(76);
    c.word(80);
    c.byte(84);
    c.byte(85);
    c.words(88, 5);
    c.words(108, 4);
    c.words(124, 4);
    c.block8(140);
    c.block8(148);
    c.byte(156);
    c.word(168);
    c.dword(160);
    c.byte(172);
    c.byte(173);
    c.word(176);
    c.block8(180);
    c.half(188);
    c.word(192);
    c.half(200);
    c.word(212);
    c.half(202);
    c.word(196);
    c.half(204);
    c.word(208);
    c.word(216);
    c.word(244);
    c.word(220);
    c.word(248);
    c.word(236);
    c.word(252);
    c.word(224);
    c.half(232);
    c.word(228);
    c.word(240);
    c.word(256);
    c.block8(260);
    c.word(268);
    c.block8(272);
    c.word(280);
    c.half(284);
    animation_state_copy(created + 288, source + 288);
    c.dword(552);
    c.word(560);
    c.word(564);
    c.byte(568);
    c.byte(569);
    c.byte(570);
    c.half(572);
    c.list(576);
    c.blocks8(580, 16);
    c.word(708);
    c.word(712);
    c.word(716);
    c.block8(720);
    c.word(728);
    c.word(732);
    c.half(748);
    c.word(740);
    c.word(736);
    c.word(744);
    c.list(752);
    c.word(756);
    c.block8(760);
    c.word(768);
    c.word(772);
    c.block8(776);
    c.word(784);
    c.half(792);
    c.word(788);
    c.half(794);
    c.half(796);
    c.half(798);
    c.byte(800);
    c.word(804);
    c.word(808);
    c.word(812);
    c.blocks8(816, 8);
    c.half(880);
    c.half(882);
    c.half(884);
    c.half(886);
    c.list(888);
    c.word(892);
    c.words(896, 16);
    field<Word>(created, 72) = rtti_c::address_bits(norrath_ctors_ant_lantern_view_44c440);
    c.byte(960);
    c.word(964);
    c.byte(968);
    c.word(972);
    c.word(980);
    c.word(976);
    c.word(984);
    c.half(988);
    return created;
}
