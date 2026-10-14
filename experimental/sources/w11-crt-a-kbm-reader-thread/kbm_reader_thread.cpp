// Observed KbmReaderThread at 0x002e89a8, 980 bytes (a local symbol), pinned SLUS_205.65. The entry
// of the keyboard/mouse reader thread that CreateReaderThread starts (see
// w10-startup-a-create-reader-thread). It never returns. It works on two objects of the keyboard
// library: GLibData (0x008ebdd8, 1640 bytes) and the local buffer LocalData (0x008e8c80). Offsets
// are bytes; words are 32-bit unless noted.
//   Set-up: LocalData[3] = 16; LocalData[514] = GLibData[64] + GLibData[68] (the two byte counts);
//   LocalData[512] = LocalData[513] = 0. For i below the word at GLibData+64 the halfword at
//   GLibData + 140 + 272*i is set to -1; for i below the word at +68 the halfword at
//   GLibData + 1228 + 104*i is set to -1. Then InitialiseIopDriver().
//   Wait for devices: while the 64-bit value at GLibData+80 is zero, do one "poll" (below).
//   Forever: poll when LocalData[512] differs from the word at GLibData+80, or LocalData[513] from
//   the word at +84. Then read the device data: sceSifCallRpc(GLibData, 2, 1, LocalData, 464,
//   LocalData, 464, kdm_rpccall_end, LocalData) and WaitSema on the word at +40. If the word at
//   +60 is nonzero, make a second call (function 4, no buffers, no argument, same end function),
//   WaitSema again only when that call returned nonnegative, clear the word at +60 and start over.
//   Otherwise copy LocalData[0] and [1] into the words at +80 and +84 when they differ and handle
//   the LocalData[2] records of 28 bytes from LocalData+4: a record whose halfword at +4 is 1 goes
//   to process_kbd_data, 2 to process_mouse_data, others are skipped; the count is read again after
//   each call to either processor.
//   Poll: sceSifCallRpc(GLibData, 1, 1, LocalData+512, 144, LocalData+512, 144, kdm_rpccall_end,
//   LocalData+512), WaitSema on the word at +40, copy bytes 0 and 1 of the reply to the words at +80
//   and +84 when they differ, then walk (word64 + word68) 16-byte records from LocalData+516:
//   the halfword at +2 selects mark_device_unplugged(halfword at +4) for 0, setup_kbd_device(record)
//   for 1, setup_mouse_device(record) for 2; any other value does nothing.
// Inferred: the call mode 1 is the non-blocking form (the semaphore wait follows every call) and
// the end function signals that semaphore; the record kinds follow the callee names. Unknown: the
// layouts of the records and of GLibData beyond the offsets used here, and whether the unplugged
// case really takes the halfword at +4 as an identifier. The six local callees and the
// end function are imports under their original names. Nothing was executed on the EE.
extern "C" {
typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef long long __attribute__((may_alias)) AliasedLong;

extern unsigned char GLibData[];
extern unsigned char LocalData[];
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void (*end_function)(void*), void* end_argument);
int WaitSema(int semaphore);
void InitialiseIopDriver();
void kdm_rpccall_end(void* argument);
void mark_device_unplugged(int device);
void setup_kbd_device(void* record);
void setup_mouse_device(void* record);
void process_kbd_data(void* record);
void process_mouse_data(void* record);
}

static inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }

static void poll_devices(unsigned char* g, unsigned char* buffer) {
    sceSifCallRpc(g, 1, 1, buffer, 144, buffer, 144, kdm_rpccall_end, buffer);
    WaitSema(word(g, 40));
    if (word(g, 80) != buffer[0]) word(g, 80) = buffer[0];
    if (word(g, 84) != buffer[1]) word(g, 84) = buffer[1];
    int remaining = word(g, 64) + word(g, 68);
    if (remaining <= 0) return;
    unsigned char* record = LocalData + 516;
    do {
        const int kind = *reinterpret_cast<AliasedShort*>(record + 2);
        if (kind == 0) mark_device_unplugged(*reinterpret_cast<AliasedShort*>(record + 4));
        else if (kind == 1) setup_kbd_device(record);
        else if (kind == 2) setup_mouse_device(record);
        --remaining;
        record += 16;
    } while (remaining != 0);
}

extern "C" void KbmReaderThread() {
    unsigned char* const g = GLibData;
    unsigned char* const local = LocalData;
    local[3] = 16;
    local[514] = static_cast<unsigned char>(g[64] + g[68]);
    local[512] = 0;
    local[513] = 0;
    for (int i = 0; i < word(g, 64); ++i) *reinterpret_cast<AliasedShort*>(g + 140 + 272 * i) = -1;
    for (int i = 0; i < word(g, 68); ++i) *reinterpret_cast<AliasedShort*>(g + 1228 + 104 * i) = -1;
    InitialiseIopDriver();
    while (*reinterpret_cast<AliasedLong*>(g + 80) == 0) poll_devices(g, local + 512);
    for (;;) {
        if (local[512] != word(g, 80) || local[513] != word(g, 84)) poll_devices(g, local + 512);
        sceSifCallRpc(g, 2, 1, local, 464, local, 464, kdm_rpccall_end, local);
        WaitSema(word(g, 40));
        if (word(g, 60) != 0) {
            const int result = sceSifCallRpc(g, 4, 1, 0, 0, 0, 0, kdm_rpccall_end, 0);
            if (result >= 0) WaitSema(word(g, 40));
            word(g, 60) = 0;
            continue;
        }
        if (word(g, 80) != local[0]) word(g, 80) = local[0];
        if (word(g, 84) != local[1]) word(g, 84) = local[1];
        if (local[2] == 0) continue;
        int index = 0;
        int count = local[2];
        unsigned char* record = local + 4;
        for (;;) {
            const int kind = *reinterpret_cast<AliasedShort*>(record + 4);
            if (kind == 1) {
                process_kbd_data(record);
                count = local[2];
            } else if (kind == 2) {
                process_mouse_data(record);
                count = local[2];
            }
            ++index;
            if (!(index < count)) break;
            record += 28;
        }
    }
}
