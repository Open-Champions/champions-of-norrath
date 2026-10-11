#include <cstdio>
extern "C" volatile unsigned norrath_archive_vblank_count_view = 1;
extern "C" volatile unsigned char norrath_archive_vblank_field_view = 0;
extern "C" int norrath_archive_vblank_semaphore_view = 0;
static int waits;
static int seen_semaphore;
static int release_on_wait;
extern "C" int norrath_player_wait_semaphore(int semaphore) {
    seen_semaphore = semaphore;
    ++waits;
    if (waits >= release_on_wait) norrath_archive_vblank_count_view = 2;
    return 0;
}
extern "C" int norrath_archive_engine_sync_vblank();
int main() {
    release_on_wait = 1;
    norrath_archive_vblank_semaphore_view = 44;
    norrath_archive_vblank_field_view = 0;
    if (norrath_archive_engine_sync_vblank() != 0 || waits != 1 || seen_semaphore != 44) return 1;
    waits = 0;
    norrath_archive_vblank_count_view = 1;
    release_on_wait = 2;
    norrath_archive_vblank_field_view = 0x7f;
    if (norrath_archive_engine_sync_vblank() != 127 || waits != 2) return 2;
    waits = 0;
    norrath_archive_vblank_count_view = 1;
    release_on_wait = 1;
    norrath_archive_vblank_field_view = 0xff;
    if (norrath_archive_engine_sync_vblank() != -1) return 3;
    norrath_archive_vblank_count_view = 1;
    norrath_archive_vblank_field_view = 0x80;
    if (norrath_archive_engine_sync_vblank() != -128) return 4;
    std::puts("sync vblank contract passed");
    return 0;
}
