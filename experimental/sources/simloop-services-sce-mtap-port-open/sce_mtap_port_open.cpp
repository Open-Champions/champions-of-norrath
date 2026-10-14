// Observed sceMtapPortOpen at 0x002d4a80, 108 bytes.
// The port number is stored in the first word of the multitap RPC buffer
// (sif_buffer at 0x008ddc40). sceSifCallRpc runs on the port_open client
// (0x008ddb40) with function 1, mode 0, and 128-byte send and receive from
// that buffer, with no end function. A negative result calls DPRINT with the
// text "sceMtapPortOpen: rpc error" and a newline, and returns 0. Otherwise
// the buffer word at +4 is returned.
extern "C" {
extern unsigned char norrath_libmtap_port_open_client_view[];
extern unsigned char norrath_libmtap_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);
void DPRINT(const char* format, ...);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

int sceMtapPortOpen(int port) {
    unsigned char* const buffer = norrath_libmtap_rpc_buffer_view;
    store_word(buffer, static_cast<unsigned>(port));
    if (sceSifCallRpc(norrath_libmtap_port_open_client_view, 1, 0, buffer, 128, buffer, 128, nullptr, nullptr) < 0) {
        DPRINT("sceMtapPortOpen: rpc error\n");
        return 0;
    }
    return static_cast<int>(load_word(buffer + 4));
}
}
