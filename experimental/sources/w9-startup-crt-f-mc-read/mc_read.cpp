/* Observed sceMcRead at 0x002e1310, 276 bytes. A zero word at offset 36 of
 * the client mcClientID (0x008e6b40) returns -100 with nothing else done.
 * PollSema on semaidRegFunc (0x00446074) returning a negative value returns
 * -200. Otherwise the parameter block sifParamOrd (0x008e6bc0, 48 bytes) gets
 * word 0 = the file descriptor, word 3 = the size, word 6 = the destination
 * address and word 7 = the address of sifParamNext (0x008e7040). The
 * destination range (address, size) and the first 192 bytes of sifParamNext
 * are written back from the data cache, in that order. sceSifCallRpc then
 * runs on the client with command 5, mode 1, the parameter block as the
 * 48-byte send area, retval (0x008e8100) as the 4-byte receive area,
 * mceIntrReadFixAlign (0x002e1278) as completion function and sifParamNext as
 * its argument. A zero result stores 5 in mcRunCmdNo (0x00446070); any other
 * result signals semaidRegFunc. The call result is returned.
 */
extern "C" {
extern int mcClientID[];
extern int sifParamOrd[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;
extern int norrath_libmc_param_next_view[];

int PollSema(int id);
int SignalSema(int id);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceIntrReadFixAlign(void* argument);

static int word_of(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int sceMcRead(int fd, void* buffer, int size) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[0] = fd;
    sifParamOrd[7] = word_of(norrath_libmc_param_next_view);
    sifParamOrd[6] = word_of(buffer);
    sifParamOrd[3] = size;
    sceSifWriteBackDCache(buffer, size);
    sceSifWriteBackDCache(norrath_libmc_param_next_view, 192);
    const int result = sceSifCallRpc(mcClientID, 5, 1, sifParamOrd, 48, retval, 4, mceIntrReadFixAlign,
                                     norrath_libmc_param_next_view);
    if (result == 0)
        mcRunCmdNo = 5;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
