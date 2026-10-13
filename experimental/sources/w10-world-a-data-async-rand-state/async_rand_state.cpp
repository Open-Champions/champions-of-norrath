// Observed asyncRandState at 0x00483e60 (.data), 4 bytes. Four bytes in .data (nm size 4), image word zero. Word loads and stores only.
// Inferred: The state word of a random generator that is independent of the main one (from the name only).
// Unknown: The generator algorithm is not read here.
extern "C" {
int asyncRandState;
}
