#include "public_metadata.hpp"
extern "C" unsigned long long norrath_amx_get_public(
    void* amx, long long index, char* name) {
    using namespace norrath_constructor_detail;
    const unsigned record_offset = static_cast<unsigned>(index) * 24u;
    const Address header = read_word(address(amx));
    const int count = norrath_amx_public_metadata_detail::signed_count(header + 32u);
    // Original compares full signed64 index only against signed16 upper bound.
    if (index >= static_cast<long long>(count)) return 20ull;
    const unsigned table_offset = read_word(header + 36u);
    norrath_factory_strcpy(name, text(header + table_offset + record_offset + 4u));
    return 0ull;
}
