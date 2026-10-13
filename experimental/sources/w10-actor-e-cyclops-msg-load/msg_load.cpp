#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__7CyclopsPc at 0x203c10, 80 bytes.
// Runs Creature's msg_load on the object and the buffer, and returns its byte count unchanged.
// Afterwards, when the signed halfword at object+572 is 10, the byte at object+40 becomes 120.
// Unknown: what 10 at +572 and the byte at +40 mean; this routine reads nothing further from the buffer.
extern "C" int msg_load__8CreaturePc(void* self, char* data);
extern "C" int msg_load__7CyclopsPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_load__8CreaturePc(self, data);
    if (read_i16(address(self) + 572u) == 10) write_u8(address(self) + 40u, 120u);
    return count;
}
