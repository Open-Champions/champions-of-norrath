// Observed sceMcFormat at 0x002e1bd0, 204 bytes. Same shape as sceMcClose: -100 when
// the word at offset 36 of mcClientID (0x008e6b40) is zero, -200 when PollSema on
// semaidRegFunc (0x00446074) is negative. Otherwise word 1 of sifParamOrd (0x008e6bc0)
// receives the port and word 2 the slot, and sceSifCallRpc runs on mcClientID with
// command 16, mode 1, sifParamOrd as the 48-byte send area, retval (0x008e8100) as the
// 4-byte receive area and mceSifRpcEndFunc (0x002e0de0) with a zero argument. A zero
// result stores 16 in mcRunCmdNo (0x00446070); any other result signals semaidRegFunc.
// The call result is returned. Word 0 of the block is not written.
extern "C" {
extern int mcClientID[];
extern int sifParamOrd[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;

int PollSema(int id);
int SignalSema(int id);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceSifRpcEndFunc(void* argument);

int sceMcFormat(int port, int slot) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[1] = port;
    sifParamOrd[2] = slot;
    const int result = sceSifCallRpc(mcClientID, 16u, 1u, sifParamOrd, 48, retval, 4, mceSifRpcEndFunc, nullptr);
    if (result == 0)
        mcRunCmdNo = 16;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
