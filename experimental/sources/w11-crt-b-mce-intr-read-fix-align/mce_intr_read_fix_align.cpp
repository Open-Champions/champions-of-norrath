// Observed mceIntrReadFixAlign at 0x002e1278, 152 bytes: the completion function the memory-card
// read call hands to sceSifCallRpc (mc_read passes it with the parameter block as argument). The
// argument is an address; the code works on it with the uncached bit (0x20000000) set. The block
// holds, as words: +0 a byte count A, +4 a byte count B, +8 a destination address for A, +12 a
// destination address for B, then +16 up to 64 bytes of data for A and +80 the data for B.
//   A not zero and positive: A bytes are copied from block + 16 to the address at +8, one byte at
//     a time; the count is read again after every byte (so a destination that overlaps the block
//     changes the loop).
//   B not zero and positive: the same with block + 80 and the address at +12.
//   Then iSignalSema(semaid_timerfunc) is tail-called and its result returned.
// The counts are read fresh from the uncached block, A first. Unknown: why the data are staged in
// the block (alignment of the transfer is assumed from the name only); the maximum counts. Nothing
// was executed on the EE.
extern "C" {
extern int semaid_timerfunc;
int iSignalSema(int semaphore);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_host_destination(unsigned word);  // host check: maps a 32-bit address word to a host buffer
#endif

static unsigned char* destination_of(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_host_destination(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

int mceIntrReadFixAlign(void* argument) {
#if defined(NORRATH_HOST_CONTRACT)
    unsigned char* block = static_cast<unsigned char*>(argument);
#else
    unsigned char* block = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(argument)) | 0x20000000u));
#endif
    volatile int* words = reinterpret_cast<volatile int*>(block);
    if (words[0] != 0) {
        unsigned char* to = destination_of(static_cast<unsigned>(words[2]));
        for (int i = 0; i < words[0]; ++i) {
            to[i] = block[16 + i];
        }
    }
    if (words[1] != 0) {
        unsigned char* to = destination_of(static_cast<unsigned>(words[3]));
        for (int i = 0; i < words[1]; ++i) {
            to[i] = block[80 + i];
        }
    }
    return iSignalSema(semaid_timerfunc);
}
}
