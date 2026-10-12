#include "mallinfo_r.cpp"

#include <cstdio>

norrath_mallinfo_block __malloc_current_mallinfo;

static char order[8];
static int steps;
static void* lock_arg;
static void* unlock_arg;
static int updates;

extern "C" void __malloc_lock(void* reent) { lock_arg = reent; order[steps++] = 'L'; }
extern "C" void __malloc_unlock(void* reent) {
    unlock_arg = reent;
    order[steps++] = 'U';
    // A change after unlock must not reach the result: the copy was taken earlier.
    for (int& w : __malloc_current_mallinfo.word) w = -1;
}
extern "C" void __malloc_update_mallinfo() {
    order[steps++] = 'M';
    ++updates;
    for (int i = 0; i < 10; ++i) __malloc_current_mallinfo.word[i] = 100 * updates + i;
}

int main() {
    norrath_mallinfo_block out;
    for (int& w : out.word) w = 0x5a5a;
    int reent_marker;
    norrath_mallinfo_block* r = _mallinfo_r(&out, &reent_marker);
    if (r != &out) return 1;
    if (steps != 3 || order[0] != 'L' || order[1] != 'M' || order[2] != 'U') return 2;
    if (lock_arg != &reent_marker || unlock_arg != &reent_marker) return 3;
    for (int i = 0; i < 10; ++i)
        if (out.word[i] != 100 + i) return 4;
    steps = 0;
    r = _mallinfo_r(&out, 0);
    if (r != &out || out.word[0] != 200 || out.word[9] != 209 || lock_arg != 0) return 5;
    std::puts("_mallinfo_r passed");
    return 0;
}
