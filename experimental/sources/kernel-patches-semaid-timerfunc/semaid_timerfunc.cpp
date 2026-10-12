// Observed: semaid_timerfunc at 0x00446078, a 4-byte word holding -1 in the
// image. sceMcInit stores a second CreateSema result in it, and sceMcEnd
// deletes that semaphore.
// Inferred: none.
// Unknown: what the semaphore guards.
extern "C" {
int semaid_timerfunc = -1;
}
