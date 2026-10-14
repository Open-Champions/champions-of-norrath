#include "register.hpp"
#include "dependencies.hpp"
namespace {
using namespace norrath_constructor_detail;
unsigned half(Address p) {
    return unsigned(*bytes(p)) | (unsigned(*bytes(p + 1u)) << 8);
}
int signed_half(unsigned value) {
    return value < 0x8000u ? static_cast<int>(value)
                         : static_cast<int>(value) - 65536;
}
}
extern "C" unsigned long long norrath_amx_register(
    void* amx, const void* registry, long long native_count) {
    using namespace norrath_constructor_detail;
    const Address header = read_word(address(amx));
    Address native = header + read_word(header + 44u);
    const int initial_count = signed_half(half(header + 40u));
    unsigned current_count = half(header + 40u);
    unsigned status = 0u;
    if (initial_count <= 0) return 0ull;
    const Address list = address(registry);
    const bool positive_count = native_count > 0ll;
    unsigned native_index = 0u;
    do {
        if (read_word(native) == 0u) {
            unsigned code = 0u;
            if (read_word(list) != 0u &&
                (positive_count || native_count == -1ll)) {
                unsigned index = 0u;
                Address entry = list;
                for (;;) {
                    // The original rereads this name in the real call delay.
                    const int comparison = norrath_string_compare(
                        text(native + 4u), text(read_word(entry)));
                    index += 1u;
                    if (comparison == 0) {
                        code = read_word(entry + 4u);
                        current_count = half(header + 40u);
                        break;
                    }
                    entry = list + (index << 3);
                    // This read precedes the finite-count check in the original.
                    if (read_word(entry) == 0u) {
                        current_count = half(header + 40u);
                        break;
                    }
                    if (static_cast<long long>(signed_word(index)) < native_count ||
                        native_count == -1ll) continue;
                    current_count = half(header + 40u);
                    break;
                }
            }
            if (code == 0u) status = 19u;
            else write_word(native, code);
        }
        native_index += 1u;
        native += 24u;
    } while (signed_word(native_index) < signed_half(current_count));
    return static_cast<unsigned long long>(status);
}
