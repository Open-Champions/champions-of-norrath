// Observed action at 0x00486a18 (.sbss), 4 bytes. Reached through gp. Seventy-six word loads and one word store use it; the loaded value is used as a record base by callers.
// Inferred: the base address of a table of 20-byte action records, indexed by the cut-scene step number (advanceFrame reads the float at the start of action[displayActionPos]; it does not point at a single current record).
// Unknown: The pointee type and the writers. Authored callers declare it as a pointer or an integer; the storage is one pointer-sized word either way.
extern "C" {
void* action;
}
