#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__8FroglockPc at 0x00201140, 116 bytes.
// Runs Creature's msg_load on the object and the buffer, then reads two words at the returned
// count: the first goes to object+960 and the second to object+208. initAttackVariant__8Froglock runs
// after both stores. If the signed halfword at object+572 then equals 10, the word at +960 is cleared.
// Returns the count plus eight. Unknown: what initAttackVariant does with the stored values.
extern "C" int msg_load__8CreaturePc(void* self, char* data);
extern "C" void initAttackVariant__8Froglock(void* self);
extern "C" int msg_load__8FroglockPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_load__8CreaturePc(self, data);
    const Address in = address(data) + static_cast<unsigned>(count);
    write_word(address(self) + 960u, read_word(in));
    write_word(address(self) + 208u, read_word(in + 4u));
    initAttackVariant__8Froglock(self);
    if (read_i16(address(self) + 572u) == 10) write_word(address(self) + 960u, 0u);
    return count + 8;
}
