// Observed msg_hurt__6MinionfP10GameObjecti at 0x002353b0, 112 bytes.
// Passes all arguments on to msg_hurt__8Creature. Afterwards copies the 14 bytes "GreaterMinion"
// plus its terminating zero from the string at 0x472540 into the boss-meter name buffer
// bossMeterName, and stores the single at object+560 truncated to an integer in bossMeterCurrent.
// Inferred: the hurt message of a minion updates the boss meter (name, then current value) from
// the creature's float at +560. Unknown: what that float is. Nothing here ran on the EE.
extern "C" {
extern char bossMeterName[];
extern int bossMeterCurrent;
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int kind);
}

extern "C" void msg_hurt__6MinionfP10GameObjecti(void* self, float amount, void* attacker, int kind) {
    msg_hurt__8CreaturefP10GameObjecti(self, amount, attacker, kind);
    static const char name[14] = {'G', 'r', 'e', 'a', 't', 'e', 'r', 'M', 'i', 'n', 'i', 'o', 'n', 0};
    for (int i = 0; i < 14; ++i) bossMeterName[i] = name[i];
    bossMeterCurrent = static_cast<int>(*reinterpret_cast<float*>(static_cast<unsigned char*>(self) + 560));
}
