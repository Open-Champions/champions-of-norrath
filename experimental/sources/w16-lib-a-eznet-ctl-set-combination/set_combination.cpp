// Observed: ezNetCtlSetCombination at 0x010bfc28, 192 bytes, pinned SLUS_205.65, in the eznetctl client file
// (the file name "../eznetcnf/libezctl.c" is printed by the routine itself). With (name, combination):
//  1. A zero m_rpcRunning (0x01192c6c) prints "**** eznetctl RPC server not currently running\n" and returns -1.
//  2. Otherwise strcpy(m_buf + 4, name) and strcpy(m_buf + 260, combination), then
//     sceSifCallRpc(m_cd, 2, 0, m_buf, 292, m_buf, 292, 0, 0). The call result is not used.
//  3. Always (the check is not conditional on the reply): printf("%s> ", file name) and
//     printf("ezNetCtlSetCombination returned %d\n", m_buf word 0). The function returns that word.
// Inferred: the 292-byte block is a word-0 status plus a 256-byte name field at +4 and a 32-byte-or-larger combination
// field at +260; the caller passes "mc0:" and a combination record, so the server is told which memory-card file to use.
// Unknown: the exact field sizes (only the offsets used are known) and what a nonzero word 0 means beyond the error text.
extern "C" {
extern int m_rpcRunning;
extern unsigned char m_cd[];
extern unsigned char m_buf[];
int printf(const char* format, ...);
char* strcpy(char* destination, const char* source);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

int ezNetCtlSetCombination(const char* name, const char* combination) {
    if (m_rpcRunning == 0) {
        printf("**** eznetctl RPC server not currently running\n");
        return -1;
    }
    strcpy(reinterpret_cast<char*>(m_buf + 4), name);
    strcpy(reinterpret_cast<char*>(m_buf + 260), combination);
    sceSifCallRpc(m_cd, 2u, 0u, m_buf, 292, m_buf, 292, nullptr, nullptr);
    const int* words = reinterpret_cast<const int*>(m_buf);
    printf("%s> ", "../eznetcnf/libezctl.c");
    printf("ezNetCtlSetCombination returned %d\n", words[0]);
    return words[0];
}
}
