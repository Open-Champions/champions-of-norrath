// Observed objectGetSaveData__FPci at 0x00142ce8, 508 bytes; its frame is 57120
// bytes. Arguments: a destination and the room left in it. Locals: a scratch
// string pool of 5008 bytes, a 52000 (0xcb20) byte object-data buffer and three
// ints (a count, an "eligible" count and an "omitted" count).
// It tries passes 0, 1, 2, ... of the local routine
// getDataForAllObjects__FPciPiiT2T2(data, 52000, &count, pass, &eligible,
// &omitted). Before each call poolStart and poolEnd (0x0048689c, 0x004868a0)
// are both set to the scratch pool and the two outputs are zeroed. For a pass:
//   - a result of -1 goes straight to the next pass;
//   - a result above 52000 returns -2;
//   - poolLen = poolEnd - poolStart after the call; poolLen of 5000 or more
//     returns -2;
//   - when room < result + poolLen + 8 and eligible >= pass, the next pass is
//     tried; when eligible < pass the code carries on as if it fitted;
//   - an eligible count that is nonzero but not above the pass returns -1.
//     Zero skips this test, so a zero eligible count is accepted in the
//     carry-on case even when the data did not fit (observed; the copies below
//     are not limited by room).
// Then the result bytes of data go to destination + 8, the poolLen pool bytes
// follow them, and the header is written: the low halfword of count at +0, the
// halfword 1 at +2 and result + 8 at +4. poolStart becomes destination + 8 +
// result, poolEnd that plus poolLen rounded up to make (end - destination) a
// multiple of 4, and the return value is end - destination.
// Inferred: "eligible" is a count of objects whose flag word has bits 0x3000
// set, from the callee's counter; that later passes thin them out. Not
// established: what the flags mean, what "omitted" counts (it is only written),
// what the halfword 1 in the header stands for.
extern "C" {
extern char* poolStart;
extern char* poolEnd;
int getDataForAllObjects__FPciPiiT2T2(char* data, int size, int* count, int pass, int* eligible, int* omitted);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ size);
}

extern "C" int objectGetSaveData__FPci(char* destination, int room) {
    char scratch[5008];
    char data[52000];
    int count;
    int eligible;
    int omitted;
    char* const body = destination + 8;
    int pass = -1;
    for (;;) {
        ++pass;
        poolStart = scratch;
        poolEnd = scratch;
        omitted = 0;
        eligible = 0;
        const int result = getDataForAllObjects__FPciPiiT2T2(data, 52000, &count, pass, &eligible, &omitted);
        if (result == -1) continue;
        if (result > 52000) return -2;
        const int poolLen = static_cast<int>(poolEnd - poolStart);
        if (poolLen >= 5000) return -2;
        if (room < result + poolLen + 8) {
            if (eligible >= pass) continue;
        }
        if (eligible != 0) {
            if (!(pass < eligible)) return -1;
        }
        memcpy(body, data, result);
        memcpy(body + result, scratch, poolLen);
        *reinterpret_cast<unsigned short*>(destination) = static_cast<unsigned short>(count);
        *reinterpret_cast<unsigned short*>(destination + 2) = 1;
        *reinterpret_cast<int*>(destination + 4) = result + 8;
        poolStart = body + result;
        char* end = body + result + poolLen;
        while ((end - destination) & 3) ++end;
        poolEnd = end;
        return static_cast<int>(end - destination);
    }
}
