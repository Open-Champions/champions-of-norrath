// Observed setOnFire__6PhPropi at 0x00154ba8, 8 bytes: stores the low byte of the argument at object+570
// and nothing else. Other authored code calls it with 1 (w10-actor-b autocontainer, w10-actor-c catapult).
// Unknown: what the byte at +570 means beyond the call sites' "on" value; whether other readers treat it
// as a flag or a counter. Nothing here ran on the EE.
extern "C" void setOnFire__6PhPropi(void* self, int value) {
    *(static_cast<unsigned char*>(self) + 570) = static_cast<unsigned char>(value);
}
