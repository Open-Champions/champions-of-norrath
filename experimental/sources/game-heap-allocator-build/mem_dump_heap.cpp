#include "heap.hpp"
extern "C" void norrath_mem_dump_heap() {
    using namespace norrath_heap_detail;
    norrath_sce_printf(text(norrath_heap_dump_border_view));
    norrath_sce_printf(text(norrath_heap_dump_count_format_view),
                      signed_word(read_word(address(norrath_heap_dump_count_view))));
    Address header = read_word(address(norrath_heap_ring_word_view));
    for (;;) {
        const unsigned allocated = read_half(header + 2u);
        const Address payload = header + 28u;
        if (allocated == 0u) {
            const unsigned size = read_word(header + 4u);
            norrath_sce_printf(text(norrath_heap_dump_free_format_view), payload, signed_word(size));
        } else {
            const Address name = read_word(header + 8u);
            const Address caller = read_word(header + 24u);
            const unsigned size = read_word(header + 4u);
            norrath_sce_printf(text(norrath_heap_dump_used_format_view), payload,
                              reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(name)),
                              caller, signed_word(size));
        }
        header = read_word(header + 12u);
        // The real printer may change the global ring pointer: compare freshly.
        if (header == read_word(address(norrath_heap_ring_word_view))) break;
    }
    const unsigned total = read_word(address(norrath_heap_total_bytes_view));
    const unsigned free = read_word(address(norrath_heap_free_bytes_view));
    norrath_sce_printf(text(norrath_heap_dump_totals_format_view), signed_word(total),
                      signed_word(total - free), signed_word(free));
    const unsigned previous = read_word(address(norrath_heap_dump_count_view));
    write_word(address(norrath_heap_dump_count_view), previous + 1u);
    norrath_sce_printf(text(norrath_heap_dump_serial_format_view), signed_word(previous));
    norrath_sce_printf(text(norrath_heap_dump_border_view));
}
