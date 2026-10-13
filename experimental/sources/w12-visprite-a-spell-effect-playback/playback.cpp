// Observed: Playback__19VISpellEffectSystemi at 0x010030c0, 72 bytes. With the
// object as this and id the second argument: slot = (id & 0xff) % 24 (the
// remainder of the unsigned low byte); r = the word at this + 4 + 4 * slot. If
// r is zero the result is 0. Otherwise the result is r when the word at r + 28
// equals the full int id and 0 when it does not. Only one record per slot is
// examined; there is no chain walk.
// Inferred: this + 4 is a 24-entry lookup table of playback records keyed by
// the low byte of the id, with the id stored at record + 28 (Stop and Process
// use the same table).
// Unknown: the playback record type name.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
char* Playback__19VISpellEffectSystemi(void* self, int id) {
    unsigned slot = (static_cast<unsigned>(id) & 0xff) % 24;
    unsigned record = *reinterpret_cast<unsigned*>(static_cast<char*>(self) + 4 + 4 * slot);
    if (record == 0) return nullptr;
    char* p = NORRATH_TRANSLATE(record);
    return *reinterpret_cast<int*>(p + 28) == id ? p : nullptr;
}
}
