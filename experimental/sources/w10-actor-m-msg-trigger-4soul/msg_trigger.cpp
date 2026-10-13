#include "../w10-actor-m-common/access.hpp"
// Observed msg_trigger__4Souli at 0x1f4ee8, 80 bytes. Only argument 0 does anything: the signed byte at
// object+570 is saved and cleared, the table entry at byte 184 of the object's table (word at +72) is
// called on the adjusted object with the argument 10, and the saved byte is stored back. Any other
// trigger value returns at once. Nothing is returned. Unknown: what the entry at 184 is and what the
// byte at +570 means.
extern "C" void msg_trigger__4Souli(void* self, int trigger) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (trigger != 0) return;
    const unsigned char saved = o[570];
    o[570] = 0;
    w10m::call_entry(o, 184, 10);
    o[570] = saved;
}
