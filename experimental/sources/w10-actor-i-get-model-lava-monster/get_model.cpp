// Observed getModel__11LavaMonster at 0x001ee280, 32 bytes.
// The word at object+36 points to a table. When the word at object+1048 is zero it returns the
// word at table+12, otherwise the word at table+28. The same body is also emitted under the name
// norrath_lava_monster_get_model by an earlier unit; the class's table refers to this original name.
// Inferred: +1048 selects between two model slots of the resource table. Unknown: what each slot
// names. Nothing here ran on the EE.
extern "C" void* getModel__11LavaMonster(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* table = *reinterpret_cast<unsigned char**>(object + 36);
    if (*reinterpret_cast<unsigned*>(object + 1048) == 0u) return *reinterpret_cast<void**>(table + 12);
    return *reinterpret_cast<void**>(table + 28);
}
