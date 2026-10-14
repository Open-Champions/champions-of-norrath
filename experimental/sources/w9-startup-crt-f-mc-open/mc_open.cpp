/* Observed sceMcOpen at 0x002e0f78, 292 bytes. A zero word at offset 36 of
 * the client mcClientID (0x008e6b40) returns -100 with nothing else done.
 * PollSema on semaidRegFunc (0x00446074) returning a negative value returns
 * -200. A null or empty name signals semaidRegFunc and returns -210.
 * Otherwise at most 1023 bytes of the name are copied (strncpy) to offset 20
 * of the 1044-byte block sifParamFname (0x008e6bf0), byte 1043 of the block is
 * cleared, and the block gets word 0 = port, word 1 = slot, word 2 = mode.
 * sceSifCallRpc runs on the client with command 2, mode 1, the block as the
 * 1044-byte send area, retval (0x008e8100) as the 4-byte receive area,
 * mceSifRpcEndFunc (0x002e0de0) as completion function and no argument. A
 * zero result stores 2 in mcRunCmdNo (0x00446070); any other result signals
 * semaidRegFunc. The call result is returned.
 */
extern "C" {
extern int mcClientID[];
extern int sifParamOrd[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;
extern unsigned char sifParamFname[];

int PollSema(int id);
int SignalSema(int id);
char* strncpy(char* destination, const char* source, unsigned long count);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceSifRpcEndFunc(void* argument);

int sceMcOpen(int port, int slot, const char* name, int mode) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    if (name == 0 || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    strncpy(reinterpret_cast<char*>(sifParamFname) + 20, name, 1023);
    int* block = reinterpret_cast<int*>(sifParamFname);
    block[0] = port;
    block[2] = mode;
    block[1] = slot;
    sifParamFname[1043] = 0;
    const int result = sceSifCallRpc(mcClientID, 2, 1, sifParamFname, 1044, retval, 4, mceSifRpcEndFunc, 0);
    if (result == 0)
        mcRunCmdNo = 2;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
