#include "clear.cpp"

static int pending;          // buffers still in the pool
static int released[16];
static int released_count;
static int release_fail_at = -1;
static int clear_result;
static int clear_calls;
static void* last_pool;
static int begin_calls, end_calls;
static unsigned char object[700];

extern "C" int Begin__Ct6VIPool1ZP12VICollBuffer(void* pool) { last_pool = pool; ++begin_calls; return 100 + pending; }
extern "C" int End__Ct6VIPool1ZP12VICollBuffer(void*) { ++end_calls; return 100; }
extern "C" int ReleaseCollBuffer__9VICollidei(void* o, int buffer) {
    if (o != object) return -9;
    released[released_count] = buffer;
    int fail = released_count == release_fail_at;
    ++released_count;
    --pending;
    return fail ? -2 : 0;
}
extern "C" int Clear__t6VIPool1ZP12VICollBuffer(void* pool) { ++clear_calls; (void)pool; return clear_result; }

int main() {
    pending = 0;
    if (Clear__9VICollide(object) != 0 || clear_calls != 1 || released_count != 0) return 1;
    if (last_pool != object + 528) return 2;
    pending = 3; clear_calls = 0; begin_calls = 0; end_calls = 0;
    if (Clear__9VICollide(object) != 0 || released_count != 3 || clear_calls != 1) return 3;
    if (released[0] != 103 || released[1] != 102 || released[2] != 101) return 4;
    if (begin_calls != 4 || end_calls != 4) return 5;
    pending = 3; released_count = 0; clear_calls = 0; release_fail_at = 1;
    if (Clear__9VICollide(object) != -1 || released_count != 2 || clear_calls != 0) return 6;
    pending = 0; release_fail_at = -1; clear_result = -4;
    if (Clear__9VICollide(object) != -1) return 7;
    return 0;
}
