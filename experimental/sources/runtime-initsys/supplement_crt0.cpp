// Observed supplement_crt0 at 0x002dd6b0. Two semaphore requests are stored in
// the named identifier words. Words other than the two written fields are not
// stored by this function; what the service reads there is unknown.
extern "C" {
int CreateSema(int* words);
extern int __sce_sema_id;
extern int __sce_eh_sema_id;
void supplement_crt0() {
    int first[6];
    int second[6];
    first[1] = 1;
    first[2] = 1;
    second[1] = 1;
    second[2] = 1;
    __sce_sema_id = CreateSema(first);
    __sce_eh_sema_id = CreateSema(second);
}
}
