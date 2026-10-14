#include "detail.hpp"
// __10FireBeetleG6Point3sPPci at 0x20ff48, 1008 bytes.
// Observed: eRand is called first; the Creature scale is (eRand % 6) + 8 as a
// single. Creature receives the snapshot, the short, the 152-byte record at
// 0x42c308 + 152 * the int argument as its map, and the argument list.
// _vt$10FireBeetle (0x44bea0) goes to +72. Halves at 1052, 1044, 1036 and 1028
// are 0xffff, bytes 1024 and 1025 are 0xff and the 16 bytes at 1008..1023 are
// zero. setParameters gets 1.0, the global single at 0x4814e4 three times (slots
// 2, 3 and 5) and 1.0 (slot 4), then 1. Next: the high word at +164 gets bit 0x80
// and the low word at +160 gets 0x1000; 20.0 goes to +168, 12.0 to +28, -0.25 to
// +92, 180.0 to +176, 1.0 to +88, the word at 0x4844a4 to +248, byte 172 is 18 and
// +236, +964 and half 572 are cleared. A second eRand gives the factor
// (r % 6) * [0x4814e8] + [0x4814ec], stored at +224 and multiplied into the
// single already at +28. The animation state at +288 is initialised, receives the
// header word at model+84 (flag 1) and gets a randomised time. A third eRand,
// scaled by 2^-16 and compared with the single at 0x4814f0, can enter a block: the
// object whose pointer is stored at 0x34b474 is called through the pointer at its
// offset 96 (short adjustment at that table+96, function at +4) with 0 and the
// truncated single from creature view 0x484cac; its integer result K is kept. A
// fourth eRand scaled by 2^-16 and K * [0x4814f4] give the value K + (int)(that
// product). The four 8-byte entries at 1028 are searched for the first whose half
// is 0xffff or 21; its half becomes 21, the next half K, the one after K plus the
// product and the one after that 0. The object then joins the slow run list, four
// setAnimEventFlags calls follow (3 with the single at 0x4814f8, 4, 5, 6 with
// 0.5), bit 1 is ORed into +244, modelGetActiveChunks(model->+12, that word) is
// stored in eight bytes at +552 and V0 is finishConstruction's.
extern "C" unsigned norrath_construct_FireBeetle(void* object, const void* point, short type, char** arguments, int kind) {
    using namespace norrath_ctors_ant_lantern;
    PointBytes local = snapshot_point(point);
    const unsigned first_roll = norrath_actor_random(object);
    const float scale = static_cast<float>(static_cast<int>(first_roll % 6u + 8u));
    (void)norrath_creature_construct(object, &local, type,
                                     pointer(address(norrath_ctors_ant_lantern_view_42c308) + static_cast<unsigned>(kind) * 152u),
                                     scale, arguments);
    const Address a = address(object);
    write_word(a + 72u, address_bits(norrath_ctors_ant_lantern_view_44bea0));
    for (unsigned index = 0u; index < 4u; ++index) write_half(a + 1052u - index * 8u, 0xffffu);
    bytes(a)[1025] = 0xffu;
    bytes(a)[1024] = 0xffu;
    for (unsigned offset = 1008u; offset < 1024u; offset += 2u) write_half(a + offset, 0u);
    const float shared = read_single(norrath_ctors_ant_lantern_view_4814e4);
    (void)norrath_actor_set_parameters(object, 1.0f, shared, shared, 1.0f, shared, 1);
    write_word(a + 164u, read_word(a + 164u) | 0x80u);
    write_word(a + 160u, read_word(a + 160u) | 0x1000u);
    bytes(a)[172] = 18u;
    write_word(a + 176u, 0x43340000u);
    write_word(a + 168u, 0x41a00000u);
    write_word(a + 28u, 0x41400000u);
    write_word(a + 248u, read_word(address(norrath_actor_view_4844a4)));
    write_word(a + 92u, 0xbe800000u);
    write_word(a + 88u, 0x3f800000u);
    write_word(a + 236u, 0u);
    write_half(a + 572u, 0u);
    write_word(a + 964u, 0u);
    const unsigned second_roll = norrath_actor_random(object);
    const float factor = static_cast<float>(static_cast<int>(second_roll % 6u)) *
                             read_single(norrath_ctors_ant_lantern_view_4814e8) +
                         read_single(norrath_ctors_ant_lantern_view_4814ec);
    float stored_scale;
    {
        const unsigned word = read_word(a + 28u);
        __builtin_memcpy(&stored_scale, &word, 4);
    }
    const float scaled = stored_scale * factor;
    {
        unsigned word;
        __builtin_memcpy(&word, &factor, 4);
        write_word(a + 224u, word);
        __builtin_memcpy(&word, &scaled, 4);
        write_word(a + 28u, word);
    }
    void* const state = pointer(a + 288u);
    norrath_actor_animation_initialize(state);
    const unsigned char* model = static_cast<const unsigned char*>(pointer(read_word(a + 36u)));
    norrath_actor_animation_add(state, pointer(read_word(address(model) + 84u)), 1);
    norrath_actor_animation_randomize_time(state);
    const float third = static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f;
    if (third < read_single(norrath_ctors_ant_lantern_view_4814f0)) {
        const int requested = static_cast<int>(read_single(norrath_creature_view_484cac));
        const Address other = read_word(address(norrath_ctors_ant_lantern_view_34b474));
        const unsigned char* table = static_cast<const unsigned char*>(pointer(read_word(address(pointer(other)) + 96u))) + 96;
        const int adjust = static_cast<short>(table[0] | (table[1] << 8));
        typedef int (*Call)(void*, int, int);
        const int result = reinterpret_cast<Call>(pointer(read_word(address(table) + 4u)))(
            pointer(address(pointer(other)) + static_cast<Address>(adjust)), 0, requested);
        const float product = static_cast<float>(result) * read_single(norrath_ctors_ant_lantern_view_4814f4) *
                              (static_cast<float>(static_cast<int>(norrath_actor_random(object))) * 1.52587890625e-05f);
        const int total = result + static_cast<int>(product);
        for (unsigned slot = 0u; slot < 4u; ++slot) {
            const Address entry = a + 1028u + slot * 8u;
            const unsigned head = bytes(entry)[0] | (bytes(entry)[1] << 8);
            if (head == 0xffffu || head == 21u) {
                write_half(entry, 21u);
                write_half(entry + 6u, 0u);
                write_half(entry + 2u, static_cast<unsigned>(result));
                write_half(entry + 4u, static_cast<unsigned>(total));
                break;
            }
        }
    }
    (void)norrath_actor_add_slow_run_list(object);
    setAnimEventFlags__8Creatureififififi(object, 3, read_single(norrath_ctors_ant_lantern_view_4814f8), 1, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 4, 0.5f, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 5, 0.5f, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    setAnimEventFlags__8Creatureififififi(object, 6, 0.5f, 2, 0.0f, 0, 0.0f, 0, 0.0f, 0);
    const unsigned flagged = read_word(a + 244u) | 1u;
    write_word(a + 244u, flagged);
    const unsigned long long active = norrath_actor_model_active_chunks(
        pointer(read_word(address(model) + 12u)), static_cast<int>(flagged));
    write_word(a + 552u, static_cast<unsigned>(active));
    write_word(a + 556u, static_cast<unsigned>(active >> 32));
    return norrath_actor_finish_construction(object);
}
