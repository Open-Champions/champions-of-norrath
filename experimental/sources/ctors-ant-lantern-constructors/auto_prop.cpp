#include "detail.hpp"
// __8AutoPropPcG6Point3sPPc at 0x220278, 1328 bytes.
// Observed: AutoObject receives the object, the name, a point snapshot, the short
// and the argument list. Word 396 is -1, _vt$8AutoProp (0x44ec40) goes to +72 and
// words 376, 384 and 388 are cleared. Seven integer tags set bits of the word at
// +284: 0x470df8 -> 4, 0x485530 -> 8, 0x470e08 -> 0x10 (this one also ORs 0x10000
// into +12), 0x470e18 -> 0x20, 0x470e28 -> 0x40, 0x470e38 -> 0x80, 0x470e48 ->
// 0x100. The integer tag at 0x485538 is stored at +288 even when zero. If it is
// negative nothing is looked up; if it is positive, for i = 0 .. +288 inclusive,
// sprintf(buffer, <0x470e58>, name+76, i) and lumpFindResource(object+109, buffer)
// store the result at +296 + 8 * i, re-reading +288 each time. If the tag is zero,
// sprintf(buffer, <0x485540>, name) and the same lookup go to +296. A string tag
// at 0x470e68 is scanned for up to ten characters; each '1' ORs 1 into the word
// at +300 + 8 * index. An integer tag at 0x470e78 ORs 1 into +284, queries the tag
// at 0x470e88 (the result is discarded), finds the closest door to a copy of
// object+16 in the world held at 0x4844a0, stores it at +396, sets its collision
// to 1, and stores the door height as a single at +400; if +288 is zero then +288
// becomes 2, +296 moves to +304, +308 is 1 and +296 is cleared. Otherwise an
// integer tag at 0x470e98 ORs 2 into +284 and, if +288 is zero, moves +296 to +304
// with +288 = 2 and clears +296; when that tag is absent the existing +296 header
// is animated as below. A non-null header at +296 is added to the state at +152
// (flag 6 then set-to-end after the door path, flag 1 then randomise time
// otherwise). The name is copied into +216 and +248 with the suffixes at 0x485480
// and 0x485488. The 64-bit modelGetActiveChunks(word +144, word +384) goes to
// +200, setPassableState(object, word +300 & 1) runs, and an integer tag at
// 0x470ea8, when present, is read again and passed to setState. The object joins
// the slow run list. A name equal to the text at 0x470eb8 edits the header at
// +296 in place (bytes 24, 32, 40, 48 and singles 0.25, 1.0, and the globals at
// 0x481704 and 0x481708); a name equal to the text at 0x485548 sets byte 40 to
// 0xa8. Word 292 is cleared at the end. V0 is not established; this returns 0.
extern "C" unsigned norrath_construct_AutoProp(void* object, char* name, const void* point, short type, char** arguments) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoObject(object, name, &local, type, arguments);
    const Address a = address(object);
    write_word(a + 396u, 0xffffffffu);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44ec40));
    write_word(a + 376u, 0u);
    write_word(a + 384u, 0u);
    write_word(a + 388u, 0u);
    const auto tag = [&](const unsigned char* view) {
        return norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(const_cast<unsigned char*>(view)), 0);
    };
    if (tag(norrath_ctors_ant_lantern_view_470df8) != 0) write_word(a + 284u, read_word(a + 284u) | 0x4u);
    if (tag(norrath_ctors_ant_lantern_view_485530) != 0) write_word(a + 284u, read_word(a + 284u) | 0x8u);
    if (tag(norrath_ctors_ant_lantern_view_470e08) != 0) {
        write_word(a + 12u, read_word(a + 12u) | 0x10000u);
        write_word(a + 284u, read_word(a + 284u) | 0x10u);
    }
    if (tag(norrath_ctors_ant_lantern_view_470e18) != 0) write_word(a + 284u, read_word(a + 284u) | 0x20u);
    if (tag(norrath_ctors_ant_lantern_view_470e28) != 0) write_word(a + 284u, read_word(a + 284u) | 0x40u);
    if (tag(norrath_ctors_ant_lantern_view_470e38) != 0) write_word(a + 284u, read_word(a + 284u) | 0x80u);
    if (tag(norrath_ctors_ant_lantern_view_470e48) != 0) write_word(a + 284u, read_word(a + 284u) | 0x100u);
    char* const own_name = reinterpret_cast<char*>(pointer(a + 76u));
    char* const lump_name = reinterpret_cast<char*>(pointer(a + 109u));
    char buffer[32];
    const int count = tag(norrath_ctors_ant_lantern_view_485538);
    write_word(a + 288u, static_cast<unsigned>(count));
    if (count == 0) {
        norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485540), own_name);
        write_word(a + 296u, address_bits(norrath_lump_find_resource(lump_name, buffer)));
    } else if (count >= 0) {
        unsigned index = 0u;
        do {
            norrath_factory_sprintf(buffer, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_470e58), own_name,
                                    static_cast<int>(index));
            write_word(a + 296u + index * 8u, address_bits(norrath_lump_find_resource(lump_name, buffer)));
            ++index;
        } while (!(static_cast<int>(read_word(a + 288u)) < static_cast<int>(index)));
    }
    const char* flags = norrath_actor_find_tag_string(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_470e68), 0);
    if (flags != nullptr) {
        for (unsigned index = 0u; index < 10u && flags[index] != 0; ++index)
            if (flags[index] == '1') write_word(a + 300u + index * 8u, read_word(a + 300u + index * 8u) | 1u);
    }
    bool door_path = false;
    if (tag(norrath_ctors_ant_lantern_view_470e78) != 0) {
        door_path = true;
        write_word(a + 284u, read_word(a + 284u) | 0x1u);
        (void)tag(norrath_ctors_ant_lantern_view_470e88);
        void* const world = pointer(read_word(address(norrath_actors_b_view_4844a0)));
        const PointBytes position = snapshot_point(pointer(a + 16u));
        const int door = norrath_world_find_closest_door(world, &position);
        write_word(a + 396u, static_cast<unsigned>(door));
        (void)norrath_world_set_door_collision(world, door, 1);
        const float height = static_cast<float>(norrath_world_get_door_height(world, static_cast<int>(read_word(a + 396u))));
        unsigned height_bits;
        __builtin_memcpy(&height_bits, &height, 4);
        write_word(a + 400u, height_bits);
        if (read_word(a + 288u) == 0u) {
            const unsigned header = read_word(a + 296u);
            write_word(a + 288u, 2u);
            write_word(a + 304u, header);
            write_word(a + 308u, 1u);
            write_word(a + 296u, 0u);
        }
    } else {
        if (tag(norrath_ctors_ant_lantern_view_470e98) != 0) {
            write_word(a + 284u, read_word(a + 284u) | 0x2u);
            if (read_word(a + 288u) == 0u) {
                const unsigned header = read_word(a + 296u);
                write_word(a + 288u, 2u);
                write_word(a + 304u, header);
                write_word(a + 296u, 0u);
            }
            door_path = true;
        }
    }
    void* const header_state = pointer(a + 152u);
    const unsigned header = read_word(a + 296u);
    if (door_path) {
        if (header != 0u) {
            norrath_actor_animation_add(header_state, pointer(header), 6);
            norrath_animation_set_to_end(header_state);
        }
    } else if (header != 0u) {
        norrath_actor_animation_add(header_state, pointer(header), 1);
        norrath_actor_animation_randomize_time(header_state);
    }
    char* const first_copy = reinterpret_cast<char*>(pointer(a + 216u));
    char* const second_copy = reinterpret_cast<char*>(pointer(a + 248u));
    strcpy(first_copy, own_name);
    strcat(first_copy, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485480));
    strcpy(second_copy, own_name);
    strcat(second_copy, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485488));
    const unsigned long long active = norrath_actor_model_active_chunks(pointer(read_word(a + 144u)),
                                                                     static_cast<int>(read_word(a + 384u)));
    write_word(a + 200u, static_cast<unsigned>(active));
    write_word(a + 204u, static_cast<unsigned>(active >> 32));
    setPassableState__8AutoPropb(object, static_cast<int>(read_word(a + 300u) & 1u));
    if (tag(norrath_ctors_ant_lantern_view_470ea8) != 0) setState__8AutoPropi(object, tag(norrath_ctors_ant_lantern_view_470ea8));
    (void)norrath_actor_add_slow_run_list(object);
    if (norrath_factory_strcasecmp(own_name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_470eb8)) == 0) {
        const unsigned edit = read_word(a + 296u);
        if (edit != 0u) {
            const Address h = address(pointer(edit));
            bytes(h)[24] = 1u;
            write_word(h + 20u, 0x3e800000u);
            bytes(h)[32] = 2u;
            write_word(h + 28u, 0x3f800000u);
            bytes(h)[40] = 1u;
            write_word(h + 36u, read_word(address(norrath_ctors_ant_lantern_view_481704)));
            bytes(h)[48] = 2u;
            write_word(h + 44u, read_word(address(norrath_ctors_ant_lantern_view_481708)));
        }
    } else if (norrath_factory_strcasecmp(own_name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485548)) == 0) {
        bytes(a)[40] = 0xa8u;
    }
    write_word(a + 292u, 0u);
    return 0u;
}
