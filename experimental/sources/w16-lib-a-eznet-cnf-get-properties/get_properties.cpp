// Observed: ezNetCnfGetProperties at 0x010bf0f0, 212 bytes, pinned SLUS_205.65, in the eznetcnf client file
// (the routine prints the source name "../eznetcnf/libezcnf.c"). With (name, combination, properties):
//  1. A zero m_rpcRunning (the local word at 0x01192c68, not the one at 0x01192c6c) prints
//     "**** eznetcnf RPC server not currently running\n" and returns -1.
//  2. Otherwise strcpy(m_rpcBuf + 4, name) and strcpy(m_rpcBuf + 260, combination); m_rpcBuf word 0 = properties (the
//     address of the caller's 4928-byte block, stored as a 32-bit word); then
//     sceSifCallRpc(m_rpcCd, 2, 0, m_rpcBuf, 292, m_rpcBuf, 292, 0, 0). The call result is not used.
//  3. Always: printf("%s> ", file name) and printf("ezNetCnfGetNetcnfifData returned %d\n", m_rpcBuf word 0).
//     The function returns that word.
// Inferred: the same request layout as ezNetCtlSetCombination (name at +4, combination at +260) with the destination block
// for the server's reply passed in word 0; the reply word 0 then holds the status.
// Unknown: the layout of the properties block and the field sizes. The text calls itself ezNetCnfGetNetcnfifData.
// Storage: m_rpcCd (0x011e8380, 64 bytes by label spacing), m_rpcBuf (0x011e83c0, 320 bytes by label spacing) and
// m_rpcRunning (0x01192c68) are file-local in the image; this unit keeps file-local copies under the image names, as
// initEzNetCnf (w9-startup-crt-d-init-ez-net-cnf) and exitEzNetCnf do. Until those units share one definition the
// running flag and client set up by initEzNetCnf are not the ones read here.
extern "C" {
int printf(const char* format, ...);
char* strcpy(char* destination, const char* source);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

static volatile int m_rpcRunning __asm__("m_rpcRunning");
alignas(8) static unsigned char m_rpcCd[64] __asm__("m_rpcCd");
alignas(8) static unsigned char m_rpcBuf[320] __asm__("m_rpcBuf");

int ezNetCnfGetProperties(const char* name, const char* combination, void* properties) {
    if (m_rpcRunning == 0) {
        printf("**** eznetcnf RPC server not currently running\n");
        return -1;
    }
    strcpy(reinterpret_cast<char*>(m_rpcBuf + 4), name);
    strcpy(reinterpret_cast<char*>(m_rpcBuf + 260), combination);
    int* words = reinterpret_cast<int*>(m_rpcBuf);
    words[0] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(properties));
    sceSifCallRpc(m_rpcCd, 2u, 0u, m_rpcBuf, 292, m_rpcBuf, 292, nullptr, nullptr);
    printf("%s> ", "../eznetcnf/libezcnf.c");
    printf("ezNetCnfGetNetcnfifData returned %d\n", words[0]);
    return words[0];
}
}
