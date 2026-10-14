#define NORRATH_HOST_ARENA
#include <cstdint>
#include <cstring>
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return address == 0 ? nullptr : arena + (address - arena_base); }
#include "memseg_open.cpp"
#include <vector>

static std::vector<unsigned> news;
static int calc_calls;
static void* calc_arg;
static bool fail_new;
// The returned value is an arena address, not a host pointer.
extern "C" void* __builtin_vec_new(unsigned size) {
    news.push_back(size);
    return fail_new ? nullptr : reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(arena_base + 0x100));
}
extern "C" void MemorySegmCalcSegm__6VIFile(void* f) { ++calc_calls; calc_arg = f; }

static uint64_t q(const unsigned char* p) { uint64_t v; std::memcpy(&v, p, 8); return v; }

int main() {
    for (int mode : {0, 1, 2, 4, 6, 7, 8, 9, 0x10, 0x11, 0x12, 0x14, 0x15, 0x18, 0x19, 0x1e, 0x20, 0x41, -1, -8})
    for (int count : {0, 1, 3, 7})
    for (int failing = 0; failing < 2; ++failing)
    for (long extra : {0L, 5L, -3L, 0x7fffffffL, -0x7fffffffL - 1}) {
        alignas(8) unsigned char file[256];
        alignas(8) unsigned char segments[16 * 8];
        std::memset(file, 0x6b, sizeof file);
        std::memset(arena, 0x77, sizeof arena);
        for (int i = 0; i < 16 * 8; ++i) segments[i] = static_cast<unsigned char>(i * 5 + 3);
        news.clear(); calc_calls = 0; fail_new = failing;
        int r = MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil(file, count, segments, mode, extra);
        if (*reinterpret_cast<int*>(file + 4) != mode) return 1;
        if ((mode & 7) == 0) {
            if (r != -1 || !news.empty() || calc_calls != 0 || file[52] != 0x6b || file[56] != 0x6b) return 2;
            continue;
        }
        if (*reinterpret_cast<int*>(file + 52) != count || news.size() != 1 || news[0] != unsigned(count) * 24) return 3;
        if (failing) {
            if (r != -1 || *reinterpret_cast<unsigned*>(file + 56) != 0 || calc_calls != 0 || file[32] != 0x6b) return 4;
            continue;
        }
        if (*reinterpret_cast<unsigned*>(file + 56) != arena_base + 0x100) return 5;
        if (r != 0 || calc_calls != 1 || calc_arg != file || *reinterpret_cast<int*>(file + 8) != 0) return 6;
        uint64_t total = 0;
        for (int i = 0; i < count; ++i) {
            const unsigned char* rec = arena + 0x100 + 24 * i;
            if (q(rec) != total || q(rec + 8) != q(segments + 16 * i) || q(rec + 16) != q(segments + 16 * i + 8)) return 7;
            total += q(segments + 16 * i);
        }
        if (count > 0 && arena[0x100 + 24 * count] != 0x77) return 8;   // nothing written past the block
        if (q(file + 32) != total) return 9;
        uint64_t wide = static_cast<uint64_t>(static_cast<int64_t>(extra));
        uint64_t want40 = (mode & 8) ? 0 : wide;
        uint64_t want16 = (mode & 8) ? 0 : (((mode & 0x10) && (mode & 6)) ? wide : 0);
        if (q(file + 40) != want40 || q(file + 16) != want16) return 10;
        if (file[12] != 0x6b || file[24] != 0x6b || file[48] != 0x6b || file[60] != 0x6b) return 11;
    }
    return 0;
}
