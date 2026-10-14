// Observed: semaidRegFunc at 0x00446074, a 4-byte word holding -1 in the image.
// sceMcInit stores a CreateSema result in it and waits on it; sceMcClose polls
// and signals it; sceMcEnd deletes that semaphore and stores -1 back.
// Inferred: none.
// Unknown: none.
extern "C" {
int semaidRegFunc = -1;
}
