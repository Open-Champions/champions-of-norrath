/* Observed sceMcGetInfo at 0x002e16c0, 380 bytes. A zero word at offset 36
 * of the client mcClientID (0x008e6b40) returns -100 with nothing else done.
 * PollSema on semaidRegFunc (0x00446074) returning a negative value returns
 * -200. Otherwise the parameter block sifParamOrd (0x008e6bc0, 48 bytes) is
 * filled: word 1 the port, word 2 the slot, word 7 the address of
 * sifParamNext (0x008e7040), and words 5, 4 and 3 each 1 when the third,
 * fourth and fifth arguments are nonnull and 0 otherwise. The three pointer
 * arguments are stored in the three words at 0x008e6b68, 0x008e6b6c and
 * 0x008e6b70. The first 192 bytes of sifParamNext are written back from the
 * data cache. sceSifCallRpc then runs on the client with command 1, mode 1,
 * the parameter block as the 48-byte send area, retval (0x008e8100) as the
 * 4-byte receive area, mceGetInfoApdx (0x002e1668) as completion function and
 * sifParamNext as its argument. A zero result stores 1 in mcRunCmdNo
 * (0x00446070); any other result signals semaidRegFunc. The call result is
 * returned. Word 0 of the parameter block is not written by this function.
 * What the three pointer words are used for after the call is not
 * established here.
 */
extern "C" {
extern int mcClientID[];
extern int sifParamOrd[];
extern int retval[];
extern int mcRunCmdNo;
extern int semaidRegFunc;
extern int norrath_libmc_info_type_pointer_word_view[];
extern int norrath_libmc_info_free_pointer_word_view[];
extern int norrath_libmc_info_format_pointer_word_view[];
extern int norrath_libmc_param_next_view[];

int PollSema(int id);
int SignalSema(int id);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void (*done)(void*), void* done_arg);
void mceGetInfoApdx(void* argument);

static int word_of(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int sceMcGetInfo(int port, int slot, int* type, int* free_clusters, int* format) {
    if (mcClientID[9] == 0) return -100;
    if (PollSema(semaidRegFunc) < 0) return -200;
    sifParamOrd[1] = port;
    sifParamOrd[2] = slot;
    sifParamOrd[7] = word_of(norrath_libmc_param_next_view);
    sifParamOrd[5] = type != 0;
    sifParamOrd[4] = free_clusters != 0;
    sifParamOrd[3] = format != 0;
    norrath_libmc_info_type_pointer_word_view[0] = word_of(type);
    norrath_libmc_info_free_pointer_word_view[0] = word_of(free_clusters);
    norrath_libmc_info_format_pointer_word_view[0] = word_of(format);
    sceSifWriteBackDCache(norrath_libmc_param_next_view, 192);
    const int result = sceSifCallRpc(mcClientID, 1, 1, sifParamOrd, 48, retval, 4, mceGetInfoApdx,
                                     norrath_libmc_param_next_view);
    if (result == 0)
        mcRunCmdNo = 1;
    else
        SignalSema(semaidRegFunc);
    return result;
}
}
