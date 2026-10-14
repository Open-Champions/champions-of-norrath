// Observed: Reset__13VISoundDevice at 0x011248a8, 656 bytes. It returns 0 and works on the device by byte offset. It prints ten
// short trace lines through printf, the texts being "qaw!\n" and then "qaw!1\n" to "qaw!9\n" in order (the line numbers below
// refer to that suffix). In order:
//   "qaw!\n"; if the pointer at +3420 is nonzero, free it with __builtin_delete and clear it.
//   "qaw!1\n"; walk the playback records (FirstPlayback__13VISoundDevice / NextPlayback__13VISoundDeviceP10VIPlayback, the next one is
//   fetched first). For each, sound = Sound__13VISoundDevicei(device, record word at +28); if Type__C7VISound(sound) == 1, call
//   Stop__13VISoundDevicei(device, record word at +32).
//   "qaw!2\n"; StopAllBgm__13VISoundDevice(device); "qaw!3\n";
//   for the 128 sound slots starting at device+2584: a nonzero pointer has its virtual member at table byte offset 8 called with
//   this = sound + adjustment and second argument 3 (the table is the pointer at sound+12; entries are halfword adjustment, halfword
//   pad, function pointer). The slots are not cleared here.
//   "qaw!4\n"; if the word at +4472 is nonzero, FreeSpuMem__13VISoundDeviceUii(device, that word, 2 * BgmBufferSize__13VISoundDevice(device))
//   and clear it. "qaw!5\n", "qaw!6\n".
//   Init__7VILList on the lists at +3284 and +3296. Clear the 128 words at +2584..+3092, then the 46 words at +3100..+3280.
//   Link the 46 records of 56 bytes at +8 (to +2584) onto the list at +3296 with AddTail__7VILListP7VILNode, lowest address first.
//   "qaw!7\n"; store: the doubleword at +0 = 0; +3096 = 0; +3432 = 1; +3384 = 0; +3408 = 0; +3404 = 0; +3412 = 1; +4472 = 0;
//   the bytes at +3436, +3692, +3948 and +4204 = 0; the float at +4468 = 1.0; +4460 = 0; +4464 = 0. "qaw!8\n"; the floats at
//   +3424 and +3428 = 1.0; "qaw!9\n"; return 0.
// Inferred: +8 is an array of 46 playback records (56 bytes each) with a live list at +3284 and a free list at +3296; +2584 holds
// 128 sound pointers (the slots Sound__13VISoundDevicei and ReleaseSound__13VISoundDeviceiR12VIDictionary use); +0 is the channel
// mask; +3424/+3428 are the class volumes; +3436 and the following 256-byte strips are the BGM name buffers PlayBgm writes; the word
// at +4472 is a BGM buffer in sound memory. Type 1 is XM according to AsXm__7VISound.
// Unknown: the meaning of +3096, +3384 to +3412 (the transfer state shared with BeginAdpcmTransfer and IdleTransfer), +3432, +4460,
// +4464 and +4468, and what the trace markers were for.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
extern "C" void norrath_host_call_destructor(unsigned self, unsigned function, int flags);
static inline void call_destructor(unsigned self, unsigned function, int flags) { norrath_host_call_destructor(self, function, flags); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
static inline void call_destructor(unsigned self, unsigned function, int flags) {
    typedef void (*Fn)(unsigned long, int);
    reinterpret_cast<Fn>(static_cast<unsigned long>(function))(static_cast<unsigned long>(self), flags);
}
#endif

extern "C" {
int printf(const char* format, ...);
void __builtin_delete(void* block);
unsigned FirstPlayback__13VISoundDevice(void* device);
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void* device, unsigned playback);
unsigned Sound__13VISoundDevicei(void* device, int index);
int Type__C7VISound(unsigned sound);
void Stop__13VISoundDevicei(void* device, int voice);
void StopAllBgm__13VISoundDevice(void* device);
int BgmBufferSize__13VISoundDevice(void* device);
void FreeSpuMem__13VISoundDeviceUii(void* device, unsigned address, int size);
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

int Reset__13VISoundDevice(void* device) {
    unsigned char* base = static_cast<unsigned char*>(device);
    unsigned* w = reinterpret_cast<unsigned*>(base);
    printf("qaw!\n");
    if (w[3420 / 4]) {
        __builtin_delete(reinterpret_cast<void*>(static_cast<unsigned long>(w[3420 / 4])));
        w[3420 / 4] = 0;
    }
    printf("qaw!1\n");
    for (unsigned p = FirstPlayback__13VISoundDevice(device); p;) {
        unsigned next = NextPlayback__13VISoundDeviceP10VIPlayback(device, p);
        const unsigned* record = words_at(p);
        unsigned sound = Sound__13VISoundDevicei(device, static_cast<int>(record[28 / 4]));
        if (Type__C7VISound(sound) == 1) Stop__13VISoundDevicei(device, static_cast<int>(record[32 / 4]));
        p = next;
    }
    printf("qaw!2\n");
    StopAllBgm__13VISoundDevice(device);
    printf("qaw!3\n");
    for (int i = 0; i < 128; ++i) {
        unsigned sound = w[2584 / 4 + i];
        if (sound) {
            const unsigned* entry = words_at(words_at(sound)[3]) + 2;
            call_destructor(sound + static_cast<short>(entry[0]), entry[1], 3);
        }
    }
    printf("qaw!4\n");
    if (w[4472 / 4]) {
        int size = BgmBufferSize__13VISoundDevice(device);
        FreeSpuMem__13VISoundDeviceUii(device, w[4472 / 4], size * 2);
        w[4472 / 4] = 0;
    }
    printf("qaw!5\n");
    printf("qaw!6\n");
    Init__7VILList(base + 3284);
    Init__7VILList(base + 3296);
    for (int i = 0; i < 128; ++i) w[2584 / 4 + i] = 0;
    for (int i = 0; i < 46; ++i) w[3100 / 4 + i] = 0;
    for (int i = 0; i < 46; ++i) AddTail__7VILListP7VILNode(base + 3296, base + 8 + 56 * i);
    printf("qaw!7\n");
    *reinterpret_cast<unsigned long long*>(base) = 0;
    w[3096 / 4] = 0;
    w[3432 / 4] = 1;
    w[3384 / 4] = 0;
    w[3408 / 4] = 0;
    w[3404 / 4] = 0;
    w[3412 / 4] = 1;
    w[4472 / 4] = 0;
    base[3436] = 0;
    base[3692] = 0;
    base[3948] = 0;
    base[4204] = 0;
    reinterpret_cast<float*>(base)[4468 / 4] = 1.0f;
    w[4460 / 4] = 0;
    w[4464 / 4] = 0;
    printf("qaw!8\n");
    reinterpret_cast<float*>(base)[3428 / 4] = 1.0f;
    reinterpret_cast<float*>(base)[3424 / 4] = 1.0f;
    printf("qaw!9\n");
    return 0;
}
}
