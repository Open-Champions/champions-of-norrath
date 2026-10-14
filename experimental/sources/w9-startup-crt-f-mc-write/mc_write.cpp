/* Observed sceMcWrite at 0x002e1428, 372 bytes. A zero word at offset 36 of
 * the client mcClientID (0x008e6b40) returns -100 with nothing else done.
 * PollSema on semaidRegFunc (0x00446074) returning a negative value returns
 * -200. Otherwise the parameter block sifParamOrd (0x008e6bc0, 48 bytes) gets
 * word 0 = the file descriptor. A size of 16 or less is sent as a head only:
 * word 5 = size, words 6 and 3 = 0. A larger size is split at the next
 * 16-byte boundary of the source address: head = ((address - 1) & ~15) -
 * (address - 16) bytes (0 when already aligned), word 5 = head, word 6 =
 * address + head, word 3 = size - head. The head bytes are copied to offset 32
 * of the block (word 5 bytes, a pre-test loop), FlushCache(0) runs, and
 * sceSifCallRpc runs on the client with command 6, mode 1, the block as the
 * 48-byte send area, retval (0x008e8100) as the 4-byte receive area,
 * mceSifRpcEndFunc (0x002e0de0) as completion function and no argument. A
 * zero result stores 6 in mcRunCmdNo (0x00446070); any other result signals
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
int FlushCache(int mode);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceSifRpcEndFunc(void* argument);

static int word_of(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int sceMcWrite(int fd, const void* buffer, int size) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[0] = fd;
    if (size < 17) {
        sifParamOrd[5] = size;
        sifParamOrd[6] = 0;
        sifParamOrd[3] = 0;
    } else {
        const int address = word_of(buffer);
        const int head = ((address - 1) & ~15) - (address - 16);
        sifParamOrd[6] = address + head;
        sifParamOrd[3] = size - head;
        sifParamOrd[5] = head;
    }
    if (sifParamOrd[5] != 0) {
        const unsigned char* source = static_cast<const unsigned char*>(buffer);
        unsigned char* destination = reinterpret_cast<unsigned char*>(sifParamOrd) + 32;
        unsigned index = 0;
        do {
            destination[index] = source[index];
            ++index;
        } while (index < static_cast<unsigned>(sifParamOrd[5]));
    }
    FlushCache(0);
    const int result = sceSifCallRpc(mcClientID, 6, 1, sifParamOrd, 48, retval, 4, mceSifRpcEndFunc, 0);
    if (result == 0)
        mcRunCmdNo = 6;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
