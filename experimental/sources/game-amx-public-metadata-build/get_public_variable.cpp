#include "public_metadata.hpp"
extern "C" unsigned long long norrath_amx_get_public_variable(
    void* amx, long long index, char* name, void* address_word) {
    using namespace norrath_constructor_detail;
    const unsigned record_offset = static_cast<unsigned>(index) * 24u;
    const Address header = read_word(address(amx));
    const int count = norrath_amx_public_metadata_detail::signed_count(header + 56u);
    if (index >= static_cast<long long>(count)) return 20ull;
    const unsigned table_offset = read_word(header + 60u);
    const Address entry = header + table_offset + record_offset;
    norrath_factory_strcpy(name, text(entry + 4u));
    // A real copy may alias the entry; read its value only after that call.
    const unsigned value = read_word(entry);
    write_word(address(address_word), value);
    return 0ull;
}
