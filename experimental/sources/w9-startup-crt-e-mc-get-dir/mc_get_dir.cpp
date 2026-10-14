// Observed sceMcGetDir at 0x002e18b0, 332 bytes. The arguments are port, slot, a name,
// a mode word, a maximum entry count and a table pointer. -100 when the word at offset
// 36 of mcClientID (0x008e6b40) is zero; -200 when PollSema on semaidRegFunc
// (0x00446074) is negative; a null or empty name signals semaidRegFunc and returns
// -210. Otherwise the 1044-byte block sifParamFname (0x008e6bf0) is filled: word 0 the
// port, word 1 the slot, word 2 the mode, word 3 the maximum count, word 4 the table
// address, then strncpy(block+20, name, 1023) and byte 1043 zeroed. A maximum count that
// is not negative writes back count*64 bytes of the table from the data cache. Then
// sceSifCallRpc runs on mcClientID with command 13, mode 1, the block as the 1044-byte
// send area, retval (0x008e8100) as the 4-byte receive area and mceSifRpcEndFunc
// (0x002e0de0) with a zero argument. A zero result stores 13 in mcRunCmdNo (0x00446070);
// any other result signals semaidRegFunc. The call result is returned.
// Inferred: 64 bytes per table entry follows from the shift; the entry layout is not read.
extern "C" {
extern int mcClientID[];
extern int sifParamFname[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;

int PollSema(int id);
int SignalSema(int id);
char* strncpy(char* destination, const char* source, __SIZE_TYPE__ count);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceSifRpcEndFunc(void* argument);

static int word_of(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int sceMcGetDir(int port, int slot, const char* name, unsigned mode, int max_entries, void* table) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    if (name == nullptr || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    sifParamFname[0] = port;
    sifParamFname[1] = slot;
    sifParamFname[2] = static_cast<int>(mode);
    sifParamFname[3] = max_entries;
    sifParamFname[4] = word_of(table);
    strncpy(reinterpret_cast<char*>(sifParamFname) + 20, name, 1023u);
    reinterpret_cast<char*>(sifParamFname)[1043] = 0;
    if (max_entries >= 0) sceSifWriteBackDCache(table, max_entries << 6);
    const int result = sceSifCallRpc(mcClientID, 13u, 1u, sifParamFname, 1044, retval, 4, mceSifRpcEndFunc, nullptr);
    if (result == 0)
        mcRunCmdNo = 13;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
