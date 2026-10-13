#include "pad_mtap_state.cpp"

#include <cstdint>
#include <cstdio>

struct Span { const void* start; unsigned size; unsigned align; };

int main() {
    if (sizeof norrath_libpad_is_init_word != 4u || sizeof norrath_libpad_is_warning_word != 4u) return 1;
    if (sizeof norrath_libpad_rpc_clients_view != 80u || sizeof norrath_libpad_port_info_view != 240u) return 2;
    if (sizeof norrath_libpad_dma_info_view != 256u || sizeof norrath_libpad_rpc_buffer_view != 128u) return 3;
    if (sizeof norrath_libmtap_port_open_client_view != 40u || sizeof norrath_libmtap_port_close_client_view != 40u) return 4;
    if (sizeof norrath_libmtap_get_connection_client_view != 40u || sizeof norrath_libmtap_change_priority_client_view != 40u) return 5;
    if (sizeof norrath_libmtap_module_version_client_view != 40u || sizeof norrath_libmtap_rpc_buffer_view != 128u) return 6;
    // isInit starts at 0, isWarning at 1 (little-endian word), as in the image's data bytes.
    for (unsigned char b : norrath_libpad_is_init_word) if (b != 0) return 7;
    if (norrath_libpad_is_warning_word[0] != 1 || norrath_libpad_is_warning_word[1] != 0 ||
        norrath_libpad_is_warning_word[2] != 0 || norrath_libpad_is_warning_word[3] != 0) return 8;
    // The bss objects start zeroed.
    const Span bss[] = {
        {norrath_libpad_rpc_clients_view, 80, 16}, {norrath_libpad_port_info_view, 240, 16},
        {norrath_libpad_dma_info_view, 256, 64}, {norrath_libpad_rpc_buffer_view, 128, 64},
        {norrath_libmtap_port_open_client_view, 40, 16}, {norrath_libmtap_port_close_client_view, 40, 8},
        {norrath_libmtap_get_connection_client_view, 40, 8}, {norrath_libmtap_change_priority_client_view, 40, 8},
        {norrath_libmtap_module_version_client_view, 40, 8}, {norrath_libmtap_rpc_buffer_view, 128, 64},
    };
    for (unsigned i = 0; i < sizeof bss / sizeof bss[0]; ++i) {
        const unsigned char* p = static_cast<const unsigned char*>(bss[i].start);
        if (reinterpret_cast<std::uintptr_t>(p) % bss[i].align != 0) return 9;
        for (unsigned k = 0; k < bss[i].size; ++k) if (p[k] != 0) return 10;
        for (unsigned j = 0; j < i; ++j) {  // no two objects overlap
            const std::uintptr_t a = reinterpret_cast<std::uintptr_t>(bss[i].start);
            const std::uintptr_t b = reinterpret_cast<std::uintptr_t>(bss[j].start);
            if (a < b + bss[j].size && b < a + bss[i].size) return 11;
        }
    }
    std::puts("pad and multitap state contract passed");
    return 0;
}
