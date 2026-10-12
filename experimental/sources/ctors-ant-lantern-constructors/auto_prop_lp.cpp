#include "detail.hpp"
// __10AutoPropLPPcG6Point3sPPc at 0x223bd8, 1828 bytes.
// Observed: AutoProp receives the object, the name, a point snapshot, the short
// and the argument list. Words 408, 412, 416 and 420 are cleared and
// _vt$10AutoPropLP (0x44e9d0) goes to +72. For n = 0..3, when
// hasLightOrParticleNumber(arguments, n) is nonzero, a 388-byte block is
// allocated; a non-null block is constructed with AddParticle (snapshot,
// the word at object+44, n); the block pointer (possibly null) is stored at
// +408 + 4n. The four pointers are copied to +424..+436. Then for each i = 0..3
// word +440 + 4i is cleared; a string tag named from i (text 0x4711f0) ORs 1 into
// +208 and into +440 + 4i. Two tables of ten 8-byte entries per i exist, at
// object+456 + 80i and object+776 + 80i; each entry holds a first and a second
// word. Table one: entries start (-1, 0); a string tag (0x471208) sets flag one
// and rewrites up to ten entries from its characters ('1' gives (0, -1), anything
// else (-1, 0)); then for j = 0..9 an integer tag (0x471218) sets the first word
// to its value (and the second word from 0 to -1) and flag one, and an integer
// tag (0x471230) turns a first word of -1 into 0, sets flag one and stores its
// value as the second word. When flag one stayed clear, every entry becomes
// (0, -1). Table two does the same with the texts 0x485760 (string), 0x471248,
// 0x471260 and flag two. Finally the name is compared (strticmp) with the texts
// at 0x485768 and 0x471278; a match stores 30 at +380 of each non-null block in
// +408..+420. V0 is not established; this returns 0.
namespace {
using namespace norrath_ctors_ant_lantern;
// Entry j of table `base` for particle i: first word at this address, second word 4 bytes later.
#define NORRATH_ALWAYS_INLINE inline __attribute__((always_inline))
NORRATH_ALWAYS_INLINE Address first_at(Address a, unsigned base, unsigned i, unsigned j) {
    return a + 8u + base + 80u * i + 8u * j;
}
NORRATH_ALWAYS_INLINE void string_pairs(Address a, unsigned base, unsigned i, const char* text) {
    for (unsigned j = 0u; j < 10u && text[j] != 0; ++j) {
        const bool one = text[j] == '1';
        write_word(first_at(a, base, i, j), one ? 0u : 0xffffffffu);
        write_word(first_at(a, base, i, j) + 4u, one ? 0xffffffffu : 0u);
    }
}
NORRATH_ALWAYS_INLINE void fill(Address a, unsigned base, unsigned i, unsigned first, unsigned second) {
    for (unsigned j = 0u; j < 10u; ++j) {
        write_word(first_at(a, base, i, j), first);
        write_word(first_at(a, base, i, j) + 4u, second);
    }
}
NORRATH_ALWAYS_INLINE void counted_tags(void* object, char* buffer, unsigned base, unsigned i,
                                        const unsigned char* first_format, const unsigned char* second_format,
                                        bool& flag) {
    const Address a = address(object);
    for (unsigned j = 0u; j < 10u; ++j) {
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(first_format), static_cast<int>(i), static_cast<int>(j));
        if (norrath_game_object_find_tag_int(object, buffer, 0) != 0) {
            write_word(first_at(a, base, i, j), static_cast<unsigned>(norrath_game_object_find_tag_int(object, buffer, 0)));
            flag = true;
            if (read_word(first_at(a, base, i, j) + 4u) == 0u) write_word(first_at(a, base, i, j) + 4u, 0xffffffffu);
        }
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(second_format), static_cast<int>(i), static_cast<int>(j));
        if (norrath_game_object_find_tag_int(object, buffer, 0) != 0) {
            flag = true;
            if (read_word(first_at(a, base, i, j)) == 0xffffffffu) write_word(first_at(a, base, i, j), 0u);
            write_word(first_at(a, base, i, j) + 4u, static_cast<unsigned>(norrath_game_object_find_tag_int(object, buffer, 0)));
        }
    }
}
}
extern "C" unsigned norrath_construct_AutoPropLP(void* object, char* name, const void* point, short type, char** arguments) {
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoProp(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 408u, 0u);
    write_word(a + 412u, 0u);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44e9d0));
    write_word(a + 416u, 0u);
    write_word(a + 420u, 0u);
    for (int number = 0; number < 4; ++number) {
        if (hasLightOrParticleNumber__FPPci(arguments, number) != 0) {
            unsigned char* block = norrath_block_alloc(388);
            if (block != nullptr)
                (void)__11AddParticleG6Point3PPci(block, &local, reinterpret_cast<char**>(pointer(read_word(a + 44u))), number);
            write_word(a + 408u + static_cast<unsigned>(number) * 4u, address_bits(block));
        }
    }
    write_word(a + 424u, read_word(a + 408u));
    write_word(a + 428u, read_word(a + 412u));
    write_word(a + 432u, read_word(a + 416u));
    write_word(a + 436u, read_word(a + 420u));
    char buffer[64];
    for (unsigned i = 0u; i < 4u; ++i) {
        write_word(a + 440u + 4u * i, 0u);
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_4711f0), static_cast<int>(i));
        bool flag_one = false;
        bool flag_two = false;
        if (norrath_actor_find_tag_string(object, buffer, 0) != nullptr) {
            write_word(a + 208u, read_word(a + 208u) | 1u);
            write_word(a + 440u + 4u * i, read_word(a + 440u + 4u * i) | 1u);
        }
        fill(a, 448u, i, 0xffffffffu, 0u);
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471208), static_cast<int>(i));
        if (const char* text = norrath_actor_find_tag_string(object, buffer, 0)) {
            flag_one = true;
            string_pairs(a, 448u, i, text);
        }
        counted_tags(object, buffer, 448u, i, norrath_ctors_ant_lantern_view_471218, norrath_ctors_ant_lantern_view_471230, flag_one);
        if (!flag_one) fill(a, 448u, i, 0u, 0xffffffffu);
        fill(a, 768u, i, 0xffffffffu, 0u);
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485760), static_cast<int>(i));
        if (const char* text = norrath_actor_find_tag_string(object, buffer, 0)) {
            flag_two = true;
            string_pairs(a, 768u, i, text);
        }
        counted_tags(object, buffer, 768u, i, norrath_ctors_ant_lantern_view_471248, norrath_ctors_ant_lantern_view_471260, flag_two);
        if (!flag_two) fill(a, 768u, i, 0u, 0xffffffffu);
    }
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485768)) == 0 ||
        strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471278)) == 0) {
        for (unsigned slot = 0u; slot < 4u; ++slot) {
            const unsigned block = read_word(a + 408u + slot * 4u);
            if (block != 0u) write_word(address(pointer(block)) + 380u, 30u);
        }
    }
    return 0u;
}
