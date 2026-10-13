// Observed commanderValid__3Orc at 0x0022b930, 44 bytes. The word at object+1128 is a pointer to a
// commander record (zero means none). Returns 0 when it is zero. Otherwise the record's word at +12
// points at another object; the result is 1 when bit 3 (value 8) of that object's word at +12 is clear,
// else 0. What the record and the bit mean is unknown. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" int commanderValid__3Orc(void* self) {
    using namespace w11a;
    unsigned char* o = static_cast<unsigned char*>(self);
    const unsigned record = word(o, 1128);
    if (record == 0) return 0;
    unsigned char* target = from_word(word(from_word(record), 12));
    return (word(target, 12) & 8) == 0 ? 1 : 0;
}
