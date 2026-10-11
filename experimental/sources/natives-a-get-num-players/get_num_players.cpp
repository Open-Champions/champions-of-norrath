// amxGetNumPlayers is the 8-byte native at 0x0013b030. It returns the word
// nmPlayers at 0x00484538 and does not read the AMX arguments.
extern "C" int nmPlayers;
extern "C" int amxGetNumPlayers__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    return nmPlayers;
}
