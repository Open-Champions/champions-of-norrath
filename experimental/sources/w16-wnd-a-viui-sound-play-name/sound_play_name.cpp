// Observed: SoundPlay__4VIUIPCcf at 0x0102fb48 is 120 bytes. Arguments: the UI object, a sound name and a volume
// float. It calls VIHashResourceID(name), then Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, hash, &type,
// &handle) where dictionary is the word at ui+80 and type and handle are two stack words. When Find returns nonzero
// it calls Play__13VISoundDeviceii12VISOUNDCLASS(device, handle, 127, 0) with device the word at ui+76; when that
// result is not negative it calls SetVolume__13VISoundDeviceif(device, result, volume). It returns zero in every case.
// Inferred: starts the named sound at full level (127) and then sets the voice volume to the caller's value.
// Unknown: the meaning of the last Play argument and of the type word, and whether the return type is int.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
unsigned VIHashResourceID(const char* name);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int Play__13VISoundDeviceii12VISOUNDCLASS(void* device, int handle, int level, int soundClass);
void SetVolume__13VISoundDeviceif(void* device, int voice, float volume);

int SoundPlay__4VIUIPCcf(void* ui, const char* name, float volume) {
    int type;
    int handle;
    const unsigned hash = VIHashResourceID(name);
    unsigned char* bytes = static_cast<unsigned char*>(ui);
    void* dictionary = reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(bytes + 80)));
    if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, hash, &type, &handle) != 0) {
        void* device = reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(bytes + 76)));
        const int voice = Play__13VISoundDeviceii12VISOUNDCLASS(device, handle, 127, 0);
        if (voice >= 0) SetVolume__13VISoundDeviceif(device, voice, volume);
    }
    return 0;
}
}
