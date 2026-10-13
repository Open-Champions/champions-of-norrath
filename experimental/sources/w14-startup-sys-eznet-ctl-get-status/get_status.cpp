// Observed ezNetCtlGetStatus at 0x010bfcf0, 160 bytes, in the .srs overlay range, in the eznetctl client
// library file ("../eznetcnf/libezctl.c" is the source name the routine itself prints). Argument: a pointer to
// a 272-byte status record.
//  1. A zero m_rpcRunning (0x01192c6c, set by initEzNetCtl, cleared by exitEzNetCtl) prints the text
//     "**** eznetctl RPC server not currently running\n" with printf and returns -1.
//  2. Otherwise sceSifCallRpc(m_cd, 3, 0, status, 272, status, 272, 0, 0): the record is both the send and the
//     receive buffer, function 3, mode 0, no completion function. The call's own return value is not used.
//  3. The first word of the record is then read. When it is zero the routine returns zero. Otherwise it prints
//     "%s> " with the file name above, then "ezNetCtlGetStatus returned %d\n" with that word, and returns it.
// Inferred: the first word of the record is an error or status code that the IOP fills, zero meaning none.
// Unknown: the layout of the rest of the record, and the meaning of function 3 beyond being the status query.
// Callers: the connect window drawing code. m_cd and m_rpcRunning are image data, not defined here.
extern "C" {
extern int m_rpcRunning;
extern unsigned char m_cd[];
int printf(const char* format, ...);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

int ezNetCtlGetStatus(int* status) {
    if (m_rpcRunning == 0) {
        printf("**** eznetctl RPC server not currently running\n");
        return -1;
    }
    sceSifCallRpc(m_cd, 3u, 0u, status, 272, status, 272, nullptr, nullptr);
    if (status[0] != 0) {
        printf("%s> ", "../eznetcnf/libezctl.c");
        printf("ezNetCtlGetStatus returned %d\n", status[0]);
    }
    return status[0];
}
}
