/* Observed sceOpen at 0x002d92f8, 644 bytes. Takes a name, a flags word and
 * a variadic third argument that is read as an int (the original saves all
 * of its argument registers to the stack). In order:
 *
 * - _sceFsWaitS(0) runs. A zero _fs_init (0x004449f4) runs sceFsInit.
 * - A nonzero _fs_version() releases through _sceFsSigSema and returns
 *   0xfffefffc.
 * - new_iob() returning null releases and returns -19.
 * - The name is copied to offset 20 of _send_data (0x008e04c0), NUL included,
 *   at most 1024 bytes. A copy that reaches 1024 bytes has offset 20 + 1023
 *   forced to zero.
 * - _send_data word 3 is the flags with bit 31 cleared, word 4 the third
 *   argument, and word 261 (offset 1044) the slot index, the distance of the
 *   new slot from _iob (0x008e19c0) in 16-byte units.
 * - A semaphore is made from a six-word parameter block with words 1 = 1,
 *   2 = 0 and 5 = 0 written (the others are not). Its id is word 0, the
 *   address of a local result word is word 1, and 4 is word 2.
 * - sceSifCallRpc on _cd (0x008e1bc0): function 0, mode 0, 1048 bytes of
 *   _send_data, 4 bytes received at _rcv_data_rpc (0x008e1100), no completion
 *   function. A negative result deletes the semaphore, releases and returns
 *   -11.
 * - Otherwise the receive word is read through the uncached alias while the
 *   lock is released. A zero word deletes the semaphore and returns -11.
 * - Otherwise the semaphore is waited on and deleted. A negative local
 *   result waits on _fs_iob_semid (0x00444a00), clears the slot word at
 *   offset 4, signals, and returns that result. A nonnegative one waits on
 *   _fs_iob_semid, ORs the flags argument (unmasked) into the slot word at
 *   offset 4, stores the local result in the slot's first word, signals, and
 *   returns the slot index.
 *
 * get_iob, new_iob, _sceFsWaitS, _sceFsSigSema and _fs_version are local
 * routines of the original library and stay unresolved here. What the slot
 * and message words mean to the IOP is unknown.
 */
extern "C" {
extern int _fs_init;
extern int _fs_iob_semid;
extern unsigned char _iob[];
extern unsigned char _send_data[];
extern unsigned char _rcv_data_rpc[];
extern unsigned char _cd[];

unsigned char* new_iob();
int _sceFsWaitS(int flag);
void _sceFsSigSema();
int _fs_version();
int sceFsInit();
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

int sceOpen(const char* name, int flags, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, flags);
    const int mode = __builtin_va_arg(arguments, int);
    __builtin_va_end(arguments);

    _sceFsWaitS(0);
    if (_fs_init == 0) sceFsInit();
    if (_fs_version() != 0) {
        _sceFsSigSema();
        return static_cast<int>(0xfffefffc);
    }
    unsigned char* slot = new_iob();
    if (slot == nullptr) {
        _sceFsSigSema();
        return -19;
    }
    unsigned char* const text = _send_data + 20;
    int length = 0;
    text[0] = static_cast<unsigned char>(name[0]);
    if (name[0] != 0) {
        for (;;) {
            ++length;
            if (length >= 1024) break;
            text[length] = static_cast<unsigned char>(name[length]);
            if (name[length] == 0) break;
        }
    }
    if (length == 1024) text[1023] = 0;
    const int index = static_cast<int>(pointer_bits(slot) - pointer_bits(_iob)) >> 4;
    store_word(_send_data + 12, static_cast<unsigned>(flags) & 0x7fffffffu);
    store_word(_send_data + 16, static_cast<unsigned>(mode));
    int parameters[6];
    volatile int result;
    parameters[1] = 1;
    store_word(_send_data + 1044, static_cast<unsigned>(index));
    parameters[2] = 0;
    parameters[5] = 0;
    const int semaphore = CreateSema(parameters);
    store_word(_send_data + 4, pointer_bits(&result));
    store_word(_send_data, static_cast<unsigned>(semaphore));
    store_word(_send_data + 8, 4u);
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_note_result(&result);
#endif
    if (sceSifCallRpc(_cd, 0u, 0u, _send_data, 1048, _rcv_data_rpc, 4, nullptr, nullptr) < 0) {
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
    WaitSema(semaphore);
    DeleteSema(semaphore);
    if (result < 0) {
        WaitSema(_fs_iob_semid);
        store_word(slot + 4, 0u);
        SignalSema(_fs_iob_semid);
        return result;
    }
    WaitSema(_fs_iob_semid);
    store_word(slot + 4, load_word(slot + 4) | static_cast<unsigned>(flags));
    store_word(slot, static_cast<unsigned>(result));
    SignalSema(_fs_iob_semid);
    return index;
}
}
