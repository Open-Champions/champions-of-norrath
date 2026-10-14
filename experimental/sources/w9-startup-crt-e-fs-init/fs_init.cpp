/* Observed sceFsInit at 0x002d9028, 516 bytes. In order:
 *
 * - sceSifInitRpc(0). The first word of _sif_FsPoff_Data (0x008e1c40) is cleared, then
 *   DIntr runs and the second word of _sif_FsPoff_Data is cleared (a store in the call's delay
 *   slot, so it happens before DIntr is entered).
 * - sceSifAddCmdHandler(0x80000011, _sceFs_Rcv_Intr, _sif_FsRcv_Data) and
 *   sceSifAddCmdHandler(0x80000013, _sceFs_Poff_Intr, _sif_FsPoff_Data). _sceFs_Rcv_Intr is
 *   at 0x002d8af0 and _sceFs_Poff_Intr at 0x002d8fe0. When DIntr returned nonzero, EIntr runs.
 * - sceSifBindRpc(_cd, 0x80000001, 0) is repeated. A negative result returns -1. A result with
 *   the word at offset 36 of _cd still zero spins for 0x100001 counts and retries without
 *   limit.
 * - _sceFsIobSemaMK(), then WaitSema on _fs_iob_semid (0x00444a00). Word 1 of each of the
 *   thirty-two 16-byte slots of _iob (0x008e19c0) is cleared, then SignalSema.
 * - The two words at rcv_adr.39 (0x008e0480) are set to the address of _rcv_data_cmd and to
 *   that address plus 0x440. sceSifCallRpc on _cd: function 255, mode 0, those 8 bytes as the
 *   send area, 8 bytes received at _rcv_data_rpc (0x008e1100), no completion function. A
 *   negative result returns 0xfffeffff.
 * - Otherwise the first received word is stored in _fsversion (0x008e1be8) and the second one is
 *   read, both through the uncached alias (address | 0x20000000). _fs_init (0x004449f4) is set
 *   to 1, _fs_rcv_bufdbl (0x004449f8) to 1 when the second word is 2 and 0 otherwise, and the
 *   result is 0.
 *
 * _sceFsIobSemaMK, _sceFs_Rcv_Intr and _sceFs_Poff_Intr are local routines of the original
 * library and stay unresolved here. What the server returns is unknown beyond the two words.
 */
extern "C" {
extern int _fs_init;
extern int _fs_rcv_bufdbl;
extern int _fs_iob_semid;
extern unsigned char _iob[];
extern unsigned char _cd[];
extern unsigned char _rcv_data_cmd[];
extern unsigned char _rcv_data_rpc[];
extern unsigned char _fsversion[];
extern unsigned char _sif_FsRcv_Data[];
extern unsigned char _sif_FsPoff_Data[];
extern unsigned norrath_fs_rcv_adr_view[];

void sceSifInitRpc(int mode);
int DIntr();
int EIntr();
void sceSifAddCmdHandler(int id, void* handler, void* argument);
int sceSifBindRpc(void* client, unsigned server, unsigned mode);
int WaitSema(int semaphore);
int SignalSema(int semaphore);
void _sceFsIobSemaMK();
void _sceFs_Rcv_Intr();
void _sceFs_Poff_Intr();
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

static void spin() {
    for (int count = 0x100000;;) {
        --count;
#if defined(__mips__)
        asm volatile("nop\n\tnop\n\tnop\n\tnop");
#endif
        if (count == -1) break;
    }
}

static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

static unsigned reply_word(int index) {
#if defined(NORRATH_HOST_CONTRACT)
    return reinterpret_cast<const unsigned*>(_rcv_data_rpc)[index];
#else
    return *reinterpret_cast<const volatile unsigned*>(
        static_cast<__UINTPTR_TYPE__>(0x20000000u | pointer_bits(_rcv_data_rpc + index * 4)));
#endif
}

int sceFsInit() {
    sceSifInitRpc(0);
    reinterpret_cast<unsigned*>(_sif_FsPoff_Data)[0] = 0;
    reinterpret_cast<unsigned*>(_sif_FsPoff_Data)[1] = 0;
    const int interrupts = DIntr();
    sceSifAddCmdHandler(static_cast<int>(0x80000011u), reinterpret_cast<void*>(_sceFs_Rcv_Intr), _sif_FsRcv_Data);
    sceSifAddCmdHandler(static_cast<int>(0x80000013u), reinterpret_cast<void*>(_sceFs_Poff_Intr), _sif_FsPoff_Data);
    if (interrupts != 0) EIntr();
    for (;;) {
        if (sceSifBindRpc(_cd, 0x80000001u, 0u) < 0) return -1;
        if (reinterpret_cast<const int*>(_cd)[9] != 0) break;
        spin();
    }
    _sceFsIobSemaMK();
    WaitSema(_fs_iob_semid);
    for (unsigned char* slot = _iob; slot < _iob + 512; slot += 16) reinterpret_cast<unsigned*>(slot)[1] = 0;
    SignalSema(_fs_iob_semid);
    norrath_fs_rcv_adr_view[0] = pointer_bits(_rcv_data_cmd);
    norrath_fs_rcv_adr_view[1] = pointer_bits(_rcv_data_cmd) + 0x440u;
    if (sceSifCallRpc(_cd, 255u, 0u, norrath_fs_rcv_adr_view, 8, _rcv_data_rpc, 8, nullptr, nullptr) < 0)
        return static_cast<int>(0xfffeffffu);
    const unsigned version = reply_word(0);
    _fsversion[0] = static_cast<unsigned char>(version);
    _fsversion[1] = static_cast<unsigned char>(version >> 8);
    _fsversion[2] = static_cast<unsigned char>(version >> 16);
    _fsversion[3] = static_cast<unsigned char>(version >> 24);
    const unsigned second = reply_word(1);
    _fs_init = 1;
    _fs_rcv_bufdbl = second == 2u;
    return 0;
}
}
