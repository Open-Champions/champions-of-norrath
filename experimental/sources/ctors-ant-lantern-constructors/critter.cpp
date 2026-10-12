#include "detail.hpp"
// __7CritterPcG6Point3sPPc at 0x1efeb0, 1944 bytes. The first argument after the
// object is a name string, which is lowercased in place.
// Observed: eRand is called first; the Creature scale is (roll % 14) + 4 as a
// single. Creature receives the snapshot, the short, a null map and the argument
// list. Byte 40 is 10, the single at +788 truncated to an integer goes to +1084,
// bit 8 is ORed into +12, _vt$7Critter (0x44c080) goes to +72, the single at
// 0x480d28 to +168, 10.0 to +28, the word at 0x4844a4 to +248, byte 172 is 18,
// +1076 is 2 and +244, +176, +192, +236, half 572 and +1080 are cleared. A
// setParameters call with five 1.0 values and 1 follows. Words 1028 and 1020 are
// 180, 1024 is 60 and 1032..1048, 1052..1072 are cleared. The name is lowercased
// by adding 32 to each character whose ctype table entry has bit 0 set (a signed
// character indexes the table), then copied to a stack buffer. A string tag at
// 0x46dc90 (or the name when absent) is printed with the format at 0x485158 into a
// second buffer, the lump name. Each following tag (or the name when absent) is
// printed into a third buffer and looked up in that lump: 0x485160 / 0x485168 ->
// +1036, 0x485170 / 0x485178 -> +1040, 0x46dca0 -> +1044, 0x46dcc0 -> +1056,
// 0x46dce0 -> +1060, 0x46dd00 -> +1064, 0x46dd20 -> +1068 (+1044 if the lookup
// fails), 0x46dd40 -> +1072 (+1064 if it fails); the name-form formats are at
// 0x46dcb0, 0x46dcd0, 0x46dcf0, 0x46dd10, 0x46dd30 and 0x46dd50 and the tag form
// is the format at 0x485180. A string tag at 0x46dd60 is read with the float format
// at 0x485188 into +1016, else +1016 is 1.0. The animation state at +288 is
// initialised and gameLineInitPathState receives object+960, the string tag at
// 0x485190 and object+16. If the word at +980 is set, bits 5 are ORed into +12, the
// header at +1056 is added with flag 2049, its time is randomised and state 12 is
// set; otherwise an integer tag at 0x485198 sets state 13, else state 0 when +1044
// is set, else state 1 when +1056 is set. Thirty-two string tags named from the
// format at 0x46dd70 set bit i of +244. specificInit__7CritterPcT1 receives the
// name copy and the lump name, the eight-byte chunk result for the header at +1036
// is stored at +552, the object joins the run list when half 572 is 12 and the slow
// run list otherwise, and V0 is finishConstruction's.
namespace {
using namespace norrath_ctors_ant_lantern;
inline __attribute__((always_inline)) void print_from_tag(void* object, const unsigned char* tag_view, const unsigned char* tag_format,
                                                         const unsigned char* name_format, char* buffer, const char* name_copy) {
    char* const tag = reinterpret_cast<char*>(const_cast<unsigned char*>(tag_view));
    if (norrath_actor_find_tag_string(object, tag, 0) != nullptr)
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(tag_format), norrath_actor_find_tag_string(object, tag, 0));
    else
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(name_format), name_copy);
}
}
extern "C" unsigned norrath_construct_Critter(void* object, char* name, const void* point, short type, char** arguments) {
    PointBytes local = snapshot_point(point);
    const unsigned scale_roll = norrath_actor_random(object);
    (void)norrath_creature_construct(object, &local, type, nullptr, static_cast<float>(static_cast<int>(scale_roll % 14u + 4u)), arguments);
    const Address a = address(object);
    bytes(a)[40] = 10u;
    {
        const unsigned word = read_word(a + 788u);
        float value;
        __builtin_memcpy(&value, &word, 4);
        write_word(a + 1084u, static_cast<unsigned>(static_cast<int>(value)));
    }
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44c080));
    write_word(a + 12u, read_word(a + 12u) | 0x8u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_480d28)));
    write_word(a + 28u, 0x41200000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    bytes(a)[172] = 18u;
    write_word(a + 1076u, 2u);
    write_word(a + 244u, 0u);
    write_word(a + 176u, 0u);
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 1080u, 0u);
    (void)norrath_actor_set_parameters(object, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1);
    write_word(a + 1028u, 180u);
    write_word(a + 1024u, 60u);
    write_word(a + 1020u, 180u);
    write_word(a + 1032u, 0u);
    write_word(a + 1036u, 0u);
    write_word(a + 1040u, 0u);
    write_word(a + 1072u, 0u);
    write_word(a + 1068u, 0u);
    write_word(a + 1064u, 0u);
    write_word(a + 1060u, 0u);
    write_word(a + 1056u, 0u);
    write_word(a + 1052u, 0u);
    write_word(a + 1048u, 0u);
    write_word(a + 1044u, 0u);
    for (char* cursor = name; *cursor != 0; ++cursor) {
        const int character = static_cast<signed char>(*cursor);
        const bool upper = (norrath_factory_ctype_minimum_view[character + 128] & 1u) != 0u;
        *cursor = static_cast<char>(upper ? character + 32 : character);
    }
    char name_copy[16];
    char lump_name[16];
    char resource_name[64];
    strcpy(name_copy, name);
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dc90, norrath_ctors_ant_lantern_view_485158, norrath_ctors_ant_lantern_view_485158, lump_name, name_copy);
    print_from_tag(object, norrath_ctors_ant_lantern_view_485160, norrath_ctors_ant_lantern_view_485168, norrath_ctors_ant_lantern_view_485168, resource_name, name_copy);
    write_word(a + 1036u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_485170, norrath_ctors_ant_lantern_view_485178, norrath_ctors_ant_lantern_view_485178, resource_name, name_copy);
    write_word(a + 1040u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dca0, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dcb0, resource_name, name_copy);
    write_word(a + 1044u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dcc0, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dcd0, resource_name, name_copy);
    write_word(a + 1056u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dce0, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dcf0, resource_name, name_copy);
    write_word(a + 1060u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dd00, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dd10, resource_name, name_copy);
    write_word(a + 1064u, address_bits(norrath_lump_find_resource(lump_name, resource_name)));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dd20, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dd30, resource_name, name_copy);
    void* const sixth = norrath_lump_find_resource(lump_name, resource_name);
    write_word(a + 1068u, address_bits(sixth));
    if (sixth == nullptr) write_word(a + 1068u, read_word(a + 1044u));
    print_from_tag(object, norrath_ctors_ant_lantern_view_46dd40, norrath_ctors_ant_lantern_view_485180, norrath_ctors_ant_lantern_view_46dd50, resource_name, name_copy);
    void* const seventh = norrath_lump_find_resource(lump_name, resource_name);
    write_word(a + 1072u, address_bits(seventh));
    if (seventh == nullptr) write_word(a + 1072u, read_word(a + 1064u));
    char* const speed_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_46dd60);
    if (const char* text = norrath_actor_find_tag_string(object, speed_tag, 0))
        sscanf(text, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485188), reinterpret_cast<float*>(pointer(a + 1016u)));
    else
        write_word(a + 1016u, 0x3f800000u);
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    gameLineInitPathState__FP17gameLinePathStatePcP6Point3(
        pointer(a + 960u), norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485190), 0), pointer(a + 16u));
    if (read_word(a + 980u) != 0u) {
        write_word(a + 12u, read_word(a + 12u) | 0x5u);
        norrath_actor_animation_add(state, pointer(read_word(a + 1056u)), 2049);
        norrath_actor_animation_randomize_time(state);
        setState__7Critteri(object, 12);
    } else if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_485198), 0) != 0) {
        setState__7Critteri(object, 13);
    } else if (read_word(a + 1044u) != 0u) {
        setState__7Critteri(object, 0);
    } else if (read_word(a + 1056u) != 0u) {
        setState__7Critteri(object, 1);
    }
    char change_name[16];
    for (int bit = 0; bit < 32; ++bit) {
        norrath_factory_sprintf(change_name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46dd70), bit);
        if (norrath_actor_find_tag_string(object, change_name, 0) != nullptr)
            write_word(a + 244u, read_word(a + 244u) | (1u << bit));
    }
    specificInit__7CritterPcT1(object, name_copy, lump_name);
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(a + 1036u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    if (static_cast<short>(bytes(a)[572] | (bytes(a)[573] << 8)) == 12)
        (void)norrath_actor_add_run_list(object);
    else
        (void)norrath_actor_add_slow_run_list(object);
    return norrath_actor_finish_construction(object);
}
