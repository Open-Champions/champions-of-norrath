// Observed: Save__13VIGameOptions at 0x01019a38 is 36 bytes. With (this) it tail-calls
// VIMemCardWriteFile__FPCcPCvi("game.opt", this, 1024) and returns that call's result unchanged.
// Inferred: the game options object is 1024 bytes of saved state written to the memory card under the
// name "game.opt"; the result's sign is what the options window tests (SaveAndExit).
// Unknown: the meaning of the result beyond being an int, and the field layout of the options.
extern "C" {
int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size);

int Save__13VIGameOptions(void* self) {
    return VIMemCardWriteFile__FPCcPCvi("game.opt", self, 1024);
}
}
