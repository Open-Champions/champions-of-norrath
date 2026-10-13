// Observed: IdleTransfer__13VISoundDevice at 0x01125f70 is 480 bytes. It returns 0
// except when a DMA could not be queued (-1). Offsets are device bytes; "word N"
// below means the 32-bit word at byte N. The words involved are: 3380 source
// pointer, 3384 bytes remaining, 3388 and 3392 two destination-like values (-1 =
// unused), 3396 and 3400 a pair copied to the second SetIopFunction call, 3404 size
// of the chunk in flight, 3408 the DMA id (0 = none in flight), 3412 a completion
// flag set to 1, 3416 a pointer to an int set to 1 on completion, 3420 an owned
// pointer released on completion, 3368 the fallback destination.
//  - A DMA is in flight (word 3408 nonzero): if sceSifDmaStat(id) >= 0 it is still
//    running and nothing happens. Otherwise word 3408 = 0; if word 3388 is not -1
//    then sIopFunctionData[0] = word 3404, sIopFunctionData[1] = word 3388, word 3388
//    += word 3404 (before the call) and SetIopFunction(1, 2) is called. If bytes remain
//    it returns. Otherwise word 3412 = 1, the pointer in 3420 (if any) is passed to
//    __builtin_delete and cleared; if word 3392 is not -1 then sIopFunctionData[0] =
//    word 3396, sIopFunctionData[1] = word 3400 and SetIopFunction(7, 2) is called;
//    the int pointer in word 3416, if nonzero, receives 1.
//  - Nothing in flight: a remaining count of zero or less only clears itself when
//    negative. For a positive count, if the word at index 1 of the local sRecv
//    (0x011edd00; another local sRecv exists at 0x011e2600) equals 1 it returns. Else
//    chunk = 0x10000 when more than 0xffff remain, otherwise the count rounded up to 64.
//    FlushCache(0); a four-word record {source, destination, chunk, 0} is built
//    (destination = word 3392 if word 3388 is -1, else word 3368); FlushCache(0);
//    sceSifSetDma(record, 1); word 3408 = result. A zero result prints "Sample data
//    transfer failed..\n" and returns -1. Otherwise word 3404 = chunk, the source
//    pointer advances by the chunk, the remaining count drops by it and, if word 3388
//    is -1, word 3392 advances by the chunk too; a negative remaining count is then
//    clamped to zero.
// Inferred: this feeds sample data to the IOP in chunks of up to 64 KiB; the record is
// the SIF DMA descriptor. Unknown: what the numbers 1, 7 (SetIopFunction ids) mean.
extern "C" {
extern int sRecv[];
extern int sIopFunctionData[];

int sceSifDmaStat(int id);
int sceSifSetDma(void* record, int count);
void FlushCache(int mode);
void SetIopFunction__FUii(unsigned function, int count);
void __builtin_delete(void* pointer);
int printf(const char* format, ...);

int IdleTransfer__13VISoundDevice(void* device) {
    int* w = static_cast<int*>(device);
    int remaining;
    if (w[852] != 0) {
        if (sceSifDmaStat(w[852]) >= 0) return 0;
        w[852] = 0;
        if (w[847] != -1) {
            sIopFunctionData[0] = w[851];
            sIopFunctionData[1] = w[847];
            w[847] = w[847] + w[851];
            SetIopFunction__FUii(1, 2);
        }
        if (w[846] != 0) return 0;
        void* owned = reinterpret_cast<void*>(w[855]);
        w[853] = 1;
        if (owned != nullptr) {
            __builtin_delete(owned);
            w[855] = 0;
        }
        if (w[848] != -1) {
            sIopFunctionData[0] = w[849];
            sIopFunctionData[1] = w[850];
            SetIopFunction__FUii(7, 2);
        }
        int* flag = reinterpret_cast<int*>(w[854]);
        if (flag != nullptr) *flag = 1;
        return 0;
    }
    remaining = w[846];
    if (remaining > 0) {
        if (sRecv[1] == 1) return 0;
        int chunk = remaining > 0xffff ? 0x10000 : (remaining + 63) & ~63;
        alignas(16) int record[4];
        FlushCache(0);
        record[0] = w[845];
        record[1] = w[847] == -1 ? w[848] : w[842];
        record[2] = chunk;
        record[3] = 0;
        FlushCache(0);
        int id = sceSifSetDma(record, 1);
        w[852] = id;
        if (id == 0) {
            printf("Sample data transfer failed..\n");
            return -1;
        }
        remaining = w[846] - chunk;
        int source = w[845] + chunk;
        w[851] = chunk;
        w[845] = source;
        w[846] = remaining;
        if (w[847] == -1) w[848] = w[848] + chunk;
    }
    if (remaining < 0) w[846] = 0;
    return 0;
}
}
