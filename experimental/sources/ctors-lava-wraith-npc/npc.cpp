#include "../ctors-lava-wraith-detail/detail.hpp"
// __3NPCG6Point3sPPc at 0x2315f8, 896 bytes.
// Creature receives a point snapshot, the sign-extended type, a null map, scale
// 8.0f and the argument list. Flags gain 0x01100008, byte +40 becomes 36, view
// 0x44adb0 goes to +72, 20.0f to +28, 0.5f to +168, 60.0f to +176, and +572
// (half) and +1136 are cleared. The base name at +1068 is the string tag at
// 0x4857e8 copied with strcpy, or the four bytes at 0x4857f0 when the tag is
// absent. The lump name at +960 is sprintf of the format at 0x4857f8 with the
// tag at 0x472050 or, without it, with the base name. When that name equals
// the text at 0x485800 the object is queued for deletion and the constructor
// returns that result with nothing else done. Otherwise +992 receives the
// format at 0x485810 with the tag at 0x485808 (or the base name), the model
// resource of (+960, +992) goes to +1056, +1024 receives the format at 0x485820
// with the tag at 0x485818 (or the base name) and the texture resource of
// (+960, +1024) goes to +1060. The animation state at +288 is initialised; if
// objectFindAnimation finds the name at 0x485828 the header is added with flag
// 1 and its time randomised. +244 collects bit i for each tag "change<i>"
// (format at 0x472060, i = 0..31) that exists. The 64-bit modelGetActiveChunks
// (the resource at +1056, the mask at +244) goes to +552 after the run list is
// called, +236 becomes 1, and flag bit 0x00100000 is cleared when the tag at
// 0x472040 is missing. Low V0 is finishConstruction's machine result.
extern "C" unsigned norrath_construct_NPC(void* object, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_lava_wraith;
    const PointBytes local = snapshot_point(point);
    (void)norrath_creature_construct(object, &local, type, nullptr, 8.0f, arguments);
    const Address a = address(object);
    char* const lump_name = reinterpret_cast<char*>(bytes(a + 960u));
    char* const base_name = reinterpret_cast<char*>(bytes(a + 1068u));
    char* const model_name = reinterpret_cast<char*>(bytes(a + 992u));
    char* const texture_name = reinterpret_cast<char*>(bytes(a + 1024u));
    const unsigned flags = read_word(a + 12u) | 0x01000008u | 0x00100000u;
    write_byte(a + 40u, 36u);
    write_word(a + 12u, flags);
    write_word(a + 72u, address_bits(norrath_ctors_lava_wraith_view_44adb0));
    write_word(a + 28u, float_bits(20.0f));
    write_word(a + 168u, float_bits(0.5f));
    write_word(a + 176u, float_bits(60.0f));
    write_half(a + 572u, 0u);
    write_word(a + 1136u, 0u);
    char* const name_tag = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_4857e8);
    if (norrath_actor_find_tag_string(object, name_tag, 0) != nullptr) {
        (void)strcpy(base_name, norrath_actor_find_tag_string(object, name_tag, 0));
    } else {
        write_word(a + 1068u, read_word(address(norrath_ctors_lava_wraith_view_4857f0)));
    }
    char* const lump_tag = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_472050);
    if (norrath_actor_find_tag_string(object, lump_tag, 0) != nullptr) {
        (void)sprintf(lump_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4857f8),
                      norrath_actor_find_tag_string(object, lump_tag, 0));
    } else {
        (void)sprintf(lump_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_4857f8), base_name);
    }
    if (strcmp(lump_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485800)) == 0) {
        return norrath_object_add_to_delete_list(object);
    }
    char* const model_tag = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_485808);
    if (norrath_actor_find_tag_string(object, model_tag, 0) != nullptr) {
        (void)sprintf(model_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485810),
                      norrath_actor_find_tag_string(object, model_tag, 0));
    } else {
        (void)sprintf(model_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485810), base_name);
    }
    store_pointer(a + 1056u, norrath_lump_find_resource(lump_name, model_name));
    char* const texture_tag = reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_485818);
    if (norrath_actor_find_tag_string(object, texture_tag, 0) != nullptr) {
        (void)sprintf(texture_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485820),
                      norrath_actor_find_tag_string(object, texture_tag, 0));
    } else {
        (void)sprintf(texture_name, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485820), base_name);
    }
    void* const texture = norrath_lump_find_resource(lump_name, texture_name);
    store_pointer(a + 1060u, texture);
    norrath_actor_animation_initialize(bytes(a + 288u));
    void* const idle = norrath_object_find_animation(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_485828));
    if (idle != nullptr) {
        norrath_actor_animation_add(bytes(a + 288u), idle, 1);
        norrath_actor_animation_randomize_time(bytes(a + 288u));
    }
    write_word(a + 244u, 0u);
    char change[32];
    for (int index = 0; index < 32; ++index) {
        (void)sprintf(change, reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_472060), index);
        if (norrath_actor_find_tag_string(object, change, 0) != nullptr) {
            write_word(a + 244u, read_word(a + 244u) | (1u << index));
        }
    }
    const unsigned long long chunks = norrath_actor_model_active_chunks(load_pointer(a + 1056u),
                                                                         static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(chunks));
    write_word(a + 556u, static_cast<unsigned>(chunks >> 32));
    (void)norrath_actor_add_run_list(object);
    write_word(a + 236u, 1u);
    if (norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_lava_wraith_view_472040), 0) == nullptr) {
        write_word(a + 12u, read_word(a + 12u) & 0xffefffffu);
    }
    return norrath_creature_finish_construction(object);
}
