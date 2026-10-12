// Observed addSpell__8CreatureP5Spell at 0x00113948, 48 bytes. With a nonnull
// spell, links it onto the one-way list whose head is at creature+888, passing
// spell+4 as the node and the spell itself as the value. A null spell does
// nothing. No result is produced; the callee's return is not used.
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);

extern "C" void addSpell__8CreatureP5Spell(void* creature, void* spell) {
    if (spell) {
        unsigned char* bytes = static_cast<unsigned char*>(creature);
        norrath_list_oneway_link(bytes + 888, static_cast<unsigned char*>(spell) + 4, spell);
    }
}
