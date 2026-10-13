// Host check only; not an EE build input. Links malloc_unlock.cpp with a stub SignalSema.
#include <cstdio>
extern "C" {
int __sce_sema_id = 7;
int norrath_player_malloc_lock_owner_view = 100;
int norrath_player_malloc_lock_count_view = 2;
void __malloc_unlock(void*);
}
static int signals, last_signal;
extern "C" int SignalSema(int s) { ++signals; last_signal = s; return 0; }
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)
int main() {
    int r;
    __malloc_unlock(&r);                                 // 2 -> 1: still held
    CHECK(signals == 0 && norrath_player_malloc_lock_count_view == 1 && norrath_player_malloc_lock_owner_view == 100, 1);
    __malloc_unlock(&r);                                 // 1 -> 0: release
    CHECK(signals == 1 && last_signal == 7 && norrath_player_malloc_lock_owner_view == -1 && norrath_player_malloc_lock_count_view == 0, 2);
    norrath_player_malloc_lock_count_view = 0;           // no guard: underflow still decrements
    __malloc_unlock(&r);
    CHECK(norrath_player_malloc_lock_count_view == -1 && signals == 1, 3);
    std::puts("malloc unlock ok");
    return 0;
}
