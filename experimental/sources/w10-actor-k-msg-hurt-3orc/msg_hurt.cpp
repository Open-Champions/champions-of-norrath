// Observed msg_hurt__3OrcfP10GameObjecti at 0x0022b550, 28 bytes. Forwards the object, the
// damage amount, the attacker and the integer unchanged to the Creature handler and returns.
// Inferred: Orc adds nothing of its own to being hurt. Nothing here ran on the EE.
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int extra);

void msg_hurt__3OrcfP10GameObjecti(void* self, float amount, void* attacker, int extra) {
    msg_hurt__8CreaturefP10GameObjecti(self, amount, attacker, extra);
}
}
