#include "heap.hpp"
extern "C" void* norrath_mem_alloc(int requested_bytes, char* allocation_name) {
    using namespace norrath_heap_detail;
    // The original records the actual incoming return PC, not a constant game
    // address. Rebuilt callers must retain their own genuine execution provenance.
    const Address caller = address(__builtin_return_address(0));
    const unsigned requested = static_cast<unsigned>(requested_bytes > 64 ? requested_bytes : 64);
    norrath_mem_check_heap_real(text(norrath_heap_check_file_view), 502);
    Address header = read_word(address(norrath_heap_ring_word_view));
    const Address initial_header = header;
    unsigned payload = 0;
    unsigned charged_size = 0;
    for (;;) {
        if (read_half(header + 2u) == 0u) {
            payload = header + 28u;
            const bool needs_alignment = (payload & 255u) != 0u;
            unsigned displacement = ~0u;
            unsigned needed = requested;
            if (needs_alignment) {
                const unsigned base = read_word(address(norrath_heap_fragment_threshold_view)) + 28u;
                const unsigned candidate = header + base;
                // Original signed SRA sequence computes division by256 toward0
                // after adding255 only to a negative candidate, then remainder.
                const int quotient = signed_word(candidate) / 256;
                const unsigned remainder = candidate - static_cast<unsigned>(quotient * 256);
                displacement = base - remainder + 256u;
                needed += displacement;
            }
            needed = (needed + 7u) & 0xfffffff8u;
            unsigned available = read_word(header + 4u);
            if (!(signed_word(available) < signed_word(needed))) {
                if (needs_alignment) {
                    const unsigned prefix_size = displacement - 56u;
                    const Address aligned_header = header + prefix_size + 28u;
                    const Address old_next = read_word(header + 12u);
                    const unsigned aligned_available = available - prefix_size - 28u;
                    write_word(aligned_header + 16u, header);
                    payload = header + prefix_size + 56u;
                    write_word(aligned_header + 12u, old_next);
                    write_word(aligned_header + 4u, aligned_available);
                    const Address fresh_next = read_word(header + 12u);
                    write_word(header + 4u, prefix_size);
                    write_word(fresh_next + 16u, aligned_header);
                    write_word(header + 12u, aligned_header);
                    header = aligned_header;
                    write_half(header, 0xabcdu);
                    write_half(header + 2u, 0u);
                    write_word(header + 8u, 0u);
                    available = read_word(header + 4u);
                }
                const unsigned remainder = available - needed;
                if (signed_word(read_word(address(norrath_heap_fragment_threshold_view))) < signed_word(remainder)) {
                    const Address old_next = read_word(header + 12u);
                    const Address tail = header + needed + 28u;
                    write_word(tail + 12u, old_next);
                    available = needed;
                    write_word(tail + 16u, header);
                    write_word(tail + 4u, remainder - 28u);
                    const Address fresh_next = read_word(header + 12u);
                    write_half(tail, 0xabcdu);
                    write_word(fresh_next + 16u, tail);
                    write_half(tail + 2u, 0u);
                    write_word(header + 12u, tail);
                    write_word(tail + 8u, 0u);
                    write_word(header + 4u, needed);
                }
                charged_size = available;
                break;
            }
        }
        header = read_word(header + 12u);
        if (header == initial_header) {
            norrath_mem_dump_heap();
            norrath_sce_printf(text(norrath_heap_failure_format_view), caller, signed_word(requested));
            norrath_loader_failure_word_view = 0u;
            return nullptr;
        }
    }
    const unsigned free_bytes = read_word(address(norrath_heap_free_bytes_view));
    const unsigned tag = read_word(address(norrath_heap_allocation_tag_view));
    write_word(address(norrath_heap_free_bytes_view), free_bytes - charged_size - 28u);
    write_word(header + 8u, address(allocation_name));
    write_half(header + 2u, 1u);
    write_word(header + 20u, tag);
    const Address next = read_word(header + 12u);
    write_word(address(norrath_heap_next_fit_view), next);
    write_word(header + 24u, caller);
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(payload));
}
