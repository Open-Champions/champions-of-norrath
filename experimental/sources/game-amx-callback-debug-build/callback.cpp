#include "init.hpp"
extern "C" unsigned long long norrath_amx_callback(
    void* amx, int index, int* result, void* parameters) {
    using namespace norrath_constructor_detail;
    const Address state = address(amx);
    const Address output = address(result);
    const Address header = read_word(state);
    const unsigned table_offset = read_word(header + 44u);
    const unsigned record_offset = static_cast<unsigned>(index) * 24u;
    const Address target = read_word(header + table_offset + record_offset);
    // Real native provider boundary: the selected target supplies the result.
    // Only its word result and first two argument lanes are reconstructed here.
    using Native = unsigned (*)(void*, void*);
    const Native native = reinterpret_cast<Native>(static_cast<__UINTPTR_TYPE__>(target));
    write_word(state + 92u, 0u);
    const unsigned native_result = native(amx, parameters);
    write_word(output, native_result);
    // Reload after the output store: output may alias the AMX error word.
    const int error = signed_word(read_word(state + 92u));
    return static_cast<unsigned long long>(static_cast<long long>(error));
}
