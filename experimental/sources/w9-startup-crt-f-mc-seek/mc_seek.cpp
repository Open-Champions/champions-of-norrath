/* Observed sceMcSeek at 0x002e1198, 220 bytes. A zero word at offset 36 of
 * the client mcClientID (0x008e6b40) returns -100 with nothing else done.
 * PollSema on semaidRegFunc (0x00446074) returning a negative value returns
 * -200. Otherwise the parameter block sifParamOrd (0x008e6bc0, 48 bytes) gets
 * word 0 = the file descriptor, word 4 = the offset and word 5 = the origin;
 * sceSifCallRpc runs on the client with command 4, mode 1, the parameter block
 * as the 48-byte send area, retval (0x008e8100) as the 4-byte receive area,
 * mceSifRpcEndFunc (0x002e0de0) as completion function and no argument. A
 * zero result stores 4 in mcRunCmdNo (0x00446070); any other result signals
 * semaidRegFunc. The call result is returned.
 */
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

int sceMcSeek(int fd, int offset, int origin) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[0] = fd;
    sifParamOrd[4] = offset;
    sifParamOrd[5] = origin;
    const int result = sceSifCallRpc(mcClientID, 4, 1, sifParamOrd, 48, retval, 4, mceSifRpcEndFunc, 0);
    if (result == 0)
        mcRunCmdNo = 4;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
