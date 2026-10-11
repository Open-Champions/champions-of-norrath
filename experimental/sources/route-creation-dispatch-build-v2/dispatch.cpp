#include "dispatch.hpp"
namespace {
using Word = unsigned;
Word address(const void* p) { return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
unsigned char* bytes(Word a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
Word read_word(Word a) {
    return Word(bytes(a)[0]) | (Word(bytes(a)[1]) << 8) |
           (Word(bytes(a)[2]) << 16) | (Word(bytes(a)[3]) << 24);
}
void write_word(Word a, Word value) {
    bytes(a)[0] = static_cast<unsigned char>(value);
    bytes(a)[1] = static_cast<unsigned char>(value >> 8);
    bytes(a)[2] = static_cast<unsigned char>(value >> 16);
    bytes(a)[3] = static_cast<unsigned char>(value >> 24);
}
}
extern "C" unsigned char* norrath_object_create_by_name(char* name, const unsigned char* point, int type_word, char** arguments) {
    const Word function_word = read_word(address(norrath_create_by_name_function_word));
    const Word point_address = address(point);
    const Word x_word = read_word(point_address);
    const Word y_word = read_word(point_address + 4u);
    const Word z_word = read_word(point_address + 8u);
    NorrathPoint3 snapshot;
    unsigned char* const snapshot_bytes = reinterpret_cast<unsigned char*>(&snapshot);
    for (unsigned i = 0; i != 4u; ++i) {
        snapshot_bytes[i] = static_cast<unsigned char>(x_word >> (8u * i));
        snapshot_bytes[4u + i] = static_cast<unsigned char>(y_word >> (8u * i));
        snapshot_bytes[8u + i] = static_cast<unsigned char>(z_word >> (8u * i));
    }
    const unsigned low_type = static_cast<unsigned>(type_word) & 65535u;
    const int signed_type = low_type >= 32768u ? static_cast<int>(low_type) - 65536 : static_cast<int>(low_type);
    unsigned char* object = nullptr;
    if (function_word != 0u) {
        const auto function = reinterpret_cast<NorrathCreateByName>(static_cast<__UINTPTR_TYPE__>(function_word));
        object = function(name, snapshot, static_cast<short>(signed_type), arguments);
    }
    if (object != nullptr) write_word(address(object) + 52u, address(name));
    return object;
}
