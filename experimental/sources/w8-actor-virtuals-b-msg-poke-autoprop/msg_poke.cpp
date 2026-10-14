// Observed msg_poke__8AutoPropi at 0x00224e78, 76 bytes. The int argument is not read.
// Does nothing unless bit 4 (value 0x10) of the flag word at object+284 is set and the
// word at object+288 is nonzero. Then it calls the table entry at byte offset 136 of
// the table whose address is the word at object+72, with the object address adjusted
// by that entry's 16-bit adjustment. In the AutoProp family tables that entry holds
// advanceState__8AutoProp (observed in the GothTrap table, slot 17).
// Unknown: what the two fields mean.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" void msg_poke__8AutoPropi(void* self, int value) {
    (void)value;
    const unsigned char* object = static_cast<const unsigned char*>(self);
    if ((*reinterpret_cast<const w8avb::UWord*>(object + 284) & 0x10) == 0) return;
    if (*reinterpret_cast<const w8avb::UWord*>(object + 288) == 0) return;
    w8avb::call_slot(self, 136);
}
