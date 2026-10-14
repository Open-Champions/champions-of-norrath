// Observed msg_collision__6PlayerP10GameObject at 0x001ab988, 196 bytes. self is the player, other the
// object it touched. Acts only when bit 17 (0x20000) of the flag word at self+12 is clear and the word at
// self+1400 equals 9. Then:
//   spell = checkIfSpellIsRunning__8CreatureRC9type_info(self, __tf11SpellCharge());
//   target = other when (other's flag word at +12 & 0x1000220) == 0x20 and the float at other+24 is within
//   60.0 of the float at self+24 (absolute difference strictly below), else target = null;
//   hitSomething__11SpellChargeP10GameObject(spell, target).
// Nothing is returned. The call to hitSomething happens whether or not the target test passes (with a
// null second argument when it fails), and the checkIfSpellIsRunning result is passed on unchecked.
// Inferred: 9 is the player state in which a charged spell is being held (the callee names). Unknown:
// what the +24 floats are (a height or depth is likely). Nothing here ran on the EE.
extern "C" {
const void* __tf11SpellCharge();
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* self, const void* type);
void hitSomething__11SpellChargeP10GameObject(void* spell, void* other);

void msg_collision__6PlayerP10GameObject(void* self, void* other) {
    unsigned char* s = static_cast<unsigned char*>(self);
    unsigned char* o = static_cast<unsigned char*>(other);
    if (*reinterpret_cast<unsigned*>(s + 12) & 0x20000u) return;
    if (*reinterpret_cast<int*>(s + 1400) != 9) return;
    void* spell = checkIfSpellIsRunning__8CreatureRC9type_info(self, __tf11SpellCharge());
    void* target = nullptr;
    if ((*reinterpret_cast<unsigned*>(o + 12) & 0x1000220u) == 0x20u) {
        float d = *reinterpret_cast<float*>(o + 24) - *reinterpret_cast<float*>(s + 24);
        if (__builtin_fabsf(d) < 60.0f) target = other;
    }
    hitSomething__11SpellChargeP10GameObject(spell, target);
}
}
