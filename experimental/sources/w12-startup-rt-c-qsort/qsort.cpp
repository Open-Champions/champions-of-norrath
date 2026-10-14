/* Observed qsort at 0x003025d0, 2540 bytes, pinned SLUS_205.65. The comparison sort of the
 * runtime library: qsort(base, count, size, compare), compare(a, b) returning a value
 * that is negative, zero or positive. Every behaviour below was read from the code and
 * confirmed by paired runs that compared the order of the compare calls and the final
 * memory.
 *
 * Element swapping (swaptype): 2 (byte loop over `size` bytes) when the base address or
 * the size is not a multiple of 8, 0 (one 8-byte swap) when both are and the size is 8,
 * otherwise 1 (loop over size / 8 eight-byte words, at least one pass). It is chosen on
 * entry and again each time the tail loop restarts.
 *
 * Small arrays (count < 7): insertion sort. For each element from the second on, while
 * its predecessor compares greater (compare(previous, current) > 0) the two are swapped
 * and the cursor moves back.
 *
 * Larger arrays: choose a pivot, swap it to the front, partition, then handle the parts.
 *   - Pivot: with middle = base + (count / 2) * size, counts below 8 use the middle.
 *     Otherwise first = base, last = base + (count - 1) * size, and for count > 40 each of
 *     the three is replaced by a median of three samples spaced d = (count / 8) * size apart
 *     (first from first, first + d, first + 2d; middle from middle - d, middle, middle +
 *     d; last from last - 2d, last - d, last); the pivot is then the median of first,
 *     middle, last. The median of three (a, b, c) calls compare(a, b) and, when it is
 *     negative, compare(b, c) < 0 ? b : (compare(a, c) < 0 ? c : a); otherwise
 *     compare(b, c) > 0 ? b : (compare(a, c) < 0 ? a : c).
 *   - Partition keeps four cursors: left-equal edge pa and scan pb start at base + size,
 *     right scan pc and right-equal edge pd start at the last element. pb advances while
 *     compare(pb, base) <= 0, swapping equal elements to pa (and marking that a swap
 *     happened); pc retreats while compare(pc, base) >= 0, swapping equal elements to pd
 *     (also marking); when they cross it stops, otherwise it swaps pb and pc (marking)
 *     and moves both.
 *   - With no swap marked the call finishes with the same insertion sort as the small case
 *     (over the whole array) and returns.
 *   - Otherwise the equal runs are moved to the middle: r = min(pa - base, pb - pa) bytes
 *     (signed minimum) are exchanged between base and pb - r, then r = min(pd - pc, end -
 *     pd - size) (unsigned minimum, end = base + count * size) bytes between pb and end - r;
 *     exchanges of r <= 0 bytes do nothing. If pb - pa is larger than size the left part
 *     is sorted by a recursive call with count (pb - pa) / size; if pd - pc is larger than
 *     size the right part (the last pd - pc bytes) becomes the new array and the routine
 *     loops instead of recursing.
 * Inferred: this is the Bentley and McIlroy engineering of qsort. Unknown: the library
 * lineage beyond what the code shows; no source for it was consulted.
 */
extern "C" {
typedef int (*QsortCompare)(const void*, const void*);

static inline void swap_range(char* left, char* right, int swaptype, unsigned size) {
    if (swaptype == 0) {
        long long* l = reinterpret_cast<long long*>(left);
        long long* r = reinterpret_cast<long long*>(right);
        const long long t = *l;
        *l = *r;
        *r = t;
    } else if (swaptype < 2) {
        long long count = size / 8;
        long long* l = reinterpret_cast<long long*>(left);
        long long* r = reinterpret_cast<long long*>(right);
        do {
            const long long t = *l;
            *l++ = *r;
            *r++ = t;
        } while (--count > 0);
    } else {
        long long count = size;
        do {
            const char t = *left;
            *left++ = *right;
            *right++ = t;
        } while (--count > 0);
    }
}

static inline void swap_bytes(char* left, char* right, int swaptype, int length) {
    if (length <= 0) return;
    if (swaptype < 2) {
        long long count = static_cast<unsigned>(length) / 8;
        long long* l = reinterpret_cast<long long*>(left);
        long long* r = reinterpret_cast<long long*>(right);
        do {
            const long long t = *l;
            *l++ = *r;
            *r++ = t;
        } while (--count > 0);
    } else {
        long long count = static_cast<unsigned>(length);
        do {
            const char t = *left;
            *left++ = *right;
            *right++ = t;
        } while (--count > 0);
    }
}

static inline char* median3(char* a, char* b, char* c, QsortCompare compare) {
    if (compare(a, b) < 0) {
        if (compare(b, c) < 0) return b;
        return compare(a, c) < 0 ? c : a;
    }
    if (compare(b, c) > 0) return b;
    return compare(a, c) < 0 ? a : c;
}

static inline void insertion_sort(char* base, unsigned count, unsigned size, int swaptype, QsortCompare compare) {
    char* const end = base + count * size;
    for (char* current = base + size; current < end; current += size)
        for (char* cursor = current; cursor > base && compare(cursor - size, cursor) > 0; cursor -= size)
            swap_range(cursor, cursor - size, swaptype, size);
}

void qsort(void* array, unsigned count, unsigned size, QsortCompare compare) {
    char* base = static_cast<char*>(array);
    for (;;) {
        const int swaptype = ((reinterpret_cast<unsigned long>(base) % 8) || (size % 8)) ? 2 : (size == 8 ? 0 : 1);
        int swapped = 0;
        if (count < 7) {
            insertion_sort(base, count, size, swaptype, compare);
            return;
        }
        char* middle = base + (count / 2) * size;
        if (count > 7) {
            char* first = base;
            char* last = base + (count - 1) * size;
            if (count > 40) {
                const unsigned d = (count / 8) * size;
                first = median3(first, first + d, first + 2 * d, compare);
                middle = median3(middle - d, middle, middle + d, compare);
                last = median3(last - 2 * d, last - d, last, compare);
            }
            middle = median3(first, middle, last, compare);
        }
        swap_range(base, middle, swaptype, size);
        char* pa = base + size;
        char* pb = pa;
        char* pc = base + (count - 1) * size;
        char* pd = pc;
        for (;;) {
            int result;
            while (pb <= pc && (result = compare(pb, base)) <= 0) {
                if (result == 0) {
                    swapped = 1;
                    swap_range(pa, pb, swaptype, size);
                    pa += size;
                }
                pb += size;
            }
            while (pb <= pc && (result = compare(pc, base)) >= 0) {
                if (result == 0) {
                    swapped = 1;
                    swap_range(pc, pd, swaptype, size);
                    pd -= size;
                }
                pc -= size;
            }
            if (pb > pc) break;
            swap_range(pb, pc, swaptype, size);
            swapped = 1;
            pb += size;
            pc -= size;
        }
        if (swapped == 0) {
            insertion_sort(base, count, size, swaptype, compare);
            return;
        }
        char* const end = base + count * size;
        int span = pb - pa;
        const int left = pa - base;
        int moved = left < span ? left : span;
        swap_bytes(base, pb - moved, swaptype, moved);
        unsigned right_gap = pd - pc;
        const unsigned tail = end - pd - size;
        unsigned moved_right = right_gap < tail ? right_gap : tail;
        swap_bytes(pb, end - moved_right, swaptype, static_cast<int>(moved_right));
        if (static_cast<unsigned>(span) > size) qsort(base, static_cast<unsigned>(span) / size, size, compare);
        if (right_gap > size) {
            base = end - right_gap;
            count = right_gap / size;
            continue;
        }
        return;
    }
}
}
