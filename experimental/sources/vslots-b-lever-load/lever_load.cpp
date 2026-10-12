#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_load__5LeverPc at 0x21c9e0, 112 bytes.
// Ten buffer bytes are consumed. The first word is stored at object+76, the
// next word at object+12, and the next two bytes at object+80 and object+81.
// A nonzero byte at object+81 then calls the existing objectAddToRunList body.
// The return is 10.
extern "C" unsigned norrath_actor_add_run_list(void* object);
extern "C" int msg_load__5LeverPc(void* self, void* buffer) {
    using namespace vslots_b;
    const Address source = address(buffer);
    const Address base = address(self);
    write_word(base + 76u, read_word(source));
    write_word(base + 12u, read_word(source + 4u));
    write_u8(base + 80u, read_u8(source + 8u));
    const unsigned running = read_u8(source + 9u);
    write_u8(base + 81u, running);
    if (running != 0u) norrath_actor_add_run_list(self);
    return 10;
}
