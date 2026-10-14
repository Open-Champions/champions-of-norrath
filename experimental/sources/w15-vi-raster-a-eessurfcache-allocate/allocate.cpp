// Observed: Allocate__13VIEESurfCacheR13VIEESurfAlloc at 0x01132df8 is 596 bytes in the pinned SLUS-20565 image. The request record r has
// three words: r[0] a bucket index, r[4] a value stored into the bucket's word at +8, r[8] a size. The cache holds a pool of 24-byte
// buckets whose entry-array pointer is the word at cache+40 (the pool object starts at cache+32) and a multimap at cache+76. Bucket words:
// +0 start, +4 size, +8 state (255 appears as the free marker), +12 map index (-1 for none). The routine:
//   1. calls Key__Ct10VIMultiMap2ZUiZii(map, bucket[12]) and keeps the low 24 bits of the word it returns as the free size S; if
//      bucket[12] is negative it returns -1; otherwise it calls Erase__t10VIMultiMap2ZUiZii(map, bucket[12]) (negative -> return -1) and
//      sets bucket[12] = -1;
//   2. if the bucket's state is 255 and S - r[8] >= 8192, it splits off the tail starting at the next 8192-aligned address after
//      start + r[8]: a new bucket {aligned, S - (aligned - start), 255, -1} is inserted into the pool through
//      Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(pool, &new, Next__Ct6VIPool1Z10VIEEBucketi(pool, r[0])) and registered with
//      AddToFreeMap__13VIEESurfCachei(cache, its index) (any negative result -> return -1); S then becomes S minus that tail's size;
//   3. if r[8] < S, it splits off the remainder {start + r[8], S - r[8], r[4], -1} the same way;
//   4. it finally sets bucket[4] = r[8], bucket[8] = r[4], bucket[12] = -1, adds r[8] to the word at cache+20, and returns 0.
// Inferred: a free-block allocator for the GS/EE surface cache: take a free bucket, trim it to the requested size (with the 8192-byte
// page alignment of the tail), put the leftovers back in the free map. The bucket is re-read from the pool after every insert because the
// pool may move.
// Unknown: what the free map's key packs besides the low 24 bits, what r[4] means (the tail remainder receives the same value, which
// may be intended as a free marker), the helpers' behavior (not authored here), and bucket words past +12.
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
const unsigned* Key__Ct10VIMultiMap2ZUiZii(unsigned char* map, int index);
int Erase__t10VIMultiMap2ZUiZii(unsigned char* map, int index);
int Next__Ct6VIPool1Z10VIEEBucketi(unsigned char* pool, int index);
int Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(unsigned char* pool, const unsigned* bucket, int position);
int AddToFreeMap__13VIEESurfCachei(unsigned char* self, int index);

static unsigned char* bucket_at(unsigned char* pool, int index) {
    return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 24;
}

int Allocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* self, unsigned char* request) {
    unsigned char* pool = self + 32;
    unsigned char* map = self + 76;
    int index = *reinterpret_cast<int*>(request);
    unsigned size = *Key__Ct10VIMultiMap2ZUiZii(map, *reinterpret_cast<int*>(bucket_at(pool, index) + 12)) & 0xffffff;
    int link = *reinterpret_cast<int*>(bucket_at(pool, index) + 12);
    if (link < 0) return -1;
    if (Erase__t10VIMultiMap2ZUiZii(map, link) < 0) return -1;
    *reinterpret_cast<int*>(bucket_at(pool, index) + 12) = -1;

    int want = *reinterpret_cast<int*>(request + 8);
    unsigned char* bucket = bucket_at(pool, index);
    int s = static_cast<int>(size);
    if (*reinterpret_cast<int*>(bucket + 8) == 255) {
        if (s - want >= 8192) {
            unsigned start = *reinterpret_cast<unsigned*>(bucket);
            unsigned end = start + want;
            unsigned aligned = end;
            unsigned low = end & 0x1fff;
            if (low != 0) aligned = end - low + 8192;
            unsigned fresh[4];
            fresh[0] = aligned;
            fresh[1] = s - (aligned - start);
            fresh[2] = 255;
            fresh[3] = static_cast<unsigned>(-1);
            int position = Next__Ct6VIPool1Z10VIEEBucketi(pool, index);
            int added = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(pool, fresh, position);
            if (added < 0) return -1;
            if (AddToFreeMap__13VIEESurfCachei(self, added) < 0) return -1;
            s -= static_cast<int>(fresh[1]);
            want = *reinterpret_cast<int*>(request + 8);
        }
    }
    if (want < s) {
        unsigned start = *reinterpret_cast<unsigned*>(bucket_at(pool, index));
        unsigned fresh[4];
        fresh[0] = start + want;
        fresh[1] = s - want;
        fresh[2] = *reinterpret_cast<unsigned*>(request + 4);
        fresh[3] = static_cast<unsigned>(-1);
        int position = Next__Ct6VIPool1Z10VIEEBucketi(pool, index);
        int added = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(pool, fresh, position);
        if (added < 0) return -1;
        if (AddToFreeMap__13VIEESurfCachei(self, added) < 0) return -1;
        want = *reinterpret_cast<int*>(request + 8);
    }
    unsigned char* final = bucket_at(pool, index);
    *reinterpret_cast<int*>(final + 4) = want;
    *reinterpret_cast<int*>(final + 12) = -1;
    *reinterpret_cast<int*>(final + 8) = *reinterpret_cast<int*>(request + 4);
    *reinterpret_cast<int*>(self + 20) += *reinterpret_cast<int*>(request + 8);
    return 0;
}
}
