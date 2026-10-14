#include "animation_dependencies.hpp"
using namespace creature_detail;
namespace {
// Actual LW/LHU polling field views. Original instructions justify their 4/2
// alignment and widths; no additional field, memory owner or callback is added.
unsigned polled_word(Address a) {
    return *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(a));
}
unsigned polled_half(Address a) {
    return *reinterpret_cast<volatile unsigned short*>(static_cast<__UINTPTR_TYPE__>(a));
}
}

// The entire actual LOCAL getFreeFrameState body. This explicit rebuilt machine
// interface is for the real private source closure; original LOCAL strict-public
// admission stays rejected. No original global alias or pool owner is invented.
extern "C" void* norrath_animation_frame_state_acquire() {
    Address head = word(address(norrath_creature_view_486910));
    Address node = head;
    Address owner = polled_word(head + 20u);
    if (owner != 0u && (polled_half(owner + 28u) & 0x4800u) != 0u) {
        Address tail = word(address(norrath_creature_view_48690c));
        Address first = head;
        for (;;) {
            const Address initial_next = polled_word(node + 16u);
            if (initial_next != 0u) {
                Address previous = word(node + 12u);
                if (previous == 0u) first = initial_next;
                else {
                    put_word(previous + 16u, initial_next);
                    previous = word(node + 12u);
                }
                head = first;
                const Address fresh_next = polled_word(node + 16u);
                put_word(fresh_next + 12u, previous);
                put_word(node + 12u, tail);
                put_word(tail + 16u, node);
                tail = node;
                put_word(tail + 16u, 0u);
            }
            // Preserve actual rechecks, including next==0: a later owner/flag/
            // link change may release the poll. No exhaustion or null repair.
            owner = polled_word(head + 20u);
            node = head;
            if (owner == 0u || (polled_half(owner + 28u) & 0x4800u) == 0u) break;
        }
        put_word(address(norrath_creature_view_48690c), tail);
        put_word(address(norrath_creature_view_486910), first);
    }
    const Address selected = head;
    const Address next = word(selected + 16u);
    const Address old_owner = word(selected + 20u);
    put_word(address(norrath_creature_view_486910), next);
    put_word(next + 12u, 0u); // Actual unconditional store, even for next==0.
    if (old_owner != 0u) put_word(old_owner + 4u, 0u);
    const Address old_tail = word(address(norrath_creature_view_48690c));
    put_word(selected + 12u, old_tail);
    put_word(old_tail + 16u, selected);
    put_word(address(norrath_creature_view_48690c), selected);
    put_word(selected + 8u, 1000000u);
    put_word(selected + 16u, 0u);
    put_word(selected + 32u, 0u);
    put_word(selected + 36u, 0u);
    put_word(selected + 20u, 0u);
    return pointer(selected);
}

extern "C" void norrath_model_bounding_box(void* model, int mask,
        int* minimum_x, int* minimum_y, int* minimum_z,
        int* maximum_x, int* maximum_y, int* maximum_z) {
    const Address base = address(model);
    const int count = signed_byte(bytes(base)[18]);
    const int stride_count = signed_byte(bytes(base)[38]);
    const unsigned offset = static_cast<unsigned>(stride_count) *
                            (static_cast<unsigned>(count) * 4u + 4u);
    Address bounds = base + offset + 40u;
    unsigned selection = static_cast<unsigned>(mask);
    int remaining = count - 1;
    int low_x = 100000, low_y = 100000, low_z = 100000;
    int high_x = -100000, high_y = -100000, high_z = -100000;
    if (remaining >= 0 && selection != 0u) {
        do {
            const bool selected = (selection & 1u) != 0u;
            // Original incoming long is canonical signed32 in a 64-bit GPR;
            // DSRA sign propagation has the same low32 bits as this target SRA.
            selection = sra(selection, 1u);
            if (selected) {
                const int x0 = signed_byte(bytes(bounds)[0]);
                const int y0 = signed_byte(bytes(bounds)[1]);
                const int z0 = signed_byte(bytes(bounds)[2]);
                const int x1 = signed_byte(bytes(bounds)[3]);
                const int y1 = signed_byte(bytes(bounds)[4]);
                const int z1 = signed_byte(bytes(bounds)[5]);
                if (x0 < low_x) low_x = x0;
                if (y0 < low_y) low_y = y0;
                if (z0 < low_z) low_z = z0;
                if (high_x < x1) high_x = x1;
                if (high_y < y1) high_y = y1;
                if (high_z < z1) high_z = z1;
            }
            --remaining;
            bounds += 10u;
        } while (remaining >= 0 && selection != 0u);
    }
    // All model reads precede these six stores; aliased outputs use this order.
    put_word(address(minimum_x), static_cast<unsigned>(low_x));
    put_word(address(minimum_y), static_cast<unsigned>(low_y));
    put_word(address(minimum_z), static_cast<unsigned>(low_z));
    put_word(address(maximum_x), static_cast<unsigned>(high_x));
    put_word(address(maximum_y), static_cast<unsigned>(high_y));
    put_word(address(maximum_z), static_cast<unsigned>(high_z));
}

extern "C" int norrath_network_player_connected(int index) {
    if (signed_byte(*norrath_creature_view_483829) == 0) return 1;
    const int partners = signed_word(word(address(norrath_network_partner_count_word_view)));
    if (!(index < partners)) return 0;
    // Actual body has no lower-bound check for a negative index.
    const Address status = address(norrath_animation_dependency_view_4e6755) +
                           static_cast<unsigned>(index) * 17632u;
    return signed_byte(*bytes(status)) == 0 ? 1 : 0;
}

extern "C" float norrath_native_powf(float x, float y) {
    // Entire genuine powf tail wrapper; real 2016-byte mathematical body pending.
    return norrath_math_ieee754_powf(x, y);
}
extern "C" float norrath_native_atan2f(float y, float x) {
    // Entire genuine atan2f tail wrapper; real 480-byte mathematical body pending.
    return norrath_math_ieee754_atan2f(y, x);
}
extern "C" int norrath_integer_atan2(float y, float x) {
    const float radians = norrath_native_atan2f(y, x);
    const float scale = view(norrath_animation_dependency_view_47ff1c);
    const float scaled = radians * scale; // Separate actual single-float MUL.S.
    const unsigned converted = static_cast<unsigned>(chop(scaled));
    return signed_half(converted & 0xffffu);
}
