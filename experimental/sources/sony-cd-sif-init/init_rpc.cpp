// Observed sceSifInitRpc at 0x002d7908, 412 bytes. The incoming argument is unused.
// A nonzero _sceSifInitCheck returns after restoring interrupts. Otherwise the
// 64-byte _data_table at 0x008e0440 is published, four command handlers are
// registered, and a zero sceSifGetReg(0x80000002) sends 16 bytes from
// _packet_buffer+64 and waits until sceSifGetSreg(0) is nonzero.
extern "C" {
extern unsigned char _data_table[];
extern unsigned char _packet_buffer[];
extern unsigned char _free_buffer[];
extern unsigned char _free_buffer2[];
extern int _sceSifInitCheck;
int DIntr();
void EIntr();
void sceSifInitCmd();
int sceSifAddCmdHandler(unsigned command, void (*handler)(void*, void*), void* parameter);
unsigned long long sceSifGetReg(unsigned identifier);
unsigned long long sceSifGetSreg(unsigned identifier);
int sceSifSetReg(unsigned identifier, unsigned value);
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source,
                  void* destination, int data_bytes);
void _request_end(void*, void*);
void _request_bind(void*, void*);
void _request_call(void*, void*);
void _request_rdata(void*, void*);
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned uncached(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)) | 0x20000000u;
}
void sceSifInitRpc() {
    (void)DIntr();
    if (_sceSifInitCheck != 0) {
        EIntr();
        return;
    }
    _sceSifInitCheck = 1;
    EIntr();
    sceSifInitCmd();
    (void)DIntr();
    store_word(_data_table + 32, 32u);
    store_word(_data_table, 1u);
    store_word(_data_table + 20, uncached(_free_buffer));
    store_word(_data_table + 4, uncached(_packet_buffer));
    store_word(_data_table + 28, uncached(_free_buffer2));
    store_word(_data_table + 8, 32u);
    store_word(_data_table + 12, 0u);
    store_word(_data_table + 16, 0u);
    store_word(_data_table + 24, 32u);
    store_word(_data_table + 36, 0u);
    sceSifAddCmdHandler(0x80000008u, _request_end, _data_table);
    sceSifAddCmdHandler(0x80000009u, _request_bind, _data_table);
    sceSifAddCmdHandler(0x8000000au, _request_call, _data_table);
    sceSifAddCmdHandler(0x8000000cu, _request_rdata, _data_table);
    EIntr();
    if (sceSifGetReg(0x80000002u) != 0ull) return;
    unsigned char* packet = _packet_buffer + 64;
    store_word(packet + 12, 1u);
    sceSifSendCmd(0x80000002u, packet, 16, nullptr, nullptr, 0);
    while (sceSifGetSreg(0u) == 0ull) {
    }
    sceSifSetReg(0x80000002u, 1u);
}
}
