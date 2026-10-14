// Observed: MemorySegmCalcSegm__6VIFile at 0x01002508, 176 bytes. With the file as this: the word at +52 is a segment count n, the
// word at +56 points to n records of 24 bytes (a 64-bit start at +0 and a 64-bit length at +8; the third doubleword is not read),
// and the 64-bit value at +16 is the current position P.
// It binary-searches the records for the one with start <= P < start + length: mid = (lo + hi) >> 1 over lo = 0, hi = n - 1; if P
// is below the start then hi = mid - 1; else if P is below start + length the search stops; else lo = mid + 1. The loop ends
// when hi < lo. In every case (found or not) it then stores the last mid at +60 and P - start(mid) at +64 as a 64-bit value, and
// returns 0. A search that finds nothing therefore leaves the last probed record, not an error.
// With n <= 0 the loop is skipped and "mid" is whatever register a2 held on entry (the third argument slot); the image uses it
// as the record index. The signature keeps that slot as a third parameter for that reason; callers are not known to set it.
// Inferred: this positions a segment-backed file (see MemorySegmSeek / MemorySegmRead) on the segment that holds P and the offset
// inside it.
// Unknown: the member names, whether n <= 0 ever occurs, and the use of the third doubleword of each record.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
int MemorySegmCalcSegm__6VIFile(unsigned char* file, int, int mid) {
    int count = *reinterpret_cast<int*>(file + 52);
    unsigned char* records = bytes_at(*reinterpret_cast<unsigned*>(file + 56));
    long long position = *reinterpret_cast<long long*>(file + 16);
    int low = 0;
    int high = count - 1;
    if (high >= 0) {
        do {
            mid = (low + high) >> 1;
            unsigned char* record = records + mid * 24;
            long long start = *reinterpret_cast<long long*>(record);
            if (position < start) {
                high = mid - 1;
            } else if (position < start + *reinterpret_cast<long long*>(record + 8)) {
                break;
            } else {
                low = mid + 1;
            }
        } while (low <= high);
    }
    *reinterpret_cast<int*>(file + 60) = mid;
    *reinterpret_cast<long long*>(file + 64) = position - *reinterpret_cast<long long*>(records + mid * 24);
    return 0;
}
}
