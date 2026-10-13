// Observed actor at 0x00486a0c (.sbss), 4 bytes. Reached through gp. Thirty-one word loads and one word store use it; the loaded value is used as a record base by callers.
// Inferred: A pointer to the actor record of the current cut-scene step.
// Unknown: The pointee type and the writers.
extern "C" {
void* actor;
}
