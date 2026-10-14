// Observed sceClose at 0x002d9580, 380 bytes. get_iob(descriptor) is called
// first, then _sceFsWaitS(1). A zero _fs_init (0x004449f4) releases through
// _sceFsSigSema and returns -1. A null slot, or a slot whose word at offset 4 is
// zero, releases and returns -9. Otherwise the slot's first word goes to
// _send_data + 12 and its index (distance from _iob in 16-byte units) to
// _send_data + 16. A semaphore is created from a six-word parameter block with
// words 1 = 1, 2 = 0 and 5 = 0 written (the others are not); its id goes to
// _send_data + 0, the address of a local result word to + 4 and the constant 4
// to + 8. sceSifCallRpc on _cd sends 20 bytes of _send_data with function 1 and
// receives 4 bytes at _rcv_data_rpc. A negative result deletes the semaphore,
// releases and returns -11. Otherwise the slot's word at offset 4 is cleared,
// the first receive word is read through the uncached alias, the lock is
// released, and a zero receive word deletes the semaphore and returns -11. A
// nonzero one waits on the semaphore, deletes it and returns 0 for a
// nonnegative local result word, otherwise that word. get_iob, _sceFsWaitS and
// _sceFsSigSema are file-local routines of the original library; they are
// left unresolved here. What the slot and message words mean is unknown.
extern "C" {
extern int _fs_init;
extern unsigned char _iob[];
extern unsigned char _send_data[];
extern unsigned char _rcv_data_rpc[];
extern unsigned char _cd[];
unsigned char* get_iob(unsigned descriptor);
int _sceFsWaitS(int flag);
void _sceFsSigSema();
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int norrath_player_wait_semaphore(int semaphore);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_note_result(volatile int* result);
#endif
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
static unsigned pointer_bits(const volatile void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
int sceClose(int descriptor) {
    unsigned char* slot = get_iob(static_cast<unsigned>(descriptor));
    _sceFsWaitS(1);
    if (_fs_init == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (slot == nullptr || load_word(slot + 4) == 0u) {
        _sceFsSigSema();
        return -9;
    }
    store_word(_send_data + 12, load_word(slot));
    store_word(_send_data + 16, (pointer_bits(slot) - pointer_bits(_iob)) >> 4);
    int parameters[6];
    volatile int result;
    parameters[1] = 1;
    parameters[5] = 0;
    parameters[2] = 0;
    const int semaphore = CreateSema(parameters);
    store_word(_send_data, static_cast<unsigned>(semaphore));
    store_word(_send_data + 4, pointer_bits(&result));
    store_word(_send_data + 8, 4u);
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_note_result(&result);
#endif
    if (sceSifCallRpc(_cd, 1u, 0u, _send_data, 20, _rcv_data_rpc, 4, nullptr, nullptr) < 0) {
        DeleteSema(semaphore);
        _sceFsSigSema();
        return -11;
    }
    store_word(slot + 4, 0u);
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned reply = load_word(_rcv_data_rpc);
#else
    const unsigned reply = *reinterpret_cast<volatile unsigned*>(
        static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(_rcv_data_rpc)));
#endif
    _sceFsSigSema();
    if (reply == 0u) {
        DeleteSema(semaphore);
        return -11;
    }
    norrath_player_wait_semaphore(semaphore);
    DeleteSema(semaphore);
    const int value = result;
    return value > -1 ? 0 : value;
}
}
