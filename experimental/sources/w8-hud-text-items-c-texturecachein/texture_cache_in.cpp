// Observed textureCacheIn__FP8_texture at 0x0016cf20, 576 bytes.
// Places a texture in a 16-slot ring of 12-byte cache records (0x006e5d80).
// Record: word 0 texture pointer, signed halfwords at 4 (start), 6 (end) and 8
// (lock count). cacheHead (0x0048696c) is the next slot to fill and cacheTail
// (0x00486970) the oldest. The texture's unsigned halfword at 4 is its size
// (1 when zero); its halfwords at 24 and 26 receive the cached address and the
// slot index, and evicting a record clears the halfword at 24 of its texture.
//
// Placement: with a nonempty ring, if textureBufferSize (0x00486960) minus the
// newest record's end is not below the size, the start is that end. Otherwise
// the start is 0 and, when the newest record's start is below the oldest
// record's start, records are evicted from the tail while the newest start
// stays below the new tail's start. Any eviction of a record with a nonzero
// lock count ends the function with nothing placed. Records the new range
// [start, start + size) overlaps are then evicted from the tail: skipped when
// the tail record starts below the start; otherwise the walk evicts while the
// tail record's start is below start + size, and stops at an empty ring or a
// tail record starting below the start. If the slot after the head equals the
// tail, the tail record is evicted too. Finally the record at the head is
// written (start, end = start + size, lock 0, texture), the texture receives
// slot index and (textureBufferStartAddress + start) as halfwords, and
// cacheHead advances. Evictions already done before a locked record is hit
// stay done.
struct NorrathCacheTexture {
    unsigned char before[4];
    unsigned short size;
    unsigned char middle[18];
    short cachedAddress;
    short cacheSlot;
};

struct NorrathCacheRecord {
    NorrathCacheTexture* texture;
    short start;
    short end;
    short lockCount;
    short unobserved;
};

extern "C" NorrathCacheRecord cache[16];
extern "C" int cacheHead;
extern "C" int cacheTail;
extern "C" int textureBufferSize;
extern "C" int textureBufferStartAddress;

extern "C" void textureCacheIn__FP8_texture(NorrathCacheTexture* texture) {
    int size = texture->size;
    if (size == 0) size = 1;
    int head = cacheHead;
    int tail = cacheTail;
    int start;
    bool fits = false;
    if (head != tail) {
        int last = (head - 1) & 15;
        if (!(textureBufferSize - cache[last].end < size)) {
            start = cache[last].end;
            fits = true;
        }
    }
    if (!fits) {
        start = 0;
        int last = (head - 1) & 15;
        if (cache[last].start < cache[tail].start) {
            int slot = tail;
            for (;;) {
                if (cache[slot].lockCount != 0) return;
                cache[slot].texture->cachedAddress = 0;
                tail = (slot + 1) & 15;
                cacheTail = tail;
                slot = tail;
                if (!(cache[last].start < cache[tail].start)) break;
            }
            head = cacheHead;
        }
    }
    if (tail != head && !(cache[tail].start < start)) {
        int stop = start + size;
        int slot = tail;
        for (;;) {
            if (!(cache[slot].start < stop)) {
                head = cacheHead;
                tail = cacheTail;
                break;
            }
            if (cache[slot].lockCount != 0) return;
            cache[slot].texture->cachedAddress = 0;
            tail = (slot + 1) & 15;
            cacheTail = tail;
            slot = tail;
            if (tail == head) {
                head = cacheHead;
                break;
            }
            if (cache[tail].start < start) {
                head = cacheHead;
                break;
            }
        }
    }
    int next = (head + 1) & 15;
    if (next == tail) {
        if (cache[next].lockCount != 0) return;
        head = cacheHead;
        cache[next].texture->cachedAddress = 0;
        cacheTail = (next + 1) & 15;
    }
    cache[head].end = static_cast<short>(start + size);
    cache[head].start = static_cast<short>(start);
    cache[head].lockCount = 0;
    cache[head].texture = texture;
    texture->cacheSlot = static_cast<short>(cacheHead);
    texture->cachedAddress = static_cast<short>(static_cast<unsigned short>(textureBufferStartAddress) + start);
    cacheHead = next;
}
