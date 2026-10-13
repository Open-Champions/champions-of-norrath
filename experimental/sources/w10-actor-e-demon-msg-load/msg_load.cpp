#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__5DemonPc at 0x1eb400, 88 bytes.
// Runs Creature's msg_load on the object and the buffer. The word that follows the
// returned count is stored at object+208, then initAttackVariant__5Demon runs on the
// object. The routine returns the count plus four.
// Unknown: what initAttackVariant does with the stored word.
extern "C" int msg_load__8CreaturePc(void* self, char* data);
extern "C" void initAttackVariant__5Demon(void* self);
extern "C" int msg_load__5DemonPc(void* self, char* data) {
    using namespace vslots_b;
    const int count = msg_load__8CreaturePc(self, data);
    write_word(address(self) + 208u, read_word(address(data) + static_cast<unsigned>(count)));
    initAttackVariant__5Demon(self);
    return count + 4;
}
