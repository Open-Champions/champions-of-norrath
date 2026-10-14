// Observed: Free__9VIStation at 0x01017eb8 is 212 bytes. With the object as this it runs, in order:
// Free__9VIWndEula(this + 0x15408), Close__4VIUI(this + 0xb3f0), printf("qaa54\n"),
// Clear__7VIScene(this + 0x6830), printf("qaa\n"), Close__13VISoundDevice(this + 0x56a8),
// printf("qaa1\n"), Clear__12VIDictionary(this + 0x5690), Close__9VICollide(this + 0x5270),
// Free__7VIFader(this + 0xb448), printf("qav\n"), Close__8VIRaster(this + 0x610),
// CleanUp__11VIGUIObject() with no argument, Free__15VIWindowManager(this + 0xb798) and
// Free__10VIKeyboard(this + 0xb4d0). Nothing is returned. The printf strings are at
// 0x011c3c00, 0x011c3c08, 0x011c3c10 and 0x011c3c18 respectively.
// Inferred: the printf strings are progress markers of a teardown; CleanUp__11VIGUIObject is
// taken to be a static member because no argument register is set for it.
// Unknown: the member types beyond the mangled owners, and what the marker strings abbreviate.

extern "C" {
void Free__9VIWndEula(void* object);
void Close__4VIUI(void* object);
int printf(const char* format, ...);
void Clear__7VIScene(void* object);
void Close__13VISoundDevice(void* object);
void Clear__12VIDictionary(void* object);
void Close__9VICollide(void* object);
void Free__7VIFader(void* object);
void Close__8VIRaster(void* object);
void CleanUp__11VIGUIObject();
void Free__15VIWindowManager(void* object);
void Free__10VIKeyboard(void* object);
}

extern "C" void Free__9VIStation(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    Free__9VIWndEula(base + 0x15408);
    Close__4VIUI(base + 0xb3f0);
    printf("qaa54\n");
    Clear__7VIScene(base + 0x6830);
    printf("qaa\n");
    Close__13VISoundDevice(base + 0x56a8);
    printf("qaa1\n");
    Clear__12VIDictionary(base + 0x5690);
    Close__9VICollide(base + 0x5270);
    Free__7VIFader(base + 0xb448);
    printf("qav\n");
    Close__8VIRaster(base + 0x610);
    CleanUp__11VIGUIObject();
    Free__15VIWindowManager(base + 0xb798);
    Free__10VIKeyboard(base + 0xb4d0);
}
