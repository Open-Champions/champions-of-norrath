// Observed: Free__7VISetup at 0x0114fb68 is 220 bytes. With the object as this it runs, in order:
// StopAmbientSoundtrack__7VISetup(this), Free__7VIFader(this + 0xae38),
// Free__9VIWndEula(this + 0x12d68), Free__10VIWndLegal(this + 0x15390),
// Free__9VIWndDnas(this + 0x12060), Free__12VIWndPatcher(this + 0xd5b0),
// Free__11VIWndSplash(this + 0xafe8), VISplashFxFree__Fv(), Close__4VIUI(this + 0xade0),
// Clear__7VIScene(this + 0x6220), Close__13VISoundDevice(this + 0x5098),
// Clear__12VIDictionary(this + 0x5080), Close__9VICollide(this + 0x4c60),
// Close__8VIRaster(this) and Close__5VIPad(this + 0xae58). Nothing is returned.
// Inferred: the offsets locate member sub-objects of the types in the mangled owners; the call
// order is a teardown of what Init__7VISetupi builds.
// Unknown: the member types beyond those names, and any state the callees keep.

extern "C" {
void StopAmbientSoundtrack__7VISetup(void* object);
void Free__7VIFader(void* object);
void Free__9VIWndEula(void* object);
void Free__10VIWndLegal(void* object);
void Free__9VIWndDnas(void* object);
void Free__12VIWndPatcher(void* object);
void Free__11VIWndSplash(void* object);
void VISplashFxFree__Fv();
void Close__4VIUI(void* object);
void Clear__7VIScene(void* object);
void Close__13VISoundDevice(void* object);
void Clear__12VIDictionary(void* object);
void Close__9VICollide(void* object);
void Close__8VIRaster(void* object);
void Close__5VIPad(void* object);
}

extern "C" void Free__7VISetup(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    StopAmbientSoundtrack__7VISetup(self);
    Free__7VIFader(base + 0xae38);
    Free__9VIWndEula(base + 0x12d68);
    Free__10VIWndLegal(base + 0x15390);
    Free__9VIWndDnas(base + 0x12060);
    Free__12VIWndPatcher(base + 0xd5b0);
    Free__11VIWndSplash(base + 0xafe8);
    VISplashFxFree__Fv();
    Close__4VIUI(base + 0xade0);
    Clear__7VIScene(base + 0x6220);
    Close__13VISoundDevice(base + 0x5098);
    Clear__12VIDictionary(base + 0x5080);
    Close__9VICollide(base + 0x4c60);
    Close__8VIRaster(self);
    Close__5VIPad(base + 0xae58);
}
