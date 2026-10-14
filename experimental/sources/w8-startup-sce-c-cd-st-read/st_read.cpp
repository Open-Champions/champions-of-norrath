// Observed sceCdStRead at 0x002e41e0, 392 bytes. Arguments: block count,
// destination, mode, and a pointer that receives the error value. With
// SCE_CD_debug positive it prints "sceCdStRead call read size= %d mode= %d\n"
// with the count and mode. A zero stm_status (0x00447c34) returns 0 without
// writing the error pointer. The destination is written back from the data
// cache for count << 11 bytes. Each sceCdStream call uses argument 0, command
// 2 and the dum_mode bytes; its result carries the transferred block count in
// the low 16 bits and an error value in the high 16 bits.
// Mode 0 makes one call for the whole count, stores the error value and returns
// the block count. Any other mode loops: each call asks for the remaining count at
// the destination advanced by the blocks already read; when the error value
// is nonzero it is remembered and, with debug on, "sceCdStRead BLK Read cur_size=
// %d read_size= %d req_size= %d err 0x%x\n" prints; when the error is zero and
// no block was read DelayThread(600) runs. The loop stops when the total
// equals the count, or when a call returns a nonzero error and no blocks. It
// prints "sceCdStRead BLK Read Ended\n" under debug, stores the last nonzero
// error seen (0 if none) and returns the total.
extern "C" {
extern int SCE_CD_debug;
extern int stm_status;
extern unsigned char dum_mode[];
unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode);
void sceSifWriteBackDCache(void* address, int bytes);
void DelayThread(int microseconds);
int scePrintf(const char* format, ...);
int sceCdStRead(int blocks, unsigned char* destination, int mode, int* error) {
    if (SCE_CD_debug > 0) scePrintf("sceCdStRead call read size= %d mode= %d\n", blocks, mode);
    if (stm_status == 0) return 0;
    sceSifWriteBackDCache(destination, blocks << 11);
    if (mode == 0) {
        const unsigned result = sceCdStream(0u, static_cast<unsigned>(blocks), destination, 2u, dum_mode);
        *error = static_cast<int>(result >> 16);
        return static_cast<int>(result & 0xffffu);
    }
    int total = 0;
    int last_error = 0;
    do {
        const unsigned result = sceCdStream(0u, static_cast<unsigned>(blocks - total), destination + (total << 11), 2u, dum_mode);
        const int read = static_cast<int>(result & 0xffffu);
        const int status = static_cast<int>(result >> 16);
        total += read;
        if (status != 0) {
            last_error = status;
            if (SCE_CD_debug > 0)
                scePrintf("sceCdStRead BLK Read cur_size= %d read_size= %d req_size= %d err 0x%x\n", total, read, blocks, status);
            if (read == 0) break;
        } else if (read == 0) {
            DelayThread(600);
        }
    } while (total != blocks);
    if (SCE_CD_debug > 0) scePrintf("sceCdStRead BLK Read Ended\n");
    *error = last_error;
    return total;
}
}
