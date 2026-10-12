#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed saveActivate__9Savepoint at 0x21c318, 12 bytes. The word at object+204
// becomes 1. Nothing else is read or written and no value is returned.
// Unknown: the meaning of word 204. The existing caller in the HUD message code
// declares this symbol with an unsigned argument; the image's body treats its
// first argument as the object address.
extern "C" void saveActivate__9Savepoint(void* self) {
    w8c::write_word(w8c::address(self) + 204u, 1u);
}
