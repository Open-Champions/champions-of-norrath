/* Observed sceSifMInitRpc at 0x002e45c0, 232 bytes: the start-up routine of
 * the multi-client RPC module. Its flag is _sceSifMInitCheck (0x00447c48);
 * a nonzero flag returns at once with nothing else done and no interrupt
 * handling, leaving the flag value in the result register (callers are not
 * known to use the result). Otherwise the flag is set to 1 and interrupts are disabled
 * (DIntr). The 20-byte pool header at 0x008e8c00 (the packet pool of the
 * multi-client module) then gets word 2 = 32, word 0 = 1, word 1 = the
 * uncached address (OR 0x20000000) of the packet buffer at 0x008e8400, and
 * words 3 and 4 = 0. The command handler for 0x80000018 is registered with the
 * pool header as its parameter, and interrupts are enabled (EIntr). Words 4
 * and 5 of the area 64 bytes into the packet buffer are set to 1, and
 * sceSifSendCmd sends command 0x80000001 from there with 24 bytes and no
 * data transfer. The routine then polls sceSifGetSreg(1) until it is nonzero
 * and finally returns the result of
 * sceSifAddRebootNotifyHandler(0x80000014, handler, 0) as a tail call.
 * Observed handler targets: the local routine named _iopnotify at 0x002e46a8
 * for the reboot notification and the module's own copy of _request_end
 * (0x002e47c8) for the command handler. The latter shares its symbol name
 * with the copy at 0x002d7c08 and is imported here under a distinct name.
 * What the 24-byte packet means is not established.
 */
extern "C" {
extern int _sceSifMInitCheck;
extern unsigned char norrath_sif_m_rpc_packet_pool_view[];
extern unsigned char norrath_sif_m_rpc_packet_buffer_view[];

int DIntr();
void EIntr();
void norrath_sif_m_request_end(void*, void*);
void _iopnotify(void*, void*);
int sceSifAddCmdHandler(unsigned command, void (*handler)(void*, void*), void* parameter);
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source,
                  void* destination, int data_bytes);
unsigned sceSifGetSreg(unsigned index);
int sceSifAddRebootNotifyHandler(unsigned identifier, void (*handler)(void*, void*), void* parameter);

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned uncached(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)) | 0x20000000u;
}

int sceSifMInitRpc() {
    if (_sceSifMInitCheck != 0) return _sceSifMInitCheck;
    _sceSifMInitCheck = 1;
    DIntr();
    unsigned char* const pool = norrath_sif_m_rpc_packet_pool_view;
    store_word(pool + 8, 32u);
    store_word(pool, 1u);
    store_word(pool + 4, uncached(norrath_sif_m_rpc_packet_buffer_view));
    store_word(pool + 12, 0u);
    store_word(pool + 16, 0u);
    sceSifAddCmdHandler(0x80000018u, norrath_sif_m_request_end, pool);
    EIntr();
    unsigned char* const packet = norrath_sif_m_rpc_packet_buffer_view + 64;
    store_word(packet + 20, 1u);
    store_word(packet + 16, 1u);
    sceSifSendCmd(0x80000001u, packet, 24, nullptr, nullptr, 0);
    while (sceSifGetSreg(1u) == 0u) {
    }
    return sceSifAddRebootNotifyHandler(0x80000014u, _iopnotify, nullptr);
}
}
