// Observed: _$_7VISetup at 0x0114f648 is 340 bytes. Arguments are (this, flags). It runs member
// destructors, each with the flag value 2, in this order: _$_10VIWndLegal(this + 0x15390),
// _$_9VIWndEula(this + 0x12d68), _$_9VIWndDnas(this + 0x12060), _$_12VIWndPatcher(this + 0xd5b0),
// _$_11VIWndSplash(this + 0xafe8), _$_15VIWindowManager(this + 0xaf64), _$_5VIPad(this + 0xae58),
// _$_7VIFader(this + 0xae38), _$_4VIUI(this + 0xade0), _$_7VIScene(this + 0x6220),
// _$_13VISoundDevice(this + 0x5098), _$_12VIDictionary(this + 0x5080), _$_10VIMatrix44 at
// this + 0x4f80, 0x4f40, 0x4f00 and 0x4ec0, _$_9VICollide(this + 0x4c60) and finally
// _$_8VIRaster(this). When bit 0 of flags is set it then tail-calls __builtin_delete(this).
// Inferred: this is the compiler-generated destructor with a delete flag, since the only
// flag use is the final delete and the member order is the reverse of construction.
// Unknown: the member types beyond the mangled owners.

extern "C" {
void _$_10VIWndLegal(void* object, int);
void _$_9VIWndEula(void* object, int);
void _$_9VIWndDnas(void* object, int);
void _$_12VIWndPatcher(void* object, int);
void _$_11VIWndSplash(void* object, int);
void _$_15VIWindowManager(void* object, int);
void _$_5VIPad(void* object, int);
void _$_7VIFader(void* object, int);
void _$_4VIUI(void* object, int);
void _$_7VIScene(void* object, int);
void _$_13VISoundDevice(void* object, int);
void _$_12VIDictionary(void* object, int);
void _$_10VIMatrix44(void* object, int);
void _$_9VICollide(void* object, int);
void _$_8VIRaster(void* object, int);
void __builtin_delete(void* object);
}

extern "C" void _$_7VISetup(void* self, int flags) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    _$_10VIWndLegal(base + 0x15390, 2);
    _$_9VIWndEula(base + 0x12d68, 2);
    _$_9VIWndDnas(base + 0x12060, 2);
    _$_12VIWndPatcher(base + 0xd5b0, 2);
    _$_11VIWndSplash(base + 0xafe8, 2);
    _$_15VIWindowManager(base + 0xaf64, 2);
    _$_5VIPad(base + 0xae58, 2);
    _$_7VIFader(base + 0xae38, 2);
    _$_4VIUI(base + 0xade0, 2);
    _$_7VIScene(base + 0x6220, 2);
    _$_13VISoundDevice(base + 0x5098, 2);
    _$_12VIDictionary(base + 0x5080, 2);
    _$_10VIMatrix44(base + 0x4f80, 2);
    _$_10VIMatrix44(base + 0x4f40, 2);
    _$_10VIMatrix44(base + 0x4f00, 2);
    _$_10VIMatrix44(base + 0x4ec0, 2);
    _$_9VICollide(base + 0x4c60, 2);
    _$_8VIRaster(self, 2);
    if ((flags & 1) != 0) {
        __builtin_delete(self);
    }
}
