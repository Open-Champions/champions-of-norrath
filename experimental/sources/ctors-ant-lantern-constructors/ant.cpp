#include "detail.hpp"
// __3AntG6Point3sPPcPcc at 0x205368, 1556 bytes. The last two arguments are a name
// string and a signed char.
// Observed: eRand is called first; the Creature scale is (roll % 14) + 4 as a
// single. Creature receives the snapshot, the short, the table at 0x42be08 when the
// char is zero and the table at 0x42beb0 otherwise as its map, and the argument
// list. Byte 40 is 20, bit 0x1000 goes into the low word at +160 and 0x80 into the
// high word at +164, 140.0 to +176, the single at 0x48124c to +168, 18.0 to +28, the
// word at 0x4844a4 to +248, _vt$3Ant (0x44c530) to +72, byte 172 is 18 and +192,
// +236, half 572, +972 and +980 are cleared. Byte 960 takes a roll's low bit and is
// then replaced by the integer tag at 0x4852d8 (as a byte); byte 968 takes the
// integer tag at 0x4852e0. The name is compared (strticmp) with the texts at
// 0x46fa28, 0x46fa38, 0x46fa48 and 0x46fa58 in that order: the first match sets
// (960, 968) to (1, unchanged), (1, 1), (0, 1) or (0, 2). +244 is cleared. A char of
// 1 stores the single at 0x481250 at +224 and 20.0 at +28, clears byte 960, sets
// +244 bit 0 from another roll's low bit, calls setParameters (3.0, 1.5, 1.5, 1.0,
// 1.0, 1) and, when byte 968 is 2, stores 25.0 at +28 and the single at 0x481254 at
// +224. Other chars use byte 960 and byte 968 to select among three more
// setParameters calls and the +224 factor, which is either a single or
// (double)((roll % 6) * single) + 0.75 narrowed back to a single. The single at
// 0x481270 or 0x48128c goes to +88. +208 and byte 156 are cleared; with byte 968
// zero, a roll below the single at 0x481290 stores (roll & 1) + 1 at +208; byte 156
// is then 1 for +208 = 1 and 2 for +208 = 2. +28 is multiplied by +224, the
// eight-byte modelGetActiveChunks(model->+12, +244) goes to +552, the animation
// state at +288 receives the header at model+84, the object joins the slow run list,
// six setAnimEventFlags calls follow (4, 5, 14, 6, 7, 8) and V0 is
// finishConstruction's.
extern "C" unsigned norrath_construct_Ant(void* object, const void* point, short type, char** arguments, char* name, char kind) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned scale_roll = norrath_actor_random(object);
    (void)norrath_creature_construct(object, &local, type,
                                     kind == 0 ? norrath_ctors_ant_lantern_view_42be08 : norrath_ctors_ant_lantern_view_42beb0,
                                     static_cast<float>(static_cast<int>(scale_roll % 14u + 4u)), arguments);
    const Address a = address(object);
    bytes(a)[40] = 20u;
    write_word(a + 160u, read_word(a + 160u) | 0x1000u);
    write_word(a + 164u, read_word(a + 164u) | 0x80u);
    write_word(a + 176u, 0x430c0000u);
    write_word(a + 168u, read_word(address(norrath_ctors_ant_lantern_view_48124c)));
    write_word(a + 28u, 0x41900000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44c530));
    bytes(a)[172] = 18u;
    write_word(a + 192u, 0u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 972u, 0u);
    write_word(a + 980u, 0u);
    bytes(a)[960] = static_cast<unsigned char>(norrath_actor_random(object) & 1u);
    bytes(a)[960] = static_cast<unsigned char>(norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4852d8), 0));
    bytes(a)[968] = static_cast<unsigned char>(norrath_game_object_find_tag_int(object, reinterpret_cast<char*>(norrath_ctors_ant_lantern_view_4852e0), 0));
    if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46fa28)) == 0) {
        bytes(a)[960] = 1u;
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46fa38)) == 0) {
        bytes(a)[968] = 1u;
        bytes(a)[960] = 1u;
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46fa48)) == 0) {
        bytes(a)[960] = 0u;
        bytes(a)[968] = 1u;
    } else if (strticmp__FPCcT0(name, reinterpret_cast<const char*>(norrath_ctors_ant_lantern_view_46fa58)) == 0) {
        bytes(a)[960] = 0u;
        bytes(a)[968] = 2u;
    }
    write_word(a + 244u, 0u);
    const auto narrowed = [&](const unsigned char* factor_view) {
        const float scaled = static_cast<float>(static_cast<int>(norrath_actor_random(object) % 6u)) * read_single(factor_view);
        return dptofp(dpadd(norrath_entity_fptodp(scaled), 0x3fe8000000000000ull));
    };
    const auto store_single = [&](unsigned offset, float value) {
        unsigned word;
        __builtin_memcpy(&word, &value, 4);
        write_word(a + offset, word);
    };
    unsigned selector_byte;
    if (kind == 1) {
        write_word(a + 224u, read_word(address(norrath_ctors_ant_lantern_view_481250)));
        write_word(a + 28u, 0x41a00000u);
        bytes(a)[960] = 0u;
        if (((norrath_actor_random(object) & 1u) & 0xffffu) != 0u) write_word(a + 244u, read_word(a + 244u) | 1u);
        (void)norrath_actor_set_parameters(object, 3.0f, 1.5f, 1.5f, 1.0f, 1.0f, 1);
        selector_byte = bytes(a)[968];
        if (static_cast<signed char>(bytes(a)[968]) == 2) {
            write_word(a + 28u, 0x41c80000u);
            write_word(a + 224u, read_word(address(norrath_ctors_ant_lantern_view_481254)));
        }
    } else {
        if (static_cast<signed char>(bytes(a)[960]) == 0) {
            if (static_cast<signed char>(bytes(a)[968]) == 1) {
                write_word(a + 244u, 1u);
                write_word(a + 224u, read_word(address(norrath_ctors_ant_lantern_view_481258)));
                (void)norrath_actor_set_parameters(object, 1.5f, read_single(norrath_ctors_ant_lantern_view_48125c),
                                                   read_single(norrath_ctors_ant_lantern_view_481260), 1.0f, 1.0f, 1);
            } else {
                store_single(224u, narrowed(norrath_ctors_ant_lantern_view_481264));
                (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_481268),
                                                   read_single(norrath_ctors_ant_lantern_view_48126c), 0.5f, 1.0f, 1.0f, 1);
            }
            write_word(a + 88u, read_word(address(norrath_ctors_ant_lantern_view_481270)));
        } else {
            if (static_cast<signed char>(bytes(a)[960]) == 1) {
                write_word(a + 244u, 1u);
                write_word(a + 224u, read_word(address(norrath_ctors_ant_lantern_view_481274)));
                (void)norrath_actor_set_parameters(object, 1.5f, read_single(norrath_ctors_ant_lantern_view_481278),
                                                   read_single(norrath_ctors_ant_lantern_view_48127c), 1.0f, 1.0f, 1);
            } else {
                store_single(224u, narrowed(norrath_ctors_ant_lantern_view_481280));
                (void)norrath_actor_set_parameters(object, read_single(norrath_ctors_ant_lantern_view_481284),
                                                   read_single(norrath_ctors_ant_lantern_view_481288), 0.5f, 1.0f, 1.0f, 1);
            }
            write_word(a + 88u, read_word(address(norrath_ctors_ant_lantern_view_48128c)));
        }
        selector_byte = bytes(a)[968];
    }
    write_word(a + 208u, 0u);
    bytes(a)[156] = 0u;
    if (selector_byte == 0u) {
        const float roll = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
        if (roll < read_single(norrath_ctors_ant_lantern_view_481290)) write_word(a + 208u, (norrath_actor_random(object) & 1u) + 1u);
    }
    const unsigned mode = read_word(a + 208u);
    if (mode == 1u) bytes(a)[156] = 1u;
    else if (mode == 2u) bytes(a)[156] = 2u;
    float factor, base;
    {
        const unsigned factor_bits = read_word(a + 224u), base_bits = read_word(a + 28u);
        __builtin_memcpy(&factor, &factor_bits, 4);
        __builtin_memcpy(&base, &base_bits, 4);
    }
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    const float lead = read_single(norrath_ctors_ant_lantern_view_481294);
    store_single(28u, base * factor);
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(read_word(a + 244u)));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 84u)), 1);
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 4, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, read_single(norrath_ctors_ant_lantern_view_481298), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 14, lead, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 7, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 8, 0.5f, 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    return norrath_actor_finish_construction(object);
}
