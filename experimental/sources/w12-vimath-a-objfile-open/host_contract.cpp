#include "open.cpp"
#include <string.h>

static int order[8], n, close_calls, open_calls, post_calls;
static void* open_args[4]; static int open_flags; static long open_size; static int open_a;
static int open_result, post_result;
static unsigned char* closed; static int closed_flag_seen;
static unsigned char* g_obj;
extern "C" void Close__9VIObjFile(void* f) { order[n++] = 1; ++close_calls; closed = static_cast<unsigned char*>(f); closed_flag_seen = *reinterpret_cast<int*>(closed + 560); }
extern "C" int Open__6VIFileiPQ26VIFile7MemSegmil(void* f, int a, void* seg, int flags, long size) {
    order[n++] = 2; ++open_calls; open_args[0] = f; open_args[1] = seg; open_a = a; open_flags = flags; open_size = size;
    return open_result;
}
extern "C" int PostOpen__9VIObjFile(void* f) {
    order[n++] = 3; ++post_calls; open_args[2] = f;
    return post_result + (*reinterpret_cast<int*>(g_obj + 560) == 1 ? 1000 : 0) + *reinterpret_cast<int*>(g_obj + 192) * 10000;
}

static int run(unsigned char* obj, int a, void* seg, long size, int mode, int flag, int ores) {
    g_obj = obj; n = 0; close_calls = open_calls = post_calls = 0; open_result = ores; post_result = 7;
    *reinterpret_cast<int*>(obj + 560) = flag; *reinterpret_cast<int*>(obj + 192) = -99;
    return Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(obj, a, seg, size, mode);
}

int main() {
    static unsigned char obj[640]; int seg;
    if (run(obj, 5, &seg, 77, 0, 0, 0) != 7 + 1000 + 0) return 1;
    if (close_calls != 0 || open_flags != 33 || open_a != 5 || open_size != 77 || open_args[1] != &seg || open_args[0] != obj || open_args[2] != obj) return 2;
    if (run(obj, 6, &seg, 78, 1, 0, 3) != 7 + 1000 + 10000 || open_flags != 50) return 3;
    if (run(obj, 6, &seg, 79, 2, 0, 0) != 7 + 1000 + 20000 || open_flags != 42) return 4;
    if (run(obj, 6, &seg, 79, 9, 0, 0) != 7 + 1000 + 90000 || open_flags != 42) return 5;
    // already open: close first
    if (run(obj, 1, &seg, 1, 1, 1, 0) == 0 || close_calls != 1 || order[0] != 1 || order[1] != 2 || order[2] != 3 || closed != obj || closed_flag_seen != 1) return 6;
    // failed base open: -1, no PostOpen, fields untouched
    if (run(obj, 1, &seg, 1, 1, 0, -4) != -1 || post_calls != 0 || *reinterpret_cast<int*>(obj + 192) != -99 || *reinterpret_cast<int*>(obj + 560) != 0) return 7;
    return 0;
}
