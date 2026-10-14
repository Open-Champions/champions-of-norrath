#include "../vslots-b-bytes/bytes.hpp"
// Observed setState__8Creaturei at 0x30d560, 8 bytes, weak binding.
// The low 16 bits of the argument are stored at object+572. This definition
// is a normal global; the original symbol binding was weak.
extern "C" void setState__8Creaturei(void* self, int state) {
    vslots_b::write_u16(vslots_b::address(self) + 572u, static_cast<unsigned>(state));
}
