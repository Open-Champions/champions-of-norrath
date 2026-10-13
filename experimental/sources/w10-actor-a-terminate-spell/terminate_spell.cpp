// Observed terminateSpell__8CreatureRC9type_info at 0x00113890, 64 bytes.
// Looks up a running spell for a creature by type_info (checkIfSpellIsRunning, which returns the
// spell object or null). If one is found, calls the entry stored in the spell's table at
// table+8 with flag value 3. The table is the word at spell offset 20 and each entry is a
// {short this-adjust, pad, function} triple: the object pointer is adjusted by the signed
// halfword at the entry's start before the call.
// Inferred: that table entry is the destructor, and 3 means destroy-and-free. Not verified.
// Nothing here was executed on the EE.
extern "C" {
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* typeinfo);
}

struct SpellTableEntry {
    short adjust;
    short pad;
    void (*fn)(void* self, int flags);
};

extern "C" void terminateSpell__8CreatureRC9type_info(void* creature, const void* typeinfo) {
    unsigned char* spell = static_cast<unsigned char*>(checkIfSpellIsRunning__8CreatureRC9type_info(creature, typeinfo));
    if (spell) {
        const SpellTableEntry* e = reinterpret_cast<const SpellTableEntry*>(*reinterpret_cast<unsigned char* const*>(spell + 20) + 8);
        e->fn(spell + e->adjust, 3);
    }
}
