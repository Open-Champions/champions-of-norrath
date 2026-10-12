// Observed sceMcClose at 0x002e10d8, 184 bytes. The argument is the file descriptor.
// A zero word at offset 36 of mcClientID (0x008e6b40) returns -100. PollSema on
// semaidRegFunc (0x00446074) below zero returns -200. Otherwise word 0 of sifParamOrd
// (0x008e6bc0) is set to the descriptor and sceSifCallRpc runs on mcClientID with
// command 3, mode 1, sifParamOrd as the 48-byte send area, retval (0x008e8100) as the
// 4-byte receive area and the function at 0x002e0de0 (mceSifRpcEndFunc) as completion
// function with a zero argument. A zero result stores 3 in mcRunCmdNo (0x00446070); any
// other result signals semaidRegFunc. The call result is returned.
// Unknown: what the completion function does with the result.
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

int sceMcClose(int fd) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[0] = fd;
    const int result = sceSifCallRpc(mcClientID, 3u, 1u, sifParamOrd, 48, retval, 4, mceSifRpcEndFunc, nullptr);
    if (result == 0)
        mcRunCmdNo = 3;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
