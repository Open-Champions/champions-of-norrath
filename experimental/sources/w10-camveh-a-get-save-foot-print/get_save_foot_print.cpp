// Observed GetSaveFootPrint__6Camera at 0x0026fc90, 28 bytes. Calls
// Save__6CameraPc with the camera and a 16384-byte buffer local to this
// frame, and returns whatever that call returns (the register is not touched
// afterwards). Presumably the result is the number of bytes Save wrote, but
// that is inferred from the name and is not established. Nothing here was
// executed on the EE.
extern "C" int Save__6CameraPc(void* camera, char* buffer);

extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    char buffer[16384];
    return Save__6CameraPc(camera, buffer);
}
