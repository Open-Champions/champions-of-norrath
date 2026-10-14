#include <cstdio>
#include "views.cpp"
int main() {
    if (sizeof norrath_archive_vblank_count_view != 4u) return 1;
    if (sizeof norrath_archive_vblank_field_view != 1u) return 2;
    if (sizeof norrath_archive_vblank_semaphore_view != 4u) return 3;
    if (norrath_archive_vblank_count_view != 0u || norrath_archive_vblank_field_view != 0 ||
        norrath_archive_vblank_semaphore_view != 0) return 4;
    std::puts("vblank views contract passed");
    return 0;
}
