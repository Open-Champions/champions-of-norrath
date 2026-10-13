// Observed actor at 0x00486a0c (.sbss), 4 bytes. Reached through gp. Thirty-one word loads and one word store use it; the loaded value is used as a record base by callers.
// Inferred: the base address of a table of 44-byte records; executeAction reads the first word of record `type` as an object address, and advanceFrame walks the first nmActors records.
// Unknown: The pointee type and the writers.
extern "C" {
void* actor;
}
