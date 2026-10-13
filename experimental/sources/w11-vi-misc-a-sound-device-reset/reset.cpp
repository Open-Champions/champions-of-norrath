// Observed: Reset__13VISoundDevice at 0x011248a8 is 664 bytes. It returns 0. Offsets
// are device bytes. It prints nine short marker lines ("qaw!\n", "qaw!1\n" ... "qaw!9\n")
// between the stages below, which are in this order:
//  1. printf marker 0; if the pointer at 3420 is nonzero it is passed to
//     __builtin_delete and cleared; marker 1.
//  2. For each playback from FirstPlayback__13VISoundDevice(this) onwards, the next
//     playback is fetched first (NextPlayback__13VISoundDeviceP10VIPlayback), then
//     sound = Sound__13VISoundDevicei(this, playback word 7); if
//     Type__C7VISound(sound) is 1, Stop__13VISoundDevicei(this, playback word 8).
//  3. Marker 2; StopAllBgm__13VISoundDevice; marker 3.
//  4. For the 128 words at 2584 in ascending order: a nonzero entry is an object whose
//     word at 12 points to a table; the table entry at +8 holds a halfword offset and a
//     function pointer at +4 of the entry. The function is called with (object + offset,
//     3), which reads as a virtual destructor call with the delete flags.
//  5. Marker 4; if the word at 4472 is nonzero it calls BgmBufferSize__13VISoundDevice,
//     then FreeSpuMem__13VISoundDeviceUii(this, word 4472, 2 * that size) and clears
//     the word.
//  6. Markers 5 and 6; Init__7VILList on the lists at 3284 and 3296; zero the 128 words
//     at 2584..3095 and the 46 words at 3100..3283; append the 46 nodes of 56 bytes
//     from offset 8 to the list at 3296 with AddTail__7VILListP7VILNode.
//  7. Marker 7; stores like Init__13VISoundDevice: 64-bit zero at 0; zero words at
//     3096, 3384, 3408, 3404, 4472, 4460, 4464 (before marker 8); word 3432 = 1;
//     word 3412 = 1; bytes 3436, 3692, 3948, 4204 = 0; float 1.0 at 4468; marker 8;
//     floats at 3424 and 3428 = 1.0; marker 9.
// Inferred: this is the "reset to initial state" path; Init__13VISoundDevice sets most
// of the same fields. Unknown: the playback record layout beyond words 7 and 8.
extern "C" {
int printf(const char* format, ...);
void __builtin_delete(void* pointer);
void* FirstPlayback__13VISoundDevice(void* device);
void* NextPlayback__13VISoundDeviceP10VIPlayback(void* device, void* playback);
void* Sound__13VISoundDevicei(void* device, int index);
int Type__C7VISound(const void* sound);
void Stop__13VISoundDevicei(void* device, int index);
void StopAllBgm__13VISoundDevice(void* device);
int BgmBufferSize__13VISoundDevice(void* device);
void FreeSpuMem__13VISoundDeviceUii(void* device, unsigned address, int size);
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

int Reset__13VISoundDevice(void* device) {
    char* base = static_cast<char*>(device);
    printf("qaw!\n");
    void** owned = reinterpret_cast<void**>(base + 3420);
    if (*owned != nullptr) {
        __builtin_delete(*owned);
        *owned = nullptr;
    }
    printf("qaw!1\n");
    char* playback = static_cast<char*>(FirstPlayback__13VISoundDevice(device));
    while (playback != nullptr) {
        char* next = static_cast<char*>(NextPlayback__13VISoundDeviceP10VIPlayback(device, playback));
        void* sound = Sound__13VISoundDevicei(device, *reinterpret_cast<int*>(playback + 28));
        if (Type__C7VISound(sound) == 1)
            Stop__13VISoundDevicei(device, *reinterpret_cast<int*>(playback + 32));
        playback = next;
    }
    printf("qaw!2\n");
    StopAllBgm__13VISoundDevice(device);
    printf("qaw!3\n");
    char** table = reinterpret_cast<char**>(base + 2584);
    for (int index = 127; index >= 0; --index) {
        char* object = table[127 - index];
        if (object == nullptr) continue;
        char* entry = *reinterpret_cast<char**>(object + 12) + 8;
        short offset = *reinterpret_cast<short*>(entry);
        void (*function)(void*, int) = *reinterpret_cast<void (**)(void*, int)>(entry + 4);
        function(object + offset, 3);
    }
    printf("qaw!4\n");
    if (*reinterpret_cast<int*>(base + 4472) != 0) {
        int size = BgmBufferSize__13VISoundDevice(device);
        FreeSpuMem__13VISoundDeviceUii(device, *reinterpret_cast<unsigned*>(base + 4472), size << 1);
        *reinterpret_cast<int*>(base + 4472) = 0;
    }
    void* list = base + 3296;
    printf("qaw!5\n");
    printf("qaw!6\n");
    Init__7VILList(base + 3284);
    Init__7VILList(list);
    unsigned* words = reinterpret_cast<unsigned*>(base + 2584);
    for (int index = 127; index >= 0; --index) words[index] = 0;
    unsigned* more = reinterpret_cast<unsigned*>(base + 3100);
    for (int index = 45; index >= 0; --index) more[index] = 0;
    for (int index = 0; index < 46; ++index)
        AddTail__7VILListP7VILNode(list, base + 8 + 56 * index);
    printf("qaw!7\n");
    *reinterpret_cast<unsigned long long*>(base) = 0;
    *reinterpret_cast<unsigned*>(base + 3096) = 0;
    *reinterpret_cast<unsigned*>(base + 3432) = 1;
    *reinterpret_cast<unsigned*>(base + 3384) = 0;
    *reinterpret_cast<unsigned*>(base + 3408) = 0;
    *reinterpret_cast<unsigned*>(base + 3404) = 0;
    *reinterpret_cast<unsigned*>(base + 3412) = 1;
    *reinterpret_cast<unsigned*>(base + 4472) = 0;
    base[3436] = 0;
    base[3692] = 0;
    base[3948] = 0;
    base[4204] = 0;
    *reinterpret_cast<float*>(base + 4468) = 1.0f;
    *reinterpret_cast<unsigned*>(base + 4460) = 0;
    *reinterpret_cast<unsigned*>(base + 4464) = 0;
    printf("qaw!8\n");
    float* gains = reinterpret_cast<float*>(base + 3424);
    gains[1] = 1.0f;
    gains[0] = 1.0f;
    printf("qaw!9\n");
    return 0;
}
}
