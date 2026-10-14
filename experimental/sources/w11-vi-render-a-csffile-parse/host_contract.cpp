#include "parse.cpp"
#include <string.h>

static int order[32]; static int order_n;
static void* objfile_seen;
static int dtor_n, dtor_flags;
static int decompress_result, open_result, parse_result;
static int seg_count;
static unsigned char* seg_ptr;
static long long open_total; static int open_count; static unsigned char* open_segs; static int open_mode;
static void* parse_args[3];
static void* deleted[8]; static int delete_n;
static void* vec_deleted; static int vec_n;
static unsigned int callback_arg; static int callback_n;
static unsigned char* decompress_object; static void* decompress_file;

extern "C" void __9VIObjFile(void* f) { objfile_seen = f; order[order_n++] = 1; }
extern "C" void _$_9VIObjFile(void* f, int flags) { if (f != objfile_seen) order[order_n++] = 99; ++dtor_n; dtor_flags = flags; order[order_n++] = 7; }
extern "C" int Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm(void* o, void* f, int* count, unsigned char** segs) {
    decompress_object = static_cast<unsigned char*>(o); decompress_file = f;
    order[order_n++] = 2;
    if (decompress_result >= 0) { *count = seg_count; *segs = seg_ptr; }
    return decompress_result;
}
extern "C" int Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(void* f, int c, unsigned char* s, long long t, int m) {
    if (f != objfile_seen) order[order_n++] = 99;
    open_count = c; open_segs = s; open_total = t; open_mode = m; order[order_n++] = 4; return open_result;
}
extern "C" int Parse__10VIESFParseR9VIObjFileR7VIScene(void* p, void* f, void* s) {
    if (f != objfile_seen) order[order_n++] = 99;
    parse_args[0] = p; parse_args[2] = s; order[order_n++] = 5; return parse_result;
}
extern "C" void __builtin_delete(void* p) { deleted[delete_n++] = p; order[order_n++] = 6; }
extern "C" void __builtin_vec_delete(void* p) { vec_deleted = p; ++vec_n; order[order_n++] = 8; }
static void callback(unsigned int arg) { callback_arg = arg; ++callback_n; order[order_n++] = 3; }

static void reset() { order_n = dtor_n = delete_n = vec_n = callback_n = 0; objfile_seen = 0; }

int main() {
    static unsigned int self[4];
    static unsigned char segs[3][16];
    int parse_obj, scene, file;
    self[0] = 0; self[1] = 0x1234;
    seg_ptr = segs[0]; seg_count = 3;
    long long sizes[3] = {10, 20000000000LL, -5};
    for (int i = 0; i < 3; ++i) { memcpy(segs[i], &sizes[i], 8); unsigned int q = 0x4000 + i; memcpy(segs[i] + 8, &q, 4); }

    // decompress failure
    reset(); decompress_result = -1; open_result = parse_result = 0;
    if (Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(self, &file, &parse_obj, &scene) != -1) return 1;
    if (order_n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 7 || dtor_flags != 2) return 2;
    if (decompress_object != reinterpret_cast<unsigned char*>(self) || decompress_file != &file) return 3;

    // open failure, callback present
    reset(); decompress_result = 0; open_result = -2;
    self[0] = static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(&callback));
    if (Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(self, &file, &parse_obj, &scene) != -1) return 4;
    int expect4[] = {1, 2, 3, 4, 7};
    if (order_n != 5) return 5;
    for (int i = 0; i < 5; ++i) if (order[i] != expect4[i]) return 6;
    if (callback_arg != 0x1234 || open_count != 3 || open_segs != segs[0] || open_mode != 0) return 7;
    if (open_total != 10 + 20000000000LL - 5) return 8;
    if (delete_n != 0 || vec_n != 0) return 9;

    // parse failure
    reset(); open_result = 0; parse_result = -3;
    if (Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(self, &file, &parse_obj, &scene) != -1) return 10;
    if (order_n != 6 || order[4] != 5 || order[5] != 7 || delete_n != 0 || vec_n != 0) return 11;
    if (parse_args[0] != &parse_obj || parse_args[2] != &scene) return 12;

    // success: every record's word at +8 is deleted, then the array, then the object is destroyed
    reset(); parse_result = 0;
    if (Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(self, &file, &parse_obj, &scene) != 0) return 13;
    int expect13[] = {1, 2, 3, 4, 5, 6, 6, 6, 8, 7};
    if (order_n != 10) return 14;
    for (int i = 0; i < 10; ++i) if (order[i] != expect13[i]) return 15;
    for (int i = 0; i < 3; ++i) if (deleted[i] != reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(0x4000 + i))) return 16;
    if (vec_deleted != segs[0] || dtor_flags != 2) return 17;

    // no callback, no segments: total 0, no deletes, no array release
    reset(); self[0] = 0; seg_count = 0; seg_ptr = 0;
    if (Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(self, &file, &parse_obj, &scene) != 0) return 18;
    if (callback_n != 0 || open_count != 0 || open_total != 0 || open_segs != 0) return 19;
    if (delete_n != 0 || vec_n != 0 || dtor_n != 1) return 20;
    return 0;
}
