// Observed sceMtapInit at 0x002d4840, 580 bytes.
//
// sceSifInitRpc(0) runs first. Five RPC clients are then bound in order with
// sceSifBindRpc and mode 0: port_open (0x008ddb40) with id 0x80000901,
// port_close, get_connection, and change_priority at 40-byte steps with the
// next ids, and module_version with 0x80000905. After each bind a negative
// result calls DPRINT("libmtap: bind failed\n") and returns 0. A client
// whose word +36 is still zero waits through a counted loop of 0x10001
// passes (four nops each) and is bound again; the first attempt for each
// client has no wait. sceMtapGetModVersion then supplies a version whose
// arithmetic shift right by 8 must be 3. Otherwise two scePrintf messages
// are printed (no warning flag is consulted here): the module text, then
// the format with 3, 0, the high part, and the low byte, and 0 is returned.
// A match returns 1.
extern "C" {
extern unsigned char norrath_libmtap_port_open_client_view[];
extern unsigned char norrath_libmtap_port_close_client_view[];
extern unsigned char norrath_libmtap_get_connection_client_view[];
extern unsigned char norrath_libmtap_change_priority_client_view[];
extern unsigned char norrath_libmtap_module_version_client_view[];

int sceSifInitRpc(unsigned mode);
int sceSifBindRpc(void* client, unsigned id, unsigned mode);
int sceMtapGetModVersion();
int scePrintf(const char* format, ...);
void DPRINT(const char* format, ...);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void wait_for_server() {
    for (int count = 0x10000; count != -1; --count) __asm__ __volatile__("nop\n\tnop\n\tnop\n\tnop");
}
static bool bind_client(unsigned char* client, unsigned id) {
    for (;;) {
        if (sceSifBindRpc(client, id, 0) < 0) {
            DPRINT("libmtap: bind failed\n");
            return false;
        }
        if (load_word(client + 36) != 0) return true;
        wait_for_server();
    }
}

int sceMtapInit() {
    sceSifInitRpc(0);
    if (!bind_client(norrath_libmtap_port_open_client_view, 0x80000901u)) return 0;
    if (!bind_client(norrath_libmtap_port_close_client_view, 0x80000902u)) return 0;
    if (!bind_client(norrath_libmtap_get_connection_client_view, 0x80000903u)) return 0;
    if (!bind_client(norrath_libmtap_change_priority_client_view, 0x80000904u)) return 0;
    if (!bind_client(norrath_libmtap_module_version_client_view, 0x80000905u)) return 0;
    const int version = sceMtapGetModVersion();
    const int major = version >> 8;
    if (major != 3) {
        scePrintf("libmtap: Module version mismatch ");
        scePrintf("[libmtap.a = %d.%d, mtapman.irx = %d.%d]\n", 3, 0, major, version & 0xff);
        return 0;
    }
    return 1;
}
}
