#include "named_lookup.hpp"
extern "C" unsigned long long norrath_object_find_named(char* name) {
    using namespace norrath_constructor_detail;
    void* matches[2];
    // List lookup writes matches[0] on both a successful and an empty search.
    (void)norrath_object_find_named_list(name, matches, 2ull);
    return static_cast<unsigned long long>(
        static_cast<long long>(signed_word(read_word(address(matches)))));
}
