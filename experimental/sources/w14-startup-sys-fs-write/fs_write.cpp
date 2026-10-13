/* Observed sceWrite at 0x002d9b98, 704 bytes, the write call of the file-system client library. It follows
 * sceRead (0x002d9938, authored in w8-startup-sce-d-fs-read) with a longer message that carries the unaligned
 * head of the data. Arguments: descriptor, buffer, size.
 *
 * get_iob(descriptor) runs first, then _sceFsWaitS(3).
 * - A zero _fs_init (0x004449f4) releases through _sceFsSigSema and returns -1.
 * - A null slot, or a slot whose word at offset 4 is zero, releases and returns -9. That word is read as
 *   the slot's flags below.
 * - _send_data (0x008e04c0): word 3 is the slot's first word, word 4 the buffer address, word 5 the size,
 *   word 11 the slot index (distance from _iob in 16-byte units), word 2 the constant 4 and word 1 the
 *   address of a local result word. A semaphore is made from a six-word parameter block with words 1 = 1,
 *   2 = 0 and 5 = 0 written; its id is word 0 of the message.
 * - When flag bit 0x8000 is set: WaitSema on _fs_fsq_semid (0x00444a04); the first of the 32 words of
 *   _sceFs_q (0x00444970) equal to -1 receives the semaphore id and word 0 of the message becomes its
 *   negation; no such word leaves both untouched; SignalSema on _fs_fsq_semid.
 * - The head length is 0 for a buffer address that is a multiple of 16, otherwise the distance to the next
 *   multiple of 16 (16 minus the low four bits), and never more than the size. Unless flag bit 0x20000000 is
 *   set, the whole buffer (size bytes) is written back from the data cache. Word 6 of the message gets the
 *   head length, and that many bytes are copied from the buffer, read through its uncached alias
 *   (address | 0x20000000), to the message from byte 28 on.
 * - sceSifCallRpc on _cd (0x008e1bc0): function 3, mode 0, 48 bytes of _send_data, 4 bytes received at
 *   _rcv_data_rpc (0x008e1100), no completion function. A negative result deletes the semaphore, releases and
 *   returns -11.
 * - Otherwise the receive word is read through the uncached alias while the lock is released. A zero word
 *   deletes the semaphore and returns -11. With flag bit 0x8000 the semaphore is deleted and 0 returned.
 *   Otherwise the semaphore is waited on and deleted, and the local result word is returned.
 *
 * Inferred: the head is sent inside the message because the IOP side fetches only aligned blocks by DMA.
 * The helpers get_iob, _sceFsWaitS and _sceFsSigSema are local routines of the original library and stay
 * unresolved here. Unknown: what the slot, flag and message words mean to the IOP.
 */
extern "C" {
extern int _fs_init;
extern int _fs_fsq_semid;
extern int _sceFs_q[];
extern unsigned char _iob[];
extern unsigned char _send_data[];
extern unsigned char _rcv_data_rpc[];
extern unsigned char _cd[];

unsigned char* get_iob(unsigned descriptor);
int _sceFsWaitS(int flag);
void _sceFsSigSema();
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int WaitSema(int semaphore);
int SignalSema(int semaphore);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(volatile void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_note_result(volatile int* result);
static unsigned read_reply() { return load_word(_rcv_data_rpc); }
static unsigned char uncached_byte(const unsigned char* address) { return *address; }
#else
static unsigned read_reply() {
    return *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(_rcv_data_rpc)));
}
static unsigned char uncached_byte(const unsigned char* address) {
    return *reinterpret_cast<volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(const_cast<unsigned char*>(address))));
}
#endif

int sceWrite(int descriptor, const void* buffer, int size) {
    unsigned char* slot = get_iob(static_cast<unsigned>(descriptor));
    _sceFsWaitS(3);
    if (_fs_init == 0) {
        _sceFsSigSema();
        return -1;
    }
    unsigned flags;
    if (slot == nullptr || (flags = load_word(slot + 4)) == 0u) {
        _sceFsSigSema();
        return -9;
    }
    store_word(_send_data + 12, load_word(slot));
    int parameters[6];
    volatile int result;
    parameters[1] = 1;
    const int index = static_cast<int>(pointer_bits(slot) - pointer_bits(_iob)) >> 4;
    store_word(_send_data + 44, static_cast<unsigned>(index));
    store_word(_send_data + 20, static_cast<unsigned>(size));
    store_word(_send_data + 16, pointer_bits(const_cast<void*>(buffer)));
    parameters[2] = 0;
    parameters[5] = 0;
    const int semaphore = CreateSema(parameters);
    store_word(_send_data + 8, 4u);
    store_word(_send_data + 4, pointer_bits(&result));
    store_word(_send_data, static_cast<unsigned>(semaphore));
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_note_result(&result);
#endif
    if (flags & 0x8000u) {
        WaitSema(_fs_fsq_semid);
        for (int entry = 0; entry < 32; ++entry) {
            if (_sceFs_q[entry] == -1) {
                const int identifier = static_cast<int>(load_word(_send_data));
                _sceFs_q[entry] = identifier;
                store_word(_send_data, static_cast<unsigned>(-identifier));
                break;
            }
        }
        SignalSema(_fs_fsq_semid);
    }
    const unsigned address = pointer_bits(const_cast<void*>(buffer));
    int head = (address & 15u) == 0u ? 0 : static_cast<int>(16u - (address & 15u));
    if (size < head) head = size;
    if ((flags & 0x20000000u) == 0) sceSifWriteBackDCache(const_cast<void*>(buffer), size);
    store_word(_send_data + 24, static_cast<unsigned>(head));
    const unsigned char* const source = static_cast<const unsigned char*>(buffer);
    for (int i = 0; i < head; ++i) _send_data[28 + i] = uncached_byte(source + i);
    if (sceSifCallRpc(_cd, 3u, 0u, _send_data, 48, _rcv_data_rpc, 4, nullptr, nullptr) < 0) {
        DeleteSema(semaphore);
        _sceFsSigSema();
        return -11;
    }
    const unsigned reply = read_reply();
    _sceFsSigSema();
    if (reply == 0u) {
        DeleteSema(semaphore);
        return -11;
    }
    if (flags & 0x8000u) {
        DeleteSema(semaphore);
        return 0;
    }
    WaitSema(semaphore);
    DeleteSema(semaphore);
    return result;
}
}
