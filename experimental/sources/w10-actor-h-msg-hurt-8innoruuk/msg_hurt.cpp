// Observed msg_hurt__8InnoruukfP10GameObjecti at 0x001ff568, 64 bytes.
// When the word at object+984 is zero, calls the Creature hurt routine with the object and the
// three incoming arguments unchanged, then stores the float at object+560 converted to an integer
// (the compiler's float-to-int conversion) in the boss-meter value bossMeterCurrent. When the word
// is nonzero, nothing happens. No result is returned.
// Unknown: what the word at +984 means; bossMeterCurrent is named from its symbol only.
// Nothing here ran on the EE.
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int kind);
extern int bossMeterCurrent;
}

extern "C" void msg_hurt__8InnoruukfP10GameObjecti(void* self, float amount, void* attacker, int kind) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<unsigned*>(object + 984) != 0) return;
    msg_hurt__8CreaturefP10GameObjecti(self, amount, attacker, kind);
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<float*>(object + 560));
}
