#include "pointer_cap.hpp"

extern "C" char** norrath_megatile_cap_pm_pointers() {
    using namespace norrath_pointer_cap_detail;
    const Address pool = address(norrath_megatile_pm_pointers_view);
    const Address count_word = address(norrath_megatile_counters_view) + 4u;

    const unsigned old_count = read_word(count_word);
    const Address sentinel = pool + 4u * old_count;
    write_word(count_word, old_count + 1u);
    write_word(sentinel, 0u);
    // The sentinel write may alias the counter: capture the count afterward.
    unsigned retained_count = read_word(count_word);
    const unsigned scan_limit = retained_count;

    Address backward = pool + 4u * retained_count - 8u;
    while (backward >= pool) {
        if (read_word(backward) == 0u) break;
        backward -= 4u;
    }
    const Address final_run = backward + 4u;
    Address result = final_run;
    bool found_earlier_run = false;
    unsigned matched = 0u;
    unsigned scanned = 0u;
    Address earlier = pool;

    while (signed_word(scanned) < signed_word(scan_limit)) {
        const Address probe = final_run + 4u * matched;
        const unsigned probe_value = read_word(probe);
        matched += 1u;
        if (probe_value == 0u) {
            const unsigned earlier_value = read_word(earlier);
            if (earlier_value == 0u && probe != earlier) {
                // Find the beginning of this earlier zero-terminated run.
                backward = earlier - 4u;
                found_earlier_run = true;
                while (backward >= pool) {
                    if (read_word(backward) == 0u) break;
                    backward -= 4u;
                }
                result = backward + 4u;
                break;
            }
        }
        // Original rereads both cells even after the zero tests above.
        const unsigned current_probe = read_word(probe);
        const unsigned current_earlier = read_word(earlier);
        if (current_probe != current_earlier) matched = 0u;
        scanned += 1u;
        earlier += 4u;
    }

    if (found_earlier_run) {
        Address cursor = result;
        while (read_word(cursor) != 0u) {
            retained_count -= 1u;
            cursor += 4u;
            // Preserve each publication before reading the next pointer cell.
            write_word(count_word, retained_count);
        }
        retained_count -= 1u;
        write_word(count_word, retained_count);
    }
    return pointers(result);
}
