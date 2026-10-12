#include "detail.hpp"
// __13AutoContainerPcG6Point3sPPc at 0x221118, 1876 bytes.
// Observed: AutoObject receives the object, the name, a point snapshot, the short
// and the argument list. _vt$13AutoContainer (0x44ebb0) goes to +72. The name at
// +76 is copied to +224 with the suffix at 0x470ff0 appended and looked up in the
// lump named at +109 (result +216); then copied to +256 with the suffix at
// 0x471000 appended and looked up the same way (result +220). Integer tags at
// 0x470df8, 0x471010 and 0x471020 OR 1, 4 and 8 into +292. Byte 288 is cleared;
// a string tag at 0x4854c8 compared (case-insensitively) with the texts at
// 0x485560, 0x485568, 0x485570 and 0x485578 sets it to 0, 2, 2 and 1. The name is
// then compared with strticmp against 29 texts in a fixed order, each match
// storing 0, 1 or 2 into byte 288 (later matches overwrite earlier ones): the
// 8-byte-spaced run 0x485580..0x4855d0 and the 16-byte-spaced run 0x471030..0x471140
// in the order of the table below. Bit 4 of +292 ORs 0x10000 into +12, then 0x10 is
// ORed into +12 either way. Word 308 is -1, 304 is 0, the eight bytes at +200 are
// all 0xff, and byte 312 is the low byte of worldCheckForWater(world at
// 0x4844a0, copy of object+16, 12.0, null). A string tag and then an integer tag at
// 0x471150 gate two Blockers: with a = type + 0x4000, cos = icos(a), sin = isin(a),
// the first is built at object+16 plus (cos, sin, 0) * 50 and stored at +320, the
// second at object+16 plus (cos, sin, 0) * -40 and stored at +324. Each live block
// ORs 0x4000 into its +12 and gets 36.0 at +28; a failed allocation is stored as
// null. When the tags are absent both words are cleared. V0 is not established;
// this returns 0.
namespace {
using namespace norrath_ctors_ant_lantern;
struct Vector3 { float x, y, z; };
// Allocates and constructs one Blocker at object+16 plus direction * distance.
inline __attribute__((always_inline)) unsigned make_blocker(Address object, const Vector3& direction, float distance) {
    unsigned char* block = norrath_block_alloc(76);
    unsigned result = 0u;
    if (block != nullptr) {
        const Address origin = object + 16u;
        const unsigned ox = read_word(origin), oy = read_word(origin + 4u), oz = read_word(origin + 8u);
        float x, y, z;
        __builtin_memcpy(&x, &ox, 4);
        __builtin_memcpy(&y, &oy, 4);
        __builtin_memcpy(&z, &oz, 4);
        const float dx = direction.x * distance, dy = direction.y * distance, dz = direction.z * distance;
        const float position[3] = {x + dx, y + dy, z + dz};
        const unsigned short half = static_cast<unsigned short>(bytes(object)[56] | (bytes(object)[57] << 8));
        (void)norrath_construct_Blocker(block, position, static_cast<short>(half), nullptr);
        result = address_bits(block);
    }
    return result;
}
}
extern "C" unsigned norrath_construct_AutoContainer(void* object, char* name, const void* point, short type, char** arguments) {
    PointBytes local = snapshot_point(point);
    (void)norrath_construct_AutoObject(object, name, &local, type, arguments);
    const Address a = address(object);
    char* const own_name = reinterpret_cast<char*>(pointer(a + 76u));
    char* const lump_name = reinterpret_cast<char*>(pointer(a + 109u));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44ebb0));
    char* const first_copy = reinterpret_cast<char*>(pointer(a + 224u));
    strcpy(first_copy, own_name);
    strcat(first_copy, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_470ff0));
    write_word(a + 216u, address_bits(norrath_lump_find_resource(lump_name, first_copy)));
    char* const second_copy = reinterpret_cast<char*>(pointer(a + 256u));
    strcpy(second_copy, own_name);
    strcat(second_copy, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_471000));
    write_word(a + 220u, address_bits(norrath_lump_find_resource(lump_name, second_copy)));
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_470df8), 0) != 0)
        write_word(a + 292u, read_word(a + 292u) | 0x1u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471010), 0) != 0)
        write_word(a + 292u, read_word(a + 292u) | 0x4u);
    if (norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471020), 0) != 0)
        write_word(a + 292u, read_word(a + 292u) | 0x8u);
    bytes(a)[288] = 0u;
    char* const mode_tag = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4854c8);
    if (norrath_actor_find_tag_string(object, mode_tag, 0) != nullptr) {
        const char* mode = norrath_actor_find_tag_string(object, mode_tag, 0);
        if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485560)) == 0) {
            bytes(a)[288] = 0u;
        } else if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485568)) == 0) {
            bytes(a)[288] = 2u;
        } else if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485570)) == 0) {
            bytes(a)[288] = 2u;
        } else if (norrath_factory_strcasecmp(mode, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_485578)) == 0) {
            bytes(a)[288] = 1u;
        }
    }
    // Name table, in call order: {run, index, value}. Run 0 is the 16-byte-spaced
    // run from 0x471030, run 1 the 8-byte-spaced run from 0x485580.
    static const unsigned char order[29][3] = {
        {0, 0, 0}, {1, 0, 1}, {0, 1, 0}, {1, 1, 1}, {1, 2, 1}, {1, 3, 0}, {1, 4, 0}, {1, 5, 2},
        {0, 2, 2}, {0, 3, 2}, {0, 4, 2}, {0, 5, 2}, {0, 6, 2}, {0, 7, 2}, {0, 8, 2}, {0, 9, 2}, {0, 10, 2}, {0, 11, 2},
        {0, 12, 1}, {0, 13, 1}, {0, 14, 1}, {0, 15, 1}, {0, 16, 1},
        {1, 6, 0}, {1, 7, 0}, {1, 8, 0}, {0, 17, 1}, {1, 9, 1}, {1, 10, 1}};
    for (unsigned entry = 0u; entry < 29u; ++entry) {
        const Address text = order[entry][0] == 0u
            ? address(norrath_ctors_ant_lantern_view_471030) + 16u * order[entry][1]
            : address(norrath_ctors_ant_lantern_view_485580) + 8u * order[entry][1];
        if (strticmp__FPCcT0(own_name, reinterpret_cast<const char*>(text)) == 0) bytes(a)[288] = order[entry][2];
    }
    if ((read_word(a + 292u) & 0x4u) != 0u) write_word(a + 12u, read_word(a + 12u) | 0x10000u);
    write_word(a + 308u, 0xffffffffu);
    write_word(a + 12u, read_word(a + 12u) | 0x10u);
    write_word(a + 304u, 0u);
    const PointBytes position = snapshot_point(pointer(a + 16u));
    const int water = norrath_world_check_for_water(pointer(read_word(address(norrath_actors_b_view_4844a0))), &position, 12.0f, nullptr);
    write_word(a + 200u, 0xffffffffu);
    write_word(a + 204u, 0xffffffffu);
    bytes(a)[312] = static_cast<unsigned char>(water);
    char* const gate = reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_471150);
    if (norrath_actor_find_tag_string(object, gate, 0) != nullptr && norrath_game_object_find_tag_int(object, gate, 0) != 0) {
        const int angle = static_cast<int>(type) + 16384;
        const float c = icos__Fi(angle);
        const float s = isin__Fi(angle);
        const Vector3 direction = {c, s, 0.0f};
        const unsigned first = make_blocker(a, direction, 50.0f);
        write_word(a + 320u, first);
        if (first != 0u) {
            write_word(address(pointer(first)) + 12u, read_word(address(pointer(first)) + 12u) | 0x4000u);
            write_word(address(pointer(first)) + 28u, 0x42100000u);
        }
        const unsigned second = make_blocker(a, direction, -40.0f);
        write_word(a + 324u, second);
        if (second != 0u) {
            write_word(address(pointer(second)) + 12u, read_word(address(pointer(second)) + 12u) | 0x4000u);
            write_word(address(pointer(second)) + 28u, 0x42100000u);
        }
    } else {
        write_word(a + 320u, 0u);
        write_word(a + 324u, 0u);
    }
    return 0u;
}
