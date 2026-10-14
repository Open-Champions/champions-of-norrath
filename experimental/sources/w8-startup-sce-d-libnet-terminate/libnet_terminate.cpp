/* Observed sceLibnetTerminate at 0x002e5e28, 128 bytes. Calls
 * sceSifMUnBindRpc with the client and mode 0. A negative result returns
 * -540. A result of 1 returns 0. Any other result waits through a counted
 * loop of 0x10001 passes with four nops each, then repeats the call. The
 * semantics of the result values are decided by sceSifMUnBindRpc and are not
 * claimed here.
 */
extern "C" {
int sceSifMUnBindRpc(void* client, unsigned mode);

static void wait_for_server() {
    for (int count = 0x10000; count != -1; --count) __asm__ __volatile__("nop\n\tnop\n\tnop\n\tnop");
}

int sceLibnetTerminate(void* client) {
    for (;;) {
        const int result = sceSifMUnBindRpc(client, 0);
        if (result < 0) return -540;
        if (result == 1) return 0;
        wait_for_server();
    }
}
}
