// Observed scePadInit at 0x002d34b0, 320 bytes.
//
// isInit (0x004448f8) is set to 1. sceSifBindRpc then runs for the first
// padsif client with id 0x80000100 and mode 0 until the client's word +36 is
// nonzero; the result is not read. A failed attempt waits through a counted
// loop of 0x10001 passes (each with four nops) before the next attempt, but
// the first attempt has no wait. The second client (padsif+40) does the same
// with id 0x80000101, checking its word at +36. scePadGetModVersion supplies
// a version; its high part (arithmetic shift by 8) must be 4. A mismatch
// prints, only while the isWarning word at 0x004448fc is nonzero, two
// messages and returns 0: the module text, then the format with 4, 0, the
// high part, and the low byte. A match returns scePadInit2 of the argument.
extern "C" {
extern unsigned char norrath_libpad_is_init_word[];
extern unsigned char norrath_libpad_is_warning_word[];
extern unsigned char norrath_libpad_rpc_clients_view[];

int sceSifBindRpc(void* client, unsigned id, unsigned mode);
int scePadGetModVersion();
int scePadInit2(int mode);
int scePrintf(const char* format, ...);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void wait_for_server() {
    for (int count = 0x10000; count != -1; --count) __asm__ __volatile__("nop\n\tnop\n\tnop\n\tnop");
}

int scePadInit(int mode) {
    store_word(norrath_libpad_is_init_word, 1);
    unsigned char* const first = norrath_libpad_rpc_clients_view;
    unsigned char* const second = first + 40;
    for (;;) {
        sceSifBindRpc(first, 0x80000100u, 0);
        if (load_word(first + 36) != 0) break;
        wait_for_server();
    }
    for (;;) {
        sceSifBindRpc(second, 0x80000101u, 0);
        if (load_word(first + 76) != 0) break;
        wait_for_server();
    }
    const int version = scePadGetModVersion();
    const int major = version >> 8;
    if (major != 4) {
        if (load_word(norrath_libpad_is_warning_word) != 0) {
            scePrintf("libpad: Module version mismatch ");
            scePrintf("[libpad.a = %d.%d, padman.irx = %d.%d]\n", 4, 0, major, version & 0xff);
        }
        return 0;
    }
    return scePadInit2(mode);
}
}
