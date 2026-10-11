#include <cstdio>
static int seen[2][6];
static int calls;
extern "C" int __sce_sema_id = -1, __sce_eh_sema_id = -1;
extern "C" int CreateSema(int* words) {
    seen[calls][1] = words[1];
    seen[calls][2] = words[2];
    return 40 + calls++;
}
extern "C" void supplement_crt0();
int main() {
    supplement_crt0();
    if (calls != 2 || __sce_sema_id != 40 || __sce_eh_sema_id != 41) return 1;
    if (seen[0][1] != 1 || seen[0][2] != 1 || seen[1][1] != 1 || seen[1][2] != 1) return 2;
    std::puts("supplement_crt0 semaphore requests passed");
}
