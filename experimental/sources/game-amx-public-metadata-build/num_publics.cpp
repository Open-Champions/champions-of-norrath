#include "public_metadata.hpp"
extern "C" unsigned long long norrath_amx_num_publics(void* amx, void* count_word) {
    using namespace norrath_constructor_detail;
    const Address header = read_word(address(amx));
    const int count = norrath_amx_public_metadata_detail::signed_count(header + 32u);
    write_word(address(count_word), static_cast<unsigned>(count));
    return 0ull;
}
