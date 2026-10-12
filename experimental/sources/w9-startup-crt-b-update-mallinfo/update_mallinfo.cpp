// Observed __malloc_update_mallinfo at 0x0030af90, 152 bytes, no arguments and
// no result. It walks the player allocator's bin image (__malloc_av_,
// 0x00448488, authored as norrath_player_malloc_bins_view) and refreshes the
// ten-word record at __malloc_current_mallinfo (0x004488b8, authored as
// norrath_player_malloc_current_info_view).
//   top   = word at bins+8; its size is the word at +4 of that chunk, low two
//           bits cleared. The free-block count starts at 1 when that size is
//           16 or more, else 0, and the free total starts at that size.
//   bins  = 127 bin headers at bins+8, bins+16, ... bins+1016. For each, chunks
//           are followed through the word at +12 until the header comes back;
//           every chunk adds one to the count and its size (word +4, low two
//           bits cleared) to the total.
//   Then word 1 = count, word 8 = total, word 7 = word 0 minus total, and word 9
//   = the top chunk size again with low bits cleared. Other words are left alone.
// Inferred: from the dlmalloc layout of the symbol names, words 1, 7, 8 and 9
// are the ordinary-block count, used bytes, free bytes and keep cost. The
// original keeps the top chunk size in a register across the stores; the value
// read after them is only equal if nothing stores to the top chunk in between.
// Unknown: the owner of word 0 (the arena size); it is only read here.
#if defined(NORRATH_HOST_CHECK)
// The host check models the 32-bit address space as offsets from a base.
extern "C" unsigned char* norrath_host_arena;
extern "C" const unsigned norrath_host_bins_ref;
extern "C" const unsigned norrath_host_current_ref;
#else
extern "C" unsigned norrath_player_malloc_bins_view[258];
extern "C" unsigned norrath_player_malloc_current_info_view[10];
#endif

namespace {
struct Chunk {
    unsigned previous_size;
    unsigned size;
    unsigned forward;
    unsigned backward;
};
#if defined(__mips__)
static_assert(sizeof(unsigned) == 4 && sizeof(Chunk) == 16, "EE chunk header");
#endif

#if defined(NORRATH_HOST_CHECK)
Chunk* chunk_at(unsigned ref) { return reinterpret_cast<Chunk*>(norrath_host_arena + ref); }
unsigned* current_words() { return reinterpret_cast<unsigned*>(norrath_host_arena + norrath_host_current_ref); }
unsigned bins_ref() { return norrath_host_bins_ref; }
#else
Chunk* chunk_at(unsigned ref) { return reinterpret_cast<Chunk*>(static_cast<__UINTPTR_TYPE__>(ref)); }
unsigned* current_words() { return norrath_player_malloc_current_info_view; }
unsigned bins_ref() { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&norrath_player_malloc_bins_view[0])); }
#endif
unsigned chunk_size(const Chunk* c) { return c->size & ~3u; }
}

extern "C" void __malloc_update_mallinfo() {
    const unsigned bins = bins_ref();
    const unsigned top = chunk_at(bins)->forward;
    unsigned total = chunk_size(chunk_at(top));
    unsigned count = total >= 16u ? 1u : 0u;
    unsigned header = bins + 8u;
    for (int remaining = 126; remaining >= 0; --remaining, header += 8u) {
        for (unsigned p = chunk_at(header)->backward; p != header; p = chunk_at(p)->backward) {
            ++count;
            total += chunk_size(chunk_at(p));
        }
    }
    unsigned* const info = current_words();
    const unsigned arena = info[0];
    info[1] = count;
    info[8] = total;
    info[7] = arena - total;
    info[9] = chunk_size(chunk_at(chunk_at(bins)->forward));
}
