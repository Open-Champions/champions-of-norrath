// Observed: _$_9VIStation at 0x01017898 is 300 bytes. Arguments are (this, flags). It runs member
// destructors, each with the flag value 2, in this order: _$_9VIWndEula(this + 0x15408),
// _$_20VIWndGenericRenderer(this + 0xb820), _$_15VIWindowManager(this + 0xb798),
// _$_10VIKeyboard(this + 0xb4d0), _$_5VIPad(this + 0xb470), _$_7VIFader(this + 0xb448),
// _$_4VIUI(this + 0xb3f0), _$_7VIScene(this + 0x6830), _$_13VISoundDevice(this + 0x56a8),
// _$_12VIDictionary(this + 0x5690), _$_10VIMatrix44 at this + 0x5590, 0x5550, 0x5510 and 0x54d0,
// _$_9VICollide(this + 0x5270) and finally _$_8VIRaster(this + 0x610). When bit 0 of flags is set
// it then tail-calls __builtin_delete(this).
// Inferred: this is the compiler-generated destructor with a delete flag, as for VISetup.
// Unknown: the member types beyond the mangled owners.

extern "C" {
void _$_9VIWndEula(void* object, int);
void _$_20VIWndGenericRenderer(void* object, int);
void _$_15VIWindowManager(void* object, int);
void _$_10VIKeyboard(void* object, int);
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

extern "C" void _$_9VIStation(void* self, int flags) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    _$_9VIWndEula(base + 0x15408, 2);
    _$_20VIWndGenericRenderer(base + 0xb820, 2);
    _$_15VIWindowManager(base + 0xb798, 2);
    _$_10VIKeyboard(base + 0xb4d0, 2);
    _$_5VIPad(base + 0xb470, 2);
    _$_7VIFader(base + 0xb448, 2);
    _$_4VIUI(base + 0xb3f0, 2);
    _$_7VIScene(base + 0x6830, 2);
    _$_13VISoundDevice(base + 0x56a8, 2);
    _$_12VIDictionary(base + 0x5690, 2);
    _$_10VIMatrix44(base + 0x5590, 2);
    _$_10VIMatrix44(base + 0x5550, 2);
    _$_10VIMatrix44(base + 0x5510, 2);
    _$_10VIMatrix44(base + 0x54d0, 2);
    _$_9VICollide(base + 0x5270, 2);
    _$_8VIRaster(base + 0x610, 2);
    if ((flags & 1) != 0) {
        __builtin_delete(self);
    }
}
