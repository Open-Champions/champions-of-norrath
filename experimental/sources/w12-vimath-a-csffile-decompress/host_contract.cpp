#define NORRATH_HOST_ARENA
#include <string.h>
static unsigned char arena[1 << 18];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
static int callback_calls; static unsigned callback_arg;
extern "C" void norrath_host_call(unsigned function, unsigned argument) { if (function == 0x777) { ++callback_calls; callback_arg = argument; } }
#include "decompress.cpp"

int VIEndian = 1;
static unsigned char stream[4096]; static unsigned stream_pos, stream_len;
static int fail_read_at, reads, seeks; static long long seek_offset; static int seek_origin, seek_result;
static int vec_news, fail_new_at, deletes; static void* deleted; static unsigned alloc_top;
static int uncompress_calls, uncompress_result, bad_length_at, swaps;
static unsigned last_src_len;
extern "C" int Read__6VIFilePvi(void*, void* buffer, int size) {
    ++reads;
    if (reads == fail_read_at) return -1;
    memcpy(buffer, stream + stream_pos, size); stream_pos += size; return size;
}
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void*, long long offset, int origin) { ++seeks; seek_offset = offset; seek_origin = origin; return seek_result; }
extern "C" void VIBufferSwap__FiPUc(int size, unsigned char* d) { ++swaps; for (int i = 0; i < size / 2; ++i) { unsigned char t = d[i]; d[i] = d[size - 1 - i]; d[size - 1 - i] = t; } }
extern "C" void* __builtin_vec_new(unsigned size) {
    ++vec_news;
    if (vec_news == fail_new_at) return nullptr;
    void* p = arena + alloc_top; alloc_top += (size + 15) & ~15u; return p;
}
extern "C" void __builtin_vec_delete(void* p) { ++deletes; deleted = p; }
extern "C" int uncompress(void* dest, unsigned* length, const void*, unsigned src_len) {
    ++uncompress_calls; last_src_len = src_len;
    memset(dest, 0xA0 + uncompress_calls, *length);
    if (uncompress_calls == bad_length_at) *length -= 1;
    return uncompress_result;
}

static void put32(unsigned char* p, unsigned v, bool big) { for (int i = 0; i < 4; ++i) p[i] = (v >> (big ? 24 - 8 * i : 8 * i)) & 255; }
static void put64(unsigned char* p, unsigned long long v, bool big) { for (int i = 0; i < 8; ++i) p[i] = (v >> (big ? 56 - 8 * i : 8 * i)) & 255; }
static void build(unsigned magic, unsigned count, bool big) {
    memset(stream, 0, sizeof stream);
    put32(stream, magic, big); put32(stream + 4, count, big); put64(stream + 8, 0x1122, big); put64(stream + 16, 0x3344, big);
    put64(stream + 24, 0x9876543210ull, big); put32(stream + 32, 100, big);
    unsigned off = 40;
    for (unsigned i = 0; i < count; ++i) { put32(stream + off, 10 + i, big); put32(stream + off + 4, 50 + 10 * i, big); off += 8 + 10 + i; }
    stream_len = off;
}
static void reset() {
    stream_pos = 0; fail_read_at = reads = seeks = vec_news = fail_new_at = deletes = 0; alloc_top = 4096;
    uncompress_calls = uncompress_result = bad_length_at = swaps = 0; seek_result = 0; callback_calls = 0;
    memset(arena, 0, 1024);
}

int main() {
    unsigned char* obj = arena;                      // CSF object: callback word at +0, argument at +4
    int* count = reinterpret_cast<int*>(arena + 64);
    unsigned* table = reinterpret_cast<unsigned*>(arena + 72);
    for (int pass = 0; pass < 2; ++pass) {
        VIEndian = pass == 0 ? 1 : 0;
        build(0x46534543u, 3, pass == 1);
        reset();
        *reinterpret_cast<unsigned*>(obj) = 0x777; *reinterpret_cast<unsigned*>(obj + 4) = 0xabc;
        if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != 0) return 1 + 100 * pass;
        if (*count != 3 || reads != 1 + 6 || seeks != 1 || seek_offset != 0x9876543210ll || seek_origin != 1) return 2 + 100 * pass;
        if (uncompress_calls != 3 || vec_news != 5 || deletes != 1) return 3 + 100 * pass;
        if (callback_calls != 1 + 6 || callback_arg != 0xabc) return 4 + 100 * pass;
        if (pass == 1 && swaps != 6 + 3 * 2) return 5;
        if (pass == 0 && swaps != 0) return 6;
        unsigned char* rec = norrath_host_pointer(*table);
        for (int i = 0; i < 3; ++i) {
            if (*reinterpret_cast<long long*>(rec + 16 * i) != 50 + 10 * i) return 10 + i + 100 * pass;
            unsigned char* data = norrath_host_pointer(*reinterpret_cast<unsigned*>(rec + 16 * i + 8));
            if (data[0] != 0xA0 + i + 1 || data[50 + 10 * i - 1] != 0xA0 + i + 1) return 20 + i + 100 * pass;
        }
        if (last_src_len != 12) return 30 + 100 * pass;
        if (deleted == nullptr) return 31;
    }
    VIEndian = 1;
    build(0x12345678u, 2, false); reset();
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || vec_news != 0 || callback_calls != 0) return 40;
    build(0x46534543u, 2, false);
    reset(); fail_read_at = 1;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || seeks != 0) return 41;
    reset(); fail_new_at = 1;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || vec_news != 1 || *count != 2) return 42;
    reset(); fail_new_at = 2;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || vec_news != 2 || seeks != 0) return 43;
    reset(); seek_result = -1;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || reads != 1) return 44;
    reset(); fail_read_at = 3;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || deletes != 0) return 45;
    reset(); fail_new_at = 4;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || uncompress_calls != 1) return 46;
    reset(); uncompress_result = -3;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || uncompress_calls != 1) return 47;
    reset(); bad_length_at = 2;
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != -1 || uncompress_calls != 2) return 48;
    build(0x46534543u, 0, false); reset();
    if (Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(obj, nullptr, count, table) != 0 || reads != 1 || deletes != 1 || *count != 0) return 49;
    return 0;
}
