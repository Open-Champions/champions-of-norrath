// Observed msg_hurt__10CloudGiantfP10GameObjecti at 0x001e9b60, 60 bytes.
// Forwards its arguments to the Creature hurt routine. Afterwards, if the word at object+976 is
// nonzero, the float at object+560 is converted to int (truncating) and stored in the global
// bossMeterCurrent. Nothing is returned.
// Inferred: +976 marks the boss fight in progress and +560 is the current health, so the boss meter
// follows it; this is not confirmed. Nothing here ran on the EE.
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int flags);
extern int bossMeterCurrent;
}

extern "C" void msg_hurt__10CloudGiantfP10GameObjecti(void* self, float amount, void* attacker, int flags) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    msg_hurt__8CreaturefP10GameObjecti(self, amount, attacker, flags);
    if (*reinterpret_cast<const int*>(object + 976) != 0) bossMeterCurrent = static_cast<int>(*reinterpret_cast<const float*>(object + 560));
}
