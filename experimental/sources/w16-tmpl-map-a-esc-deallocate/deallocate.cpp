// Observed: Deallocate__13VIEESurfCacheR13VIEESurfAlloc at 0x01133050 is 1436 bytes. The cache
// holds a pool of 24-byte buckets (VIPool<VIEEBucket> at cache+32, element base at cache+40;
// bucket words +0 start, +4 length, +8 a class word, +12 the index of the bucket's entry in the
// free multimap, -1 when it has none) and the free VIMultiMap<unsigned,int> at cache+76. With
// (cache, request) it uses request word +0 as a bucket index and works as follows; any negative
// result from a callee, or a negative free-multimap slot, makes it return -1 at once.
//  1. Merge pass: when the index is not Begin(pool), take Prev(pool, index); if that bucket has a
//     free-map slot other than -1 and its class word equals the index bucket's, erase the slot
//     from the free multimap (Erase__t10VIMultiMap2ZUiZii), store -1 in its slot word, add the
//     index bucket's length to it, erase the index bucket from the pool and continue with the
//     previous bucket as the index. Then take Next(pool, index); when it is not End(pool) and
//     passes the same test, unlink it from the free multimap likewise, add its length to the
//     index bucket and erase it from the pool.
//  2. When the bucket start is not a multiple of 8192 and the next 8192 boundary lies below the
//     bucket end (compared signed), the tail from the boundary is split into a new bucket (start
//     boundary, length end - boundary, class = request word +4, slot -1) inserted with
//     Insert(pool, &bucket, Next(pool, index)) after shortening the index bucket; the shortened
//     front part goes to the free multimap via AddToFreeMap__13VIEESurfCachei and the tail
//     becomes the index.
//  3. When the index bucket's start is a multiple of 8192: if its length is at least 8192 and
//     not a multiple of 8192, the part past the last 8192 boundary of its end is split off the
//     same way (the new bucket goes to the free multimap, the shortened one keeps the index);
//     with a length of at least 8192 its class word then becomes 255. When the start is not a
//     multiple of 8192 the index is entered into the free multimap and the routine finishes.
//  4. When the class word is 255 the merge pass of step 1 runs again; then AddToFreeMap is called
//     with the index. Finally cache word +20 is reduced by request word +8 and 0 is returned.
// Inferred: a page-aware free-list coalescing step: class 255 marks a bucket made of whole
// pages; the 8192 constant is the page size of the cached surface memory. Names of the request
// words are not claimed. The partial-page split in step 2 puts the page-aligned tail first in
// line. Unknown: the meaning of the class word values other than 255, and the field names.
extern "C" {
int Begin__Ct6VIPool1Z10VIEEBucket(const void* pool);
int End__Ct6VIPool1Z10VIEEBucket(const void* pool);
int Prev__Ct6VIPool1Z10VIEEBucketi(const void* pool, int index);
int Next__Ct6VIPool1Z10VIEEBucketi(const void* pool, int index);
int Erase__t6VIPool1Z10VIEEBucketi(void* pool, int index);
int Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(void* pool, const void* bucket, int position);
int Erase__t10VIMultiMap2ZUiZii(void* map, int index);
int AddToFreeMap__13VIEESurfCachei(void* cache, int index);

static inline unsigned* bucket_word(void* cache, int index, unsigned offset) {
    unsigned char* buckets = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(cache) + 40)));
    return reinterpret_cast<unsigned*>(buckets + index * 24 + offset);
}

// Removes the bucket's entry from the free multimap and clears its slot word.
__attribute__((always_inline)) static inline int unlink_free(void* cache, int index) {
    const int slot = static_cast<int>(*bucket_word(cache, index, 12));
    if (slot < 0) return -1;
    if (Erase__t10VIMultiMap2ZUiZii(static_cast<unsigned char*>(cache) + 76, slot) < 0) return -1;
    *bucket_word(cache, index, 12) = 0xffffffffu;
    return 0;
}

