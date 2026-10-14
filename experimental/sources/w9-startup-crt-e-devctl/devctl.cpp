/* Observed sceDevctl at 0x002dbad0, 568 bytes. Arguments: a device name, a command word,
 * an input pointer and length, an output pointer and length. In order:
 *
 * - _sceFsWaitS(23) runs. A zero _fs_init (0x004449f4) runs sceFsInit.
 * - The name is copied to offset 12 of _send_data (0x008e04c0), NUL included, at most
 *   1024 bytes. A copy that reaches 1024 bytes has offset 12 + 1023 forced to zero.
 * - An input length or output length above 1024 releases through _sceFsSigSema and returns
 *   -22. The name copy has already happened by then.
 * - The input bytes are copied to offset 1036 of _send_data. Offset 2060 receives the command,
 *   2064 the input length, 2068 the output pointer and 2072 the output length.
 * - A semaphore is made from a six-word parameter block with words 1 = 1, 2 = 0 and 5 = 0
 *   written. Its id is _send_data word 0, the address of a local result word is word 1, and
 *   4 is word 2. 2076 bytes of _send_data are written back from the data cache.
 * - sceSifCallRpc on _cd (0x008e1bc0): function 23, mode 0, 2076 bytes of _send_data, 4 bytes
 *   received at _rcv_data_rpc (0x008e1100), no completion function. A negative result deletes
 *   the semaphore, releases and returns -11.
 * - Otherwise the receive word is read through the uncached alias and the lock is released.
 *   A zero word deletes the semaphore and returns -11. Otherwise the semaphore is waited on
 *   and deleted, and the local result word is returned.
 *
 * _sceFsWaitS and _sceFsSigSema are local routines of the original library and stay
 * unresolved here. What the server does with the command and buffers is unknown.
 */
extern "C" {
extern int _fs_init;
extern unsigned char _send_data[];
extern unsigned char _rcv_data_rpc[];
extern unsigned char _cd[];

int _sceFsWaitS(int flag);
void _sceFsSigSema();
int sceFsInit();
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int WaitSema(int semaphore);
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
static unsigned pointer_bits(const volatile void* pointer) {
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

int sceDevctl(const char* name, int command, const void* input, unsigned input_size, void* output,
              unsigned output_size) {
    _sceFsWaitS(23);
    if (_fs_init == 0) sceFsInit();
    unsigned char* const text = _send_data + 12;
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
    if (input_size > 1024u || output_size > 1024u) {
        _sceFsSigSema();
        return -22;
    }
    const unsigned char* const source = static_cast<const unsigned char*>(input);
    for (unsigned i = 0; i < input_size; ++i) _send_data[1036 + i] = source[i];
    store_word(_send_data + 2064, input_size);
    store_word(_send_data + 2060, static_cast<unsigned>(command));
    int parameters[6];
    volatile int result;
    parameters[1] = 1;
    parameters[2] = 0;
    parameters[5] = 0;
    const int semaphore = CreateSema(parameters);
    store_word(_send_data + 2072, output_size);
    store_word(_send_data + 4, pointer_bits(&result));
    store_word(_send_data + 8, 4u);
    store_word(_send_data + 2068, pointer_bits(output));
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_note_result(&result);
#endif
    store_word(_send_data, static_cast<unsigned>(semaphore));
    sceSifWriteBackDCache(_send_data, 2076);
    if (sceSifCallRpc(_cd, 23u, 0u, _send_data, 2076, _rcv_data_rpc, 4, nullptr, nullptr) < 0) {
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
    return result;
}
}
