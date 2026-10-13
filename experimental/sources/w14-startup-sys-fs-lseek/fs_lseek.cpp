/* Observed sceLseek at 0x002d9700, 568 bytes, the seek call of the file-system client library. It follows
 * sceRead (0x002d9938, authored in w8-startup-sce-d-fs-read) step for step, with another message.
 * Arguments: descriptor, offset, whence (the three are passed on without interpretation).
 *
 * get_iob(descriptor) runs first, then _sceFsWaitS(4).
 * - A zero _fs_init (0x004449f4) releases through _sceFsSigSema and returns -1.
 * - A null slot, or a slot whose word at offset 4 is zero, releases and returns -9. That word is read as
 *   the slot's flags below.
 * - _send_data (0x008e04c0): word 3 is the slot's first word, word 4 the offset, word 5 the whence, word 6
 *   the slot index (distance from _iob in 16-byte units), word 2 the constant 4 and word 1 the address of a
 *   local result word. A semaphore is made from a six-word parameter block with words 1 = 1, 2 = 0 and 5 = 0
 *   written; its id is word 0 of the message.
 * - When flag bit 0x8000 is set: WaitSema on _fs_fsq_semid (0x00444a04); the first of the 32 words of
 *   _sceFs_q (0x00444970) equal to -1 receives the semaphore id and word 0 of the message becomes its
 *   negation; no such word leaves both untouched; SignalSema on _fs_fsq_semid. Unlike sceRead, no data-cache
 *   write-back is made.
 * - sceSifCallRpc on _cd (0x008e1bc0): function 4, mode 0, 28 bytes of _send_data, 4 bytes received at
 *   _rcv_data_rpc (0x008e1100), no completion function. A negative result deletes the semaphore, releases and
 *   returns -11.
 * - Otherwise the receive word is read through the uncached alias while the lock is released. A zero word
 *   deletes the semaphore and returns -11. With flag bit 0x8000 the semaphore is deleted and 0 returned.
 *   Otherwise the semaphore is waited on and deleted, and the local result word is returned.
 *
 * Inferred: function 4 of the file server is the seek; the local result word is the new position.
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
#else
static unsigned read_reply() {
    return *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(_rcv_data_rpc)));
}
#endif

int sceLseek(int descriptor, int offset, int whence) {
    unsigned char* slot = get_iob(static_cast<unsigned>(descriptor));
    _sceFsWaitS(4);
    if (_fs_init == 0) {
        _sceFsSigSema();
        return -1;
    }
    unsigned flags;
    if (slot == nullptr || (flags = load_word(slot + 4)) == 0u) {
        _sceFsSigSema();
        return -9;
    }
    store_word(_send_data + 16, static_cast<unsigned>(offset));
    store_word(_send_data + 12, load_word(slot));
    const int index = static_cast<int>(pointer_bits(slot) - pointer_bits(_iob)) >> 4;
    store_word(_send_data + 20, static_cast<unsigned>(whence));
    store_word(_send_data + 24, static_cast<unsigned>(index));
    int parameters[6];
    volatile int result;
    parameters[1] = 1;
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
    if (sceSifCallRpc(_cd, 4u, 0u, _send_data, 28, _rcv_data_rpc, 4, nullptr, nullptr) < 0) {
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
