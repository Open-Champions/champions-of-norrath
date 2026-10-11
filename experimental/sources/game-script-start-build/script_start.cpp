#include "script_start.hpp"

extern "C" int norrath_script_start(char* name) {
    using namespace norrath_constructor_detail;
    if (name == nullptr) return -1;

    const Address active_amx = read_word(address(norrath_script_current_amx_word_view));
    if (active_amx == 0u) return -1;

    // FindPublic writes the index on its genuine return paths. It remains
    // uninitialized before that call, as in the original caller's stack slot.
    int public_index;
    const unsigned long long status =
        norrath_amx_find_public(pointer(active_amx), name, &public_index);
    // This is the original caller's low-word status publication, not a supplied
    // interpreter result. Exec may leave it unchanged on an early error path.
    int result = signed_word(static_cast<unsigned>(status));
    if (status != 0ull) {
        const unsigned frame = read_word(address(norrath_route_draw_frame_bytes));
        const Address threshold_address = address(norrath_script_complain_frame_word_view);
        const unsigned threshold = read_word(threshold_address);
        if (signed_word(threshold) < signed_word(frame))
            write_word(threshold_address, frame + 20u);
        return -1;
    }

    // The five original lanes are A0=AMX, A1=result, A2=signed index,
    // A3=zero count and T0=null argument-data pointer. Preserve the captured AMX.
    // The interpreter's status is deliberately ignored; return its result word.
    norrath_amx_exec(pointer(active_amx), &result,
                    static_cast<long long>(public_index), 0ll, nullptr);
    return result;
}
