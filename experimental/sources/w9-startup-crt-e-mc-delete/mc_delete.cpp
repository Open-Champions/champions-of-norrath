// Observed sceMcDelete at 0x002e1ca0, 284 bytes. The arguments are port, slot and a
// name. -100 when the word at offset 36 of mcClientID (0x008e6b40) is zero; -200 when
// PollSema on semaidRegFunc (0x00446074) is negative. A null name or an empty one
// signals semaidRegFunc and returns -210. Otherwise the 1044-byte block sifParamFname
// (0x008e6bf0) is filled: word 0 the port, word 1 the slot, word 2 zero, the name copied
// with strncpy(block+20, name, 1023), and byte 1043 set to zero afterwards. sceSifCallRpc
// runs on mcClientID with command 15, mode 1, that block as the 1044-byte send area,
// retval (0x008e8100) as the 4-byte receive area and mceSifRpcEndFunc (0x002e0de0) with a
// zero argument. A zero result stores 15 in mcRunCmdNo (0x00446070); any other result
// signals semaidRegFunc. The call result is returned.
// The 1044 bytes are 20 header bytes plus 1024 name bytes; the span to the next symbol
// (sifParamNext, 0x008e7040) is 0x450 bytes, which is larger.
extern "C" {
extern int mcClientID[];
extern int sifParamFname[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;

int PollSema(int id);
int SignalSema(int id);
char* strncpy(char* destination, const char* source, unsigned count);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceSifRpcEndFunc(void* argument);

int sceMcDelete(int port, int slot, const char* name) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    if (name == nullptr || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    strncpy(reinterpret_cast<char*>(sifParamFname) + 20, name, 1023u);
    sifParamFname[0] = port;
    sifParamFname[1] = slot;
    reinterpret_cast<char*>(sifParamFname)[1043] = 0;
    sifParamFname[2] = 0;
    const int result = sceSifCallRpc(mcClientID, 15u, 1u, sifParamFname, 1044, retval, 4, mceSifRpcEndFunc, nullptr);
    if (result == 0)
        mcRunCmdNo = 15;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
