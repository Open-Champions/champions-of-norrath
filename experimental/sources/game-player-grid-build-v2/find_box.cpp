#include "player_grid.hpp"
extern "C" void** norrath_object_find_in_box(NorrathPoint3 minimum, NorrathPoint3 maximum, void** output, int capacity) {
    using namespace norrath_player_grid_detail;
    // Snapshot both complete point representations before any output writes.
    alignas(16) unsigned char workspace[32];
    const Address low = address(workspace), high = low + 16u;
    copy_point(low, address(&minimum)); copy_point(high, address(&maximum));
    int first_y = biased_cell(read_float(low + 4u), 15264u);
    int last_y = biased_cell(read_float(high + 4u), 15456u);
    int first_x = biased_cell(read_float(low), 15264u);
    int last_x = biased_cell(read_float(high), 15456u);
    if (first_y < 0) first_y = 0;
    if (last_y >= 120) last_y = 119;
    if (first_x < 0) first_x = 0;
    if (last_x >= 120) last_x = 119;
    unsigned matched = 0u;
    const Address destination = address(output);
    const int last_slot = signed_word(unsigned(capacity) - 1u);
    if (last_y >= first_y) {
        int y = first_y;
        do {
            if (last_x >= first_x) {
                int x = first_x;
                do {
                    Address node = read_word(address(norrath_constructor_grid_a_view) +
                                              (unsigned(y) * 120u + unsigned(x)) * 4u);
                    while (node != 0u) {
                        const Address object = read_word(node + 12u);
                        // The original reloads the global mask even for object0.
                        const unsigned ignored = read_word(address(norrath_grid_ignore_flags_view));
                        bool selected = false;
                        if (object != 0u && (read_word(object + 12u) & ignored) == 0u) {
                            const float object_x = read_float(object + 16u);
                            const float radius = read_float(object + 28u);
                            const float right = object_x + radius;
                            if (!(right < read_float(low))) {
                                const float left = object_x - radius;
                                if (!(read_float(high) < left)) {
                                    const float object_y = read_float(object + 20u);
                                    const float front = object_y + radius;
                                    if (!(front < read_float(low + 4u))) {
                                        const float back = object_y - radius;
                                        if (!(read_float(high + 4u) < back)) {
                                            const float bottom = read_float(object + 24u);
                                            if (!(read_float(high + 8u) < bottom)) {
                                                const float top = bottom + read_float(object + 32u);
                                                if (!(top < read_float(low + 8u))) selected = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (selected) {
                            if (!(signed_word(matched) < last_slot)) goto complete;
                            write_word(destination + matched * 4u, object); matched += 1u;
                        }
                        node = read_word(node);
                    }
                    x = signed_word(unsigned(x) + 1u);
                } while (x <= last_x);
            }
            y = signed_word(unsigned(y) + 1u);
        } while (y <= last_y);
    }
complete:
    const Address terminator = destination + matched * 4u;
    write_word(terminator, 0u);
    // Actual V0 is the written terminal slot address, not matched count.
    return reinterpret_cast<void**>(static_cast<__UINTPTR_TYPE__>(terminator));
}
