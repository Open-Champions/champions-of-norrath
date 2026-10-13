// Observed: IdleTransfer__13VISoundDevice at 0x01125f70, 480 bytes. It returns -1 on one path and 0 on every other. Words are
// indexed by byte offset from the device; the fields are those BeginAdpcmTransfer__13VISoundDevicePPvUiiPi (0x01126f30) and
// BeginXmTransfer__13VISoundDevicePPvUiUiiPi (0x01126f60) fill, plus a few they leave alone.
//  A. If the word at +3408 (a DMA id) is nonzero:
//     - r = sceSifDmaStat(id). If r >= 0, return 0 (still running).
//     - Otherwise store 0 at +3408. If the word at +3388 is not -1: sIopFunctionData[0] = word at +3404 and sIopFunctionData[1] =
//       the word at +3388, then +3388 += word at +3404, and SetIopFunction__FUii(1, 2) is called (sIopFunctionData is the 0x011edf08
//       staging array shared with StopChannels__13VISoundDeviceUl).
//     - If the remaining-byte word at +3384 is not zero, return 0.
//     - Otherwise (finished): store 1 at +3412; if the pointer at +3420 is nonzero free it with __builtin_delete and clear it; if the word
//       at +3392 is not -1: sIopFunctionData[0] = word at +3396, sIopFunctionData[1] = word at +3400 and call SetIopFunction__FUii(7, 2).
//       If the pointer at +3416 is nonzero store 1 through it. Return 0.
//  B. Else, with remaining = signed word at +3384:
//     - remaining == 0: return 0. remaining < 0: store 0 at +3384, return 0.
//     - If word 1 of the array at 0x011edd00 (sRecv) equals 1, return 0.
//     - chunk = 0x10000 if remaining > 0xffff, else (remaining + 63) rounded down to a multiple of 64.
//       FlushCache(0). Build four words on the stack: [0] = word at +3380; [1] = word at +3392 when the word at +3388 is -1, else the
//       word at +3368; [2] = chunk; [3] = 0. FlushCache(0) is called again after [0] and [1] are loaded. id = sceSifSetDma(words, 1)
//       is stored at +3408.
//     - If id is 0, print "Sample data transfer failed..\n" with printf and return -1.
//     - Otherwise store chunk at +3404, +3380 += chunk, +3384 = remaining - chunk; if the word at +3388 is -1 also +3392 += chunk.
//       If the new remaining count is negative store 0 at +3384. Return 0.
// Inferred: this is the per-frame pump of a sample upload: +3380 source pointer, +3384 bytes left, +3404 the size of the chunk in
// flight, +3408 the DMA id of that chunk, +3412 a done flag, +3416 a completion flag the caller polls, +3420 an owned buffer. The
// chunk is at most 64 KiB and a multiple of 64 bytes. The sceSifSetDma four words are data address, destination address, size and
// mode. The ADPCM and XM flavours differ in where the destination comes from (+3368 or the running address at +3392).
// Unknown: what IOP functions 1 and 7 are, what +3368 holds, the meaning of word 1 of sRecv, and whether the stack words are the
// documented sceSifDmaData layout (only the call shape is observed).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
extern int norrath_overlay_sIopFunctionData_11edf08[];
extern int norrath_overlay_sRecv_11edd00[];
int sceSifDmaStat(unsigned id);
unsigned sceSifSetDma(void* data, int count);
void FlushCache(int mode);
void SetIopFunction__FUii(unsigned function, int count);
void __builtin_delete(void* block);
int printf(const char* format, ...);

int IdleTransfer__13VISoundDevice(void* device) {
    unsigned* w = static_cast<unsigned*>(device);
    unsigned pending = w[3408 / 4];
    if (pending) {
        if (sceSifDmaStat(pending) >= 0) return 0;
        w[3408 / 4] = 0;
        if (static_cast<int>(w[3388 / 4]) != -1) {
            norrath_overlay_sIopFunctionData_11edf08[0] = static_cast<int>(w[3404 / 4]);
            norrath_overlay_sIopFunctionData_11edf08[1] = static_cast<int>(w[3388 / 4]);
            w[3388 / 4] = w[3388 / 4] + w[3404 / 4];
            SetIopFunction__FUii(1, 2);
        }
        if (w[3384 / 4] != 0) return 0;
        w[3412 / 4] = 1;
        if (w[3420 / 4]) {
            __builtin_delete(reinterpret_cast<void*>(static_cast<unsigned long>(w[3420 / 4])));
            w[3420 / 4] = 0;
        }
        if (static_cast<int>(w[3392 / 4]) != -1) {
            norrath_overlay_sIopFunctionData_11edf08[0] = static_cast<int>(w[3396 / 4]);
            norrath_overlay_sIopFunctionData_11edf08[1] = static_cast<int>(w[3400 / 4]);
            SetIopFunction__FUii(7, 2);
        }
        if (w[3416 / 4]) *words_at(w[3416 / 4]) = 1;
        return 0;
    }
    int remaining = static_cast<int>(w[3384 / 4]);
    if (remaining <= 0) {
        if (remaining < 0) w[3384 / 4] = 0;
        return 0;
    }
    if (norrath_overlay_sRecv_11edd00[1] == 1) return 0;
    int chunk = remaining > 0xffff ? 0x10000 : ((remaining + 63) & -64);
    FlushCache(0);
    unsigned dma[4];
    dma[0] = w[3380 / 4];
    dma[1] = (static_cast<int>(w[3388 / 4]) == -1) ? w[3392 / 4] : w[3368 / 4];
    dma[2] = static_cast<unsigned>(chunk);
    dma[3] = 0;
    FlushCache(0);
    unsigned id = sceSifSetDma(dma, 1);
    w[3408 / 4] = id;
    if (id == 0) {
        printf("Sample data transfer failed..\n");
        return -1;
    }
    w[3404 / 4] = static_cast<unsigned>(chunk);
    w[3380 / 4] = w[3380 / 4] + static_cast<unsigned>(chunk);
    w[3384 / 4] = static_cast<unsigned>(remaining - chunk);
    if (static_cast<int>(w[3388 / 4]) == -1) w[3392 / 4] = w[3392 / 4] + static_cast<unsigned>(chunk);
    if (remaining - chunk < 0) w[3384 / 4] = 0;
    return 0;
}
}
