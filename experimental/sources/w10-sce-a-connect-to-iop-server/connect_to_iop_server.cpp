/* Observed connect_to_iop_server at 0x002e8fa0, 196 bytes. No arguments. It
 * calls sceSifInitRpc(0) and clears the first 40 bytes of GLibData
 * (0x008ebdd8), which is the RPC client used here. Then it binds that client
 * to service number 0x61766973 with sceSifBindRpc(client, 0x61766973, 0).
 * A negative bind result hangs in an endless loop with no further calls. A
 * bound client (client word 9, byte offset 36, nonzero) ends the routine with
 * result 0. If the word is still zero, a busy delay of 16384 iterations of
 * five no-operation instructions runs and the bind is attempted again, with no
 * limit on the number of attempts and no new clear of the client.
 * The delay loop is written with inline no-operations; its length was counted
 * from the original loop and no timing was measured.
 */
extern "C" {
extern unsigned char GLibData[];

void sceSifInitRpc(int mode);
void* memset(void* destination, int value, __SIZE_TYPE__ count);
int sceSifBindRpc(void* client, unsigned service, unsigned mode);

int connect_to_iop_server() {
    sceSifInitRpc(0);
    memset(GLibData, 0, 40);
    for (;;) {
        if (sceSifBindRpc(GLibData, 0x61766973u, 0) < 0) {
            for (;;) {
            }
        }
        if (*reinterpret_cast<volatile unsigned*>(GLibData + 36) != 0) return 0;
        for (unsigned count = 0; count < 16384u; ++count) {
            __asm__ volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop");
        }
    }
}
}
