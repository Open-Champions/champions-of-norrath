// Observed initEzNetCtl at 0x010bfb50, 200 bytes, in the .srs overlay range.
// It looks up the IOP module "eznetctl" with sceSifSearchModuleByName and, when
// that result is negative, prints it with the module name. It then binds the
// local client m_cd (0x011e8500, 40 bytes) to server 0x75488909, printing a
// message when the bind is negative. A zero word at client offset 36 means
// the server has not answered: a countdown from 0x00010000 to -1 runs and the bind
// is repeated. Once that word is nonzero it stores 1 in the local flag
// m_rpcRunning (0x01192c6c). A failed module lookup or failed bind does not
// stop the sequence; only the word at offset 36 ends the loop.
extern "C" {
int sceSifSearchModuleByName(const char* name);
int sceSifBindRpc(void* client, unsigned server, int mode);
int printf(const char* format, ...);

static unsigned m_cd[10];
static volatile int m_rpcRunning;  // write-only here; volatile keeps the store

void initEzNetCtl() {
    const char* const name = "eznetctl";
    const int module = sceSifSearchModuleByName(name);
    if (module < 0) printf("\"%s\" module not found (%d)\n", name, module);
    do {
        if (sceSifBindRpc(m_cd, 0x75488909u, 0) < 0) printf("could not bind to eznetctl service\n");
        if (m_cd[9] != 0) break;
        volatile int remaining = 0x00010000;
        do {
            remaining = remaining - 1;
        } while (remaining != -1);
    } while (true);
    m_rpcRunning = 1;
}
}
