// Observed initEzNetCnf at 0x010becd0, 200 bytes, in the overlay range. It looks
// up the IOP module "eznetcnf" with sceSifSearchModuleByName and, when that
// result is negative, prints it with the module name. It then binds the local
// client m_rpcCd (0x011e8380, 64 bytes) to server 0x75499128. A negative bind
// prints "could not bind to eznetcnf service". A zero word at client offset 36
// means the server has not answered: a countdown from 0x00010000 to -1 runs and
// the bind is repeated. Once that word is nonzero it stores 1 in the local flag
// at 0x01192c68. A failed lookup or failed bind does not stop the sequence; only
// the word at offset 36 ends the loop. The result register still holds 1 on
// return, but nothing is returned on purpose.
// The flag is the word exitEzNetCnf clears. That function is authored in another
// unit with its own file-local copy, so the two do not yet share storage here.
// Unknown: the meaning of the result codes and of the client words.
extern "C" {
int sceSifSearchModuleByName(const char* name);
int sceSifBindRpc(void* client, unsigned server, int mode);
int printf(const char* format, ...);

static unsigned m_rpcCd[16];
static volatile int m_rpcRunning;  // write-only here; volatile keeps the store

void initEzNetCnf() {
    const char* const name = "eznetcnf";
    const int module = sceSifSearchModuleByName(name);
    if (module < 0) printf("\"%s\" module not found (%d)\n", name, module);
    do {
        if (sceSifBindRpc(m_rpcCd, 0x75499128u, 0) < 0) printf("could not bind to eznetcnf service\n");
        if (m_rpcCd[9] != 0) break;
        volatile int remaining = 0x00010000;
        do {
            remaining = remaining - 1;
        } while (remaining != -1);
    } while (true);
    m_rpcRunning = 1;
}
}
