// Observed sceMcChdir at 0x002e1a90, 320 bytes. The arguments are port, slot, a name and
// a result pointer. -100 when the word at offset 36 of mcClientID (0x008e6b40) is zero;
// -200 when PollSema on semaidRegFunc (0x00446074) is negative; a null or empty name
// signals semaidRegFunc and returns -210. Otherwise the block sifParamFname (0x008e6bf0)
// gets word 0 the port, word 1 the slot and word 4 the address of currentDir
// (0x008e7100), then strncpy(block+20, name, 1023) and byte 1043 zeroed. Words 2 and 3
// are not written. The 1024 bytes of currentDir are written back from the data cache.
// sceSifCallRpc runs on mcClientID with command 12, mode 1, the block as the 1044-byte
// send area, retval (0x008e8100) as the 4-byte receive area, mceStorePwd (0x002e1a00) as
// completion function and the caller's result pointer as its argument. A zero result
// stores 12 in mcRunCmdNo (0x00446070); any other result signals semaidRegFunc. The call
// result is returned.
// Inferred: the completion function copies the new working directory out through the
// result pointer, from its name; its body is not read here.
extern "C" {
extern int mcClientID[];
extern int sifParamFname[];
extern unsigned char currentDir[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;

int PollSema(int id);
int SignalSema(int id);
char* strncpy(char* destination, const char* source, __SIZE_TYPE__ count);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceStorePwd(void* argument);

static int word_of(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int sceMcChdir(int port, int slot, const char* name, void* result_pointer) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    if (name == nullptr || *name == 0) {
        SignalSema(semaidRegFunc);
        return -210;
    }
    sifParamFname[0] = port;
    sifParamFname[4] = word_of(currentDir);
    sifParamFname[1] = slot;
    strncpy(reinterpret_cast<char*>(sifParamFname) + 20, name, 1023u);
    reinterpret_cast<char*>(sifParamFname)[1043] = 0;
    sceSifWriteBackDCache(currentDir, 1024);
    const int result = sceSifCallRpc(mcClientID, 12u, 1u, sifParamFname, 1044, retval, 4, mceStorePwd, result_pointer);
    if (result == 0)
        mcRunCmdNo = 12;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
