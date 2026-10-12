#include "sema_ids.cpp"
int main() {
    if (__sce_sema_id != 0 || __sce_eh_sema_id != 0) return 1;
    if (&__sce_sema_id == &__sce_eh_sema_id) return 2;
    __sce_sema_id = 41;
    if (__sce_eh_sema_id != 0) return 3;
    __sce_eh_sema_id = 77;
    if (__sce_sema_id != 41) return 4;
    return 0;
}
