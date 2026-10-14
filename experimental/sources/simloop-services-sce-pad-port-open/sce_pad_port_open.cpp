// Observed scePadPortOpen at 0x002d3710, 484 bytes.
//
// A buffer address with any of the low six bits set prints the libpad
// alignment message (only while the isWarning word at 0x004448fc is nonzero)
// and returns 0. PadInfo (0x008dd8d0) holds 28 bytes per slot and 112 per
// port; its word +16 equal to 1 means the port is open, which prints the
// already-open message the same way and returns 0.
//
// Two 128-byte halves of the caller's buffer are prepared: word +88 and +96
// and byte +103 are cleared, byte +112 is 5, byte +113 is 2, and the first 32
// bytes are filled with 0xff. The shared RPC buffer (0x008ddac0) then gets
// command 1, the port, the slot and the buffer address at +16, and
// sceSifCallRpc runs on the first padsif client with function 1, mode 0, and
// 128-byte send and receive from that buffer. A negative result returns 0.
// Otherwise the slot's PadInfo gets the address at +0, the matching
// dma_iop_data entry (0x008dd9c0, 32 bytes per slot, 128 per port) at +4
// with its first word cleared, the reply word +20 at +8, zero at +12, and 1
// at +16. The reply word +12 is returned.
extern "C" {
extern unsigned char norrath_libpad_is_warning_word[];
extern unsigned char norrath_libpad_rpc_clients_view[];
extern unsigned char norrath_libpad_port_info_view[];
extern unsigned char norrath_libpad_dma_info_view[];
extern unsigned char norrath_libpad_rpc_buffer_view[];

int scePrintf(const char* format, ...);
void* memset(void* destination, int value, unsigned long size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned address_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

int scePadPortOpen(int port, int slot, void* address) {
    const unsigned buffer = address_of(address);
    if ((buffer & 0x3fu) != 0) {
        if (load_word(norrath_libpad_is_warning_word) != 0)
            scePrintf("libpad: buffer addr is not 64 byte align. %08x\n", buffer);
        return 0;
    }
    unsigned char* const info = norrath_libpad_port_info_view + slot * 28 + port * 112;
    if (load_word(info + 16) == 1) {
        if (load_word(norrath_libpad_is_warning_word) != 0)
            scePrintf("libpad: pad port is already open [%d][%d]\n", port, slot);
        return 0;
    }
    unsigned char* half = static_cast<unsigned char*>(address);
    for (int count = 1; count >= 0; --count, half += 128) {
        store_word(half + 88, 0);
        half[112] = 5;
        half[113] = 2;
        half[103] = 0;
        memset(half, 255, 32);
        store_word(half + 96, 0);
    }
    unsigned char* const rpc = norrath_libpad_rpc_buffer_view;
    store_word(rpc, 1);
    store_word(rpc + 4, static_cast<unsigned>(port));
    store_word(rpc + 8, static_cast<unsigned>(slot));
    store_word(rpc + 16, buffer);
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, rpc, 128, rpc, 128, nullptr, nullptr) < 0) return 0;
    unsigned char* const dma = norrath_libpad_dma_info_view + (slot << 5) + (port << 7);
    const unsigned reply = load_word(rpc + 20);
    store_word(info + 16, 1);
    store_word(dma, 0);
    store_word(info + 12, 0);
    store_word(info + 8, reply);
    store_word(info, buffer);
    store_word(info + 4, address_of(dma));
    return static_cast<int>(load_word(rpc + 12));
}
}
