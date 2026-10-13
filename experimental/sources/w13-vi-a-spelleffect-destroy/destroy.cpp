// Observed: Destroy__13VISpellEffect at 0x01000838 is 80 bytes. If the word at +8 of the
// object is zero it returns -1 and does nothing else. Otherwise it calls
// Destroy__16VISpellEventList three times, with this+12, this+24 and this+36, stores zero
// at +8 and returns 0.
// Inferred: +8 is an "initialised/allocated" flag, and three event lists of 12 bytes each
// sit at +12, +24 and +36; destroy is idempotent, signalling an already-destroyed object
// with -1.
// Unknown: what the three lists hold and the object's other fields.
extern "C" {
void Destroy__16VISpellEventList(void* list);

int Destroy__13VISpellEffect(unsigned char* effect) {
    if (*reinterpret_cast<int*>(effect + 8) == 0) {
        return -1;
    }
    Destroy__16VISpellEventList(effect + 12);
    Destroy__16VISpellEventList(effect + 24);
    Destroy__16VISpellEventList(effect + 36);
    *reinterpret_cast<int*>(effect + 8) = 0;
    return 0;
}
}
