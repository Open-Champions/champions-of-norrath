// Host check only; not an EE build input. Links malloc_lock.cpp with stub kernel calls.
#include <cstdio>
extern "C" {
int __sce_sema_id = 7;
int norrath_player_malloc_lock_owner_view = -1;
int norrath_player_malloc_lock_count_view = 0;
void __malloc_lock(void*);
}
static int thread = 100, waits, last_wait, owner_at_wait, id_calls;
extern "C" int GetThreadId() { ++id_calls; return thread; }
extern "C" int WaitSema(int s) { ++waits; last_wait = s; owner_at_wait = norrath_player_malloc_lock_owner_view; return 0; }
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)
int main() {
    int r;
    __malloc_lock(&r);
    CHECK(waits == 1 && last_wait == 7, 1);              // first lock waits on the semaphore id
    CHECK(owner_at_wait == -1, 2);                       // owner is stored after the wait returns
    CHECK(norrath_player_malloc_lock_owner_view == 100 && norrath_player_malloc_lock_count_view == 1, 3);
    __malloc_lock(&r);                                   // same thread nests without waiting
    CHECK(waits == 1 && norrath_player_malloc_lock_count_view == 2, 4);
    norrath_player_malloc_lock_owner_view = -1; norrath_player_malloc_lock_count_view = 0;
    thread = 200; __sce_sema_id = 9;                     // semaphore id is read fresh each time
    __malloc_lock(&r);
    CHECK(waits == 2 && last_wait == 9 && norrath_player_malloc_lock_owner_view == 200, 5);
    thread = 300;                                        // a different thread while owned waits again
    __malloc_lock(&r);
    CHECK(waits == 3 && norrath_player_malloc_lock_owner_view == 300 && norrath_player_malloc_lock_count_view == 2, 6);
    CHECK(id_calls == 4, 7);
    std::puts("malloc lock ok");
    return 0;
}
