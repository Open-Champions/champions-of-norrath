#include "../w9-animation-object-c-common/mem.hpp"
// Observed setAnimEventFlags__8Creatureififififi at 0x001128a8, 88 bytes. The creature
// word at +36 is a table of eight-byte entries; entry `index` holds a record pointer in
// its second word (+4). In that record the function writes four pairs, each a float
// followed by a byte: float +20 / byte +24, float +28 / byte +32, float +36 / byte +40
// and float +44 / byte +48. The arguments after the index alternate float, int, float,
// int, float, int, float, int in that order (the byte takes the low 8 bits of the int).
// The table pointer and the entry are re-read from the creature before every pair, as in
// the original; nothing else is written.
// Inferred: the record is a per-animation event entry and the pairs are event times with
// a flag each (the name says "anim event flags"); no field names are claimed.
// Unknown: the record's declared type and whether the table word can change between pairs.
extern "C" void setAnimEventFlags__8Creatureififififi(unsigned char* self, int index,
                                                      float f0, int b0, float f1, int b1,
                                                      float f2, int b2, float f3, int b3) {
    using namespace norrath_aoc;
    const unsigned slot = static_cast<unsigned>(index) << 3;
    unsigned char* record = load_pointer(load_pointer(self + 36) + slot + 4);
    *(record + 24) = static_cast<unsigned char>(b0);
    store_float(record + 20, f0);
    record = load_pointer(load_pointer(self + 36) + slot + 4);
    *(record + 32) = static_cast<unsigned char>(b1);
    store_float(record + 28, f1);
    record = load_pointer(load_pointer(self + 36) + slot + 4);
    *(record + 40) = static_cast<unsigned char>(b2);
    store_float(record + 36, f2);
    record = load_pointer(load_pointer(self + 36) + slot + 4);
    *(record + 48) = static_cast<unsigned char>(b3);
    store_float(record + 44, f3);
}
