// Observed FlushIopCommand__Fb at 0x01126f98, 164 bytes, pinned SLUS_205.65. One argument: a
// flag. It works on four file-local objects of the original (symbol names from the executable's
// table, none defined here): the RPC client sSifClient (0x011ee108), the 512-byte send
// buffer sSend (0x011edb00), the 512-byte receive buffer sRecv (0x011edd00) and the word
// sFunctionIndex (0x011edf00).
//   1. sceSifCheckStatRpc(&sSifClient) nonzero (a call is still running): return -1.
//   2. Word 0 of sSend zero (nothing queued): return -2.
//   3. Otherwise sceSifCallRpc(&sSifClient, 0x80f0, flag != 0, sSend, 512, sRecv, 512, 0, 0).
//      The flag goes in as the call's mode argument, so a nonzero flag asks for the blocking
//      form and zero for the one that returns at once.
//   4. Word 0 of sSend is cleared, then sFunctionIndex is read and cleared, and the value read
//      is returned. The call's own return value is not used.
// Inferred: sSend word 0 is a queued-command count that SetIopFunction__FUii (not here) bumps and
// sFunctionIndex is the cursor it advances; neither is proven by this function alone.
// Unknown: the meaning of function number 0x80f0 beyond being the sound IOP server's batch call.
// Nothing was executed on the EE.
extern "C" {
struct SifRpcClient;
int sceSifCheckStatRpc(SifRpcClient* client);
int sceSifCallRpc(SifRpcClient* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*end_function)(void*), void* end_argument);
extern SifRpcClient sSifClient;
extern int sSend[128];
extern int sRecv[128];
extern int sFunctionIndex;

int FlushIopCommand__Fb(bool wait) {
    if (sceSifCheckStatRpc(&sSifClient) != 0) return -1;
    if (sSend[0] == 0) return -2;
    sceSifCallRpc(&sSifClient, 0x80f0, wait ? 1 : 0, sSend, 512, sRecv, 512, 0, 0);
    sSend[0] = 0;
    const int index = sFunctionIndex;
    sFunctionIndex = 0;
    return index;
}
}