// Merges the index bucket with a free previous and a free next bucket of the same class.
__attribute__((always_inline)) static inline int merge_neighbours(void* cache, void* pool, int& index) {
    if (index != Begin__Ct6VIPool1Z10VIEEBucket(pool)) {
        const int prev = Prev__Ct6VIPool1Z10VIEEBucketi(pool, index);
        if (static_cast<int>(*bucket_word(cache, prev, 12)) != -1 &&
            *bucket_word(cache, prev, 8) == *bucket_word(cache, index, 8)) {
            if (unlink_free(cache, prev) < 0) return -1;
            const int absorbed = index;
            *bucket_word(cache, prev, 4) = *bucket_word(cache, prev, 4) + *bucket_word(cache, absorbed, 4);
            index = prev;
            if (Erase__t6VIPool1Z10VIEEBucketi(pool, absorbed) < 0) return -1;
        }
    }
    const int next = Next__Ct6VIPool1Z10VIEEBucketi(pool, index);
    if (next != End__Ct6VIPool1Z10VIEEBucket(pool) &&
        static_cast<int>(*bucket_word(cache, next, 12)) != -1 &&
        *bucket_word(cache, next, 8) == *bucket_word(cache, index, 8)) {
        if (unlink_free(cache, next) < 0) return -1;
        *bucket_word(cache, index, 4) = *bucket_word(cache, index, 4) + *bucket_word(cache, next, 4);
        if (Erase__t6VIPool1Z10VIEEBucketi(pool, next) < 0) return -1;
    }
    return 0;
}

int Deallocate__13VIEESurfCacheR13VIEESurfAlloc(void* cache, void* request) {
    void* pool = static_cast<unsigned char*>(cache) + 32;
    unsigned* alloc = static_cast<unsigned*>(request);
    int index = static_cast<int>(alloc[0]);
    if (merge_neighbours(cache, pool, index) < 0) return -1;

    unsigned start = *bucket_word(cache, index, 0);
    if ((start & ~0x1fffu) != start) {
        const unsigned remainder = start & 0x1fffu;
        const int boundary = static_cast<int>(remainder == 0 ? start : start - remainder + 8192u);
        const int end = static_cast<int>(*bucket_word(cache, index, 0) + *bucket_word(cache, index, 4));
        if (boundary < end) {
            unsigned split[4];
            const int tail = end - boundary;
            split[0] = static_cast<unsigned>(boundary);
            split[1] = static_cast<unsigned>(tail);
            split[2] = alloc[1];
            split[3] = 0xffffffffu;
            *bucket_word(cache, index, 4) = *bucket_word(cache, index, 4) - static_cast<unsigned>(tail);
            const int created = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(
                pool, split, Next__Ct6VIPool1Z10VIEEBucketi(pool, index));
            if (created < 0) return -1;
            const int front = index;
            index = created;
            if (AddToFreeMap__13VIEESurfCachei(cache, front) < 0) return -1;
        }
    }

    start = *bucket_word(cache, index, 0);
    if ((start & ~0x1fffu) != start) {
        if (AddToFreeMap__13VIEESurfCachei(cache, index) < 0) return -1;
    } else {
        const int length = static_cast<int>(*bucket_word(cache, index, 4));
        if (length >= 8192) {
            const unsigned remainder = static_cast<unsigned>(length) & 0x1fffu;
            if (remainder != 0 && static_cast<int>(static_cast<unsigned>(length) - remainder + 8192u) != length) {
                unsigned split[4];
                const unsigned end = start + static_cast<unsigned>(length);
                const unsigned boundary = end & ~0x1fffu;
                split[0] = boundary;
                split[1] = end - boundary;
                split[2] = alloc[1];
                split[3] = 0xffffffffu;
                *bucket_word(cache, index, 8) = 255;
                *bucket_word(cache, index, 4) = *bucket_word(cache, index, 4) - split[1];
                const int created = Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(
                    pool, split, Next__Ct6VIPool1Z10VIEEBucketi(pool, index));
                if (created < 0) return -1;
                if (AddToFreeMap__13VIEESurfCachei(cache, created) < 0) return -1;
            }
            *bucket_word(cache, index, 8) = 255;
        }
        if (*bucket_word(cache, index, 8) == 255 && merge_neighbours(cache, pool, index) < 0) return -1;
        if (AddToFreeMap__13VIEESurfCachei(cache, index) < 0) return -1;
    }
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(cache) + 20) -= alloc[2];
    return 0;
}
}
