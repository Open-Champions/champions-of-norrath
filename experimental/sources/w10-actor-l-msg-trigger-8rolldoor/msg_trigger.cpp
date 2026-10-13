#include "../w10-actor-l-common/access.hpp"
// Observed msg_trigger__8RollDoori at 0x0021c658, 52 bytes. The incoming integer is ignored. The
// table whose address is the word at object+72 has an entry at table offset 112, laid out as a
// signed halfword adjustment followed by a function word at +4. That function is called with
// the object advanced by the adjustment and one argument, zero; its result is returned. Unknown:
// which virtual routine that entry is. Nothing here ran on the EE.
extern "C" int msg_trigger__8RollDoori(void* self, int) {
    return static_cast<int>(w10l::call_entry(static_cast<unsigned char*>(self), 112, 0));
}
