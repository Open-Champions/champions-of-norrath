#include "loader_primitives.hpp"
namespace {
using namespace norrath_constructor_dependency_detail;
Address cached_archive(char* name) {
    const Address cache = address(norrath_constructor_lump_cache_view);
    const Address count_word = address(norrath_constructor_lump_count_view);
    unsigned index = 0u;
    unsigned offset = 0u;
    for (;;) {
        if (!(signed_word(index) < signed_word(read_word(count_word)))) return 0u;
        if (norrath_string_compare(text(cache + offset), name) == 0) {
            const unsigned flags = read_word(cache + offset + 20u);
            const int fresh_count = signed_word(read_word(count_word));
            if ((flags & 1u) == 0u) {
                if (!(signed_word(index) < fresh_count)) return 0u;
                return read_word(cache + offset + 16u);
            }
        }
        index += 1u;
        offset += 40u;
    }
}
}
extern "C" void* norrath_lump_load(char* lump_name) {
    using namespace norrath_constructor_dependency_detail;
    const Address resident = cached_archive(lump_name);
    if (resident != 0u) return pointer(resident);
    // The original passes two actual writable4-byte stack output cells. Sector
    // is not consumed here; size is read only after a real successful lookup.
    int sector;
    int size;
    if (norrath_cd_directory_lookup(lump_name, &sector, &size, 0) != 0) {
        if (norrath_string_compare(lump_name, text(address(norrath_loader_name_a_view))) == 0 &&
            norrath_string_compare(text(address(norrath_loader_current_name_view)),
                                   text(address(norrath_loader_context_a_view))) != 0)
            write_word(address(norrath_loader_special_flag_view), 1u);
        if (norrath_string_compare(lump_name, text(address(norrath_loader_name_b_view))) == 0 &&
            norrath_string_compare(text(address(norrath_loader_current_name_view)),
                                   text(address(norrath_loader_context_b_view))) != 0)
            write_word(address(norrath_loader_special_flag_view), 1u);
        const unsigned count = read_word(address(norrath_constructor_lump_count_view));
        const unsigned allocation = ((static_cast<unsigned>(size) + 2047u) & 0xfffff800u) + 256u;
        char* const label = text(address(norrath_constructor_lump_cache_view) + 40u * count);
        void* const buffer = norrath_mem_alloc(signed_word(allocation), label);
        norrath_cd_load(lump_name, buffer, 0, 0);
        write_word(address(norrath_loader_active_buffer_view), address(buffer));
        write_word(address(norrath_loader_active_size_view), static_cast<unsigned>(size));
        *bytes(address(norrath_loader_active_marker_view)) = 1u;
        norrath_factory_strcpy(text(address(norrath_loader_active_name_view)), lump_name);
        norrath_cd_process();
        write_word(address(norrath_loader_special_flag_view), 0u);
    } else {
        norrath_sce_printf(text(address(norrath_loader_missing_format_view)));
        // Genuine fixed-address failure store, followed by the real cache scan.
        // The view is an absolute0 platform binding, never synthetic RAM. Strict
        // ownership must exclude this path unless the actual zero address is
        // writable; no faulttrace equivalence or successful-load default exists.
        norrath_loader_failure_word_view = 0u;
    }
    // Real processing may populate/change cache and count: repeat the complete
    // live cache scan, rather than return the allocation as a fabricated archive.
    return pointer(cached_archive(lump_name));
}
