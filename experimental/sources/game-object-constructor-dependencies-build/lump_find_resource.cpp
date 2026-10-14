#include "dependencies.hpp"
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name) {
    using namespace norrath_constructor_dependency_detail;
    const Address cache = address(norrath_constructor_lump_cache_view);
    const Address count_word = address(norrath_constructor_lump_count_view);
    Address lump = 0u;
    unsigned index = 0u;
    unsigned offset = 0u;
    int live_count = signed_word(read_word(count_word));
    if (live_count > 0) {
        for (;;) {
            const int name_comparison = norrath_string_compare(text(cache + offset), lump_name);
            if (name_comparison == 0) {
                if ((read_word(cache + offset + 20u) & 1u) != 0u) {
                    const Address resident_name = read_word(cache + offset + 16u);
                    if (norrath_string_compare(text(resident_name), resource_name) == 0)
                        return pointer(read_word(cache + offset + 16u) + 256u);
                } else {
                    lump = read_word(cache + offset + 16u);
                    break;
                }
            }
            live_count = signed_word(read_word(count_word));
            index += 1u;
            offset += 40u;
            if (!(signed_word(index) < live_count)) break;
        }
    }
    if (lump == 0u) {
        lump = address(norrath_lump_load(lump_name));
        if (lump == 0u) return nullptr;
    }
    // Complete inline original search. It returns the selected record's data
    // word, whereas the separate genuine lumpFind returns the record itself.
    const Address records = lump + 4u;
    const int initial_maximum = static_cast<int>(read_half(lump)) - 1;
    int lower = 0;
    int upper = initial_maximum;
    for (;;) {
        const unsigned sum = static_cast<unsigned>(lower) + static_cast<unsigned>(upper);
        const int middle = signed_word(arithmetic_shift(sum, 1u));
        int relation;
        if (middle < 0) relation = 1;
        else if (initial_maximum < middle) relation = -1;
        else {
            const Address record = records + 12u * static_cast<unsigned>(middle);
            relation = norrath_string_compare(resource_name, text(read_word(record)));
        }
        const Address record = records + 12u * static_cast<unsigned>(middle);
        if (relation == 0) return pointer(read_word(record + 4u));
        if (!(lower < upper)) return nullptr;
        if (relation <= 0) upper = signed_word(static_cast<unsigned>(middle) - 1u);
        else lower = signed_word(static_cast<unsigned>(middle) + 1u);
    }
}
