#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_save__7ShooterPc at 0x1e5d80, 24 bytes. The unsigned halfword at
// object+82 is stored at the output pointer and the return value is the number of
// bytes written, 2. Unknown: the meaning of that halfword.
extern "C" int msg_save__7ShooterPc(void* self, char* out) {
    using namespace w8c;
    write_u16(address(out), read_u16(address(self) + 82u));
    return 2;
}
