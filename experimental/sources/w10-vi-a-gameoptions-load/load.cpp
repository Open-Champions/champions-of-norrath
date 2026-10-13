// Observed: Load__13VIGameOptions at 0x01019990 is 168 bytes. It clears 1024
// bytes at this with memset, then calls VIMemCardReadFile__FPCcPvi with the
// name "game.opt", this and 1024.
// - A negative read result: SetDefaults__13VIGameOptions(this); the read result
//   is returned.
// - A result from 0 to 1023: SetDefaults; returns -1.
// - A result of 1024 or more: if the first word is nonzero, or the word at
//   offset 1020 is not 0x01234567, SetDefaults is called. Returns 0 either way.
// Inferred: the first word is an error or "invalid" marker and the last word a
// signature, from how they gate defaults. Unknown: the layout in between.
extern "C" {
void* memset(void* destination, int value, unsigned count);
int VIMemCardReadFile__FPCcPvi(const char* name, void* buffer, int size);
void SetDefaults__13VIGameOptions(void* object);

int Load__13VIGameOptions(void* object) {
    memset(object, 0, 1024);
    int result = VIMemCardReadFile__FPCcPvi("game.opt", object, 1024);
    if (result < 0) {
        SetDefaults__13VIGameOptions(object);
        return result;
    }
    if (result < 1024) {
        SetDefaults__13VIGameOptions(object);
        return -1;
    }
    const int* words = static_cast<const int*>(object);
    if (words[0] != 0 || words[255] != 0x01234567) SetDefaults__13VIGameOptions(object);
    return 0;
}
}
