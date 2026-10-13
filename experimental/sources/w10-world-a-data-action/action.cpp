// Observed action at 0x00486a18 (.sbss), 4 bytes. Reached through gp. Seventy-six word loads and one word store use it; the loaded value is used as a record base by callers.
// Inferred: A pointer to the action record being run (the neighbouring executeAction takes an Action*).
// Unknown: The pointee type and the writers. Authored callers declare it as a pointer or an integer; the storage is one pointer-sized word either way.
extern "C" {
void* action;
}
