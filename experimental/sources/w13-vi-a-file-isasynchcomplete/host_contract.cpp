#include "is_asynch_complete.cpp"

static int sync_result, sync_calls, sync_mode;
static void* sync_file;
extern "C" int CDSync__6VIFilei(void* f, int mode) { ++sync_calls; sync_file = f; sync_mode = mode; return sync_result; }

int main() {
    alignas(4) unsigned char f[64] = {};
    *(int*)f = 3; sync_result = 0;
    if (IsAsynchComplete__6VIFile(f) != 1 || sync_calls != 1 || sync_file != f || sync_mode != 1) return 1;
    sync_result = 5;
    if (IsAsynchComplete__6VIFile(f) != 0) return 2;
    sync_result = -1;
    if (IsAsynchComplete__6VIFile(f) != 0) return 3;
    *(int*)f = -1; *(int*)(f + 48) = 0;
    if (IsAsynchComplete__6VIFile(f) != -1 || sync_calls != 3) return 4;
    *(int*)(f + 48) = 9;
    if (IsAsynchComplete__6VIFile(f) != 1) return 5;
    return 0;
}
