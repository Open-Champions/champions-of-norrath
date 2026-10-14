// Observed: Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi at 0x012211a8, 364 bytes.
// Arguments: the pool, a bucket to copy in (only its first 16 bytes are read) and an entry index "position".
// Pool words (offsets in bytes): +0 count, +4 capacity, +8 entries (24-byte records), +12 free-record head,
// +16 first live record. Record words: +16 and +20.
//  1. position == 0x7fffffff: return Add__t6VIPool1Z10VIEEBucketRC10VIEEBucket(pool, bucket) (a tail call).
//  2. If pool+12 is -1: pool+4 becomes twice itself, or 1 when it was zero; Allocate__t6VIPool1Z10VIEEBucket(pool)
//     is called and a negative result returns -1. The free head is then read again.
//  3. slot = record[free head] (pool+8 base, 24 bytes each). When slot is non-zero, its first 16 bytes
//     are copied from the bucket (two unaligned 8-byte copies, both loaded before either is stored).
//  4. pool+12 receives slot+16 (the next free record); slot+20 receives record[position]+20.
//  5. slot+16 receives position. If position equals pool+16, pool+16 receives the slot index; otherwise
//     record[record[position]+20]+16 receives the slot index. record[position]+20 receives the slot index.
//  6. pool+0 is incremented; the result is the slot index.
// Inferred: the pool is a doubly linked list with +16 as next and +20 as previous link; this inserts the
// bucket before the record at "position", and free records are chained through +16. 0x7fffffff is the
// iterator end marker, so inserting at the end appends.
// Unknown: the bucket's fields in the first 16 bytes, pool+20, and what bucket bytes 16..23 hold (never read here).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Add__t6VIPool1Z10VIEEBucketRC10VIEEBucket(void* pool, const void* bucket);
int Allocate__t6VIPool1Z10VIEEBucket(void* pool);

struct __attribute__((packed)) Unaligned64 { unsigned long long value; };

static inline unsigned* record(const unsigned* p, unsigned index) {
    return reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(p[2])) + 6 * index;
}

int Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(void* pool, const void* bucket, int position) {
    unsigned* p = static_cast<unsigned*>(pool);
    if (position == 0x7fffffff) return Add__t6VIPool1Z10VIEEBucketRC10VIEEBucket(pool, bucket);
    unsigned slotIndex = p[3];
    if (slotIndex == 0xffffffffu) {
        p[1] = p[1] != 0 ? p[1] * 2 : 1;
        if (Allocate__t6VIPool1Z10VIEEBucket(pool) < 0) return -1;
        slotIndex = p[3];
    }
    unsigned* slot = record(p, slotIndex);
    if (slot != 0) {
        const unsigned* from = static_cast<const unsigned*>(bucket);
        const Unaligned64* source = reinterpret_cast<const Unaligned64*>(from);
        unsigned long long first = source[0].value;
        unsigned long long second = source[1].value;
        reinterpret_cast<Unaligned64*>(slot)[0].value = first;
        reinterpret_cast<Unaligned64*>(slot)[1].value = second;
        slotIndex = p[3];
    }
    p[3] = record(p, slotIndex)[4];
    record(p, slotIndex)[5] = record(p, static_cast<unsigned>(position))[5];
    record(p, slotIndex)[4] = static_cast<unsigned>(position);
    if (static_cast<unsigned>(position) == p[4]) p[4] = slotIndex;
    else record(p, record(p, static_cast<unsigned>(position))[5])[4] = slotIndex;
    record(p, static_cast<unsigned>(position))[5] = slotIndex;
    p[0] = p[0] + 1;
    return static_cast<int>(slotIndex);
}
}
