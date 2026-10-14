// Host check only; not an EE build input. Build with -DNORRATH_HOST_CHECK.
// Models the 32-bit space as a byte array; refs are offsets into it.
#include <cstdio>
#include <cstring>
#include <vector>
#include <cstdlib>
static unsigned char space[1 << 16];
extern "C" unsigned char* norrath_host_arena = space;
extern "C" const unsigned norrath_host_bins_ref = 0x1000;     // 1032-byte bin image
extern "C" const unsigned norrath_host_current_ref = 0x2000;  // 40-byte record
extern "C" void __malloc_update_mallinfo();
static unsigned& W(unsigned ref) { return *reinterpret_cast<unsigned*>(space + ref); }
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)

static void reset_bins() {
    std::memset(space, 0, sizeof space);
    const unsigned bins = norrath_host_bins_ref;
    for (unsigned k = 0; k < 128; ++k) {            // empty circular headers, as in the image
        const unsigned h = bins + 8 * k;
        W(h + 8) = h; W(h + 12) = h;
    }
}
// Appends a chunk of the given size word to bin k (bk chain from the header).
static unsigned next_chunk = 0x4000;
static void add_chunk(unsigned k, unsigned size_word, unsigned* order_tail, std::vector<unsigned>& list) {
    (void)order_tail;
    const unsigned h = norrath_host_bins_ref + 8 * k, c = next_chunk; next_chunk += 32;
    W(c + 4) = size_word;
    // push at the bk end: header.bk -> ... -> last -> header (walk is via bk)
    unsigned last = h; while (W(last + 12) != h) last = W(last + 12);
    W(last + 12) = c; W(c + 12) = h; list.push_back(size_word & ~3u);
}
int main() {
    const unsigned bins = norrath_host_bins_ref, cur = norrath_host_current_ref;
    // Case 1: top only, size 0x2001 (in-use bit set) -> count 1, total 0x2000.
    reset_bins();
    const unsigned top = 0x3000; W(top + 4) = 0x2001; W(bins + 8) = top;
    for (int i = 0; i < 10; ++i) W(cur + 4 * i) = 0xaaaa0000u + i;
    W(cur) = 0x10000;
    __malloc_update_mallinfo();
    CHECK(W(cur + 4) == 1 && W(cur + 32) == 0x2000 && W(cur + 28) == 0x10000 - 0x2000 && W(cur + 36) == 0x2000, 1);
    for (int i : {2, 3, 4, 5, 6}) CHECK(W(cur + 4 * i) == 0xaaaa0000u + i, 2);  // untouched words
    CHECK(W(cur) == 0x10000, 3);
    // Case 2: top below 16 bytes does not count; chunks in several bins, incl. first and last.
    reset_bins(); W(bins + 8) = top; W(top + 4) = 0xf | 1; W(cur) = 0x40000;
    std::vector<unsigned> sizes; unsigned dummy = 0;
    add_chunk(1, 0x31, &dummy, sizes);   // bin 1 is the first header walked
    add_chunk(1, 0x52, &dummy, sizes);
    add_chunk(5, 0x1001, &dummy, sizes);
    add_chunk(64, 0x4003, &dummy, sizes);
    add_chunk(127, 0x80, &dummy, sizes); // last header walked (bins+1016)
    unsigned total = 0xc; for (unsigned s : sizes) total += s;   // top 0xf & ~3 = 0xc
    __malloc_update_mallinfo();
    CHECK(W(cur + 4) == 5, 4);           // top (<16) not counted, five chunks are
    CHECK(W(cur + 32) == total && W(cur + 28) == 0x40000 - total && W(cur + 36) == 0xc, 5);
    // Case 3: a chunk linked from bin 0's header (bins+0) is not walked.
    reset_bins(); W(bins + 8) = top; W(top + 4) = 0x100; W(cur) = 0x500;
    const unsigned stray = 0x5000; W(stray + 4) = 0x999; W(bins + 12) = stray; W(stray + 12) = bins;
    __malloc_update_mallinfo();
    CHECK(W(cur + 4) == 1 && W(cur + 32) == 0x100, 6);
    // Case 4: bin 128 (past the image's walked range) is not walked.
    reset_bins(); W(bins + 8) = top; W(top + 4) = 0x100; W(cur) = 0x500;
    W(bins + 1024 + 12) = stray; W(stray + 12) = bins + 1024;
    __malloc_update_mallinfo();
    CHECK(W(cur + 4) == 1 && W(cur + 32) == 0x100, 7);
    std::puts("update_mallinfo ok");
    return 0;
}
