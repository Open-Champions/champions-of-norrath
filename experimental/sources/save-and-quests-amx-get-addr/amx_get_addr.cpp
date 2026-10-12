#include "../save-and-quests-bytes/bytes.hpp"
// Observed amx_GetAddr at 0x138758, 76 bytes. The object's first word is the
// base of the script image, and the word at base+16 is the data offset. The
// signed words at object+20, +24 and +28 bound the address: a value in
// [word20, word24) or at or above word28 returns 5 and writes nothing.
// Otherwise the result is base + data offset + address, stored through the
// third argument, and 0 is returned. The compares are signed, so a negative
// address is accepted.
extern "C" int amx_GetAddr(void* amx, int address, int** physical) {
    using namespace sq;
    const unsigned char* object = static_cast<const unsigned char*>(amx);
    const int low = signed_word(read_word(object + 20));
    const int mid = signed_word(read_word(object + 24));
    const int high = signed_word(read_word(object + 28));
    const unsigned char* base = static_cast<const unsigned char*>(pointer(read_word(object)));
    if (address >= low && address < mid) return 5;
    if (address >= high) return 5;
    const unsigned data = read_word(base + 16);
    const unsigned char* result = base + (data + static_cast<unsigned>(address));
    write_word(physical, pointer_bits(result));
    return 0;
}
