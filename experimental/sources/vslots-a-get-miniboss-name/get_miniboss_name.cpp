// Observed getMinibossName__8Creature at 0x00112950, 12 bytes.
// Returns the address of the bytes "Pablo The Great" at 0x00460750.
// The creature argument is not read or written.
extern "C" const char* getMinibossName__8Creature(void* self) {
    (void)self;
    return "Pablo The Great";
}
