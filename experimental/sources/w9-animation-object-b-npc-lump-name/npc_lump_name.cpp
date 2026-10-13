// Observed npcLumpName at 0x486d84 in .bss, 4 bytes (the next symbol, firstTime,
// begins at 0x486d88). The select-screen initializer stores a string address in it
// ("select.lmp" or "select2.lmp") and later passes it as a lump name to the lump
// resource lookup and the NPC constructor. Zero before that store.
// Unknown: other writers.
extern "C" {
const char* npcLumpName;
}
