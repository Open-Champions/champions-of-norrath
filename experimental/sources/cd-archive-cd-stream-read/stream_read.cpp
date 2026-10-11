// Observed sceCdStRead at 0x002e41e0. stm_status at 0x00447c34 must be nonzero.
// The destination is written back for blocks << 11 bytes. Mode 0 makes one
// sceCdStream call (0, blocks, buffer, 2, dum_mode) and splits the result into
// the low half (sectors returned) and the high half (stored through error).
// A nonzero mode repeats that call, advancing by the low half, delaying 600
// microseconds when a call returns no sectors and no error. A nonzero high
// half is kept and ends the loop when that call also transferred nothing.
extern "C" {
extern int SCE_CD_debug;
extern int stm_status;
extern unsigned char dum_mode[];
int norrath_sce_printf(const char* format, ...);
void DelayThread(int microseconds);
void sceSifWriteBackDCache(void* address, int bytes);
unsigned sceCdStream(int argument, int count, void* buffer, int command, void* mode);
int norrath_archive_cd_stream_read(unsigned blocks, void* buffer, int mode, int* error) {
    if (SCE_CD_debug > 0) norrath_sce_printf("sceCdStRead call read size= %d mode= %d\n", blocks);
    if (stm_status == 0) return 0;
    sceSifWriteBackDCache(buffer, static_cast<int>(blocks << 11));
    if (mode == 0) {
        const unsigned result = sceCdStream(0, static_cast<int>(blocks), buffer, 2, dum_mode);
        const unsigned transferred = result & 0xffffu;
        *error = static_cast<int>(result >> 16);
        return static_cast<int>(transferred);
    }
    unsigned progress = 0;
    unsigned transferred = 0;
    unsigned failure = 0;
    unsigned kept = 0;
    bool entered = false;
    for (;;) {
        if (entered) {
            if (failure != 0u && transferred == 0u) break;
        }
        entered = true;
        const unsigned result = sceCdStream(0, static_cast<int>(blocks - progress),
                                            static_cast<unsigned char*>(buffer) + (progress << 11), 2, dum_mode);
        transferred = result & 0xffffu;
        failure = result >> 16;
        progress += transferred;
        if (failure != 0u) {
            kept = failure;
            if (SCE_CD_debug > 0) {
                norrath_sce_printf("sceCdStRead BLK Read cur_size= %d read_size= %d req_size= %d err 0x%x\n",
                                   progress, transferred, blocks, failure);
            }
        } else if (transferred == 0u) {
            DelayThread(600);
        }
        if (progress != blocks) continue;
        break;
    }
    if (SCE_CD_debug > 0) norrath_sce_printf("sceCdStRead BLK Read Ended\n");
    *error = static_cast<int>(kept);
    return static_cast<int>(progress);
}
}
